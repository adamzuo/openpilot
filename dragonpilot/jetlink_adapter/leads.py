"""Publish fresh phone leads over local leads; local plans and future trajectories stay local."""
from __future__ import annotations

import math

from jetlink.vision import LAYOUT, boot_seconds, fresh_snapshot
from dragonpilot.jetlink_adapter.priority import LeadPriorityGuard

FIELDS = ('confidence', 'distance_m', 'lateral_m', 'velocity_mps', 'acceleration_mps2',
          'distance_std_m', 'lateral_std_m', 'velocity_std_mps', 'acceleration_std_mps2')


def project_phone_leads(snapshot, *, now, ego_speed, yaw_rate, context_valid):
  """Current-step only. Integrate short capture-to-publish motion, not a future model plan."""
  if not context_valid:
    return None, 'vehicle_or_calibration_unavailable'
  if not snapshot:
    return None, 'waiting_for_phone'
  if not fresh_snapshot(snapshot, now):
    return None, 'phone_expired_or_invalid'
  try:
    if snapshot.get('layout') != LAYOUT or snapshot.get('coordinates') != 'model' or snapshot.get('context_valid') is not True:
      return None, 'phone_context_or_layout_invalid'
    age = now - snapshot['capture_boot_s']
    captured_v = float(snapshot['ego_speed_capture'])
    captured_a = float(snapshot['ego_accel_capture'])
    captured_yaw = float(snapshot['yaw_rate_capture'])
    if not all(math.isfinite(v) for v in (now, age, captured_v, captured_a, captured_yaw, ego_speed, yaw_rate)):
      return None, 'non_finite_context'
    values = snapshot['leads']
    if any(len(values[name]) != 3 for name in FIELDS):
      return None, 'phone_shape_invalid'
    decoded = [[float(values[name][i]) for name in FIELDS] for i in range(3)]
    if any(not all(math.isfinite(v) and abs(v) < 1e30 for v in row) or
           not 0 <= row[0] <= 1 or any(s < 0 for s in row[5:]) for row in decoded):
      return None, 'phone_values_invalid'
    # Average observed yaw rate over the short hold interval. Missing lateral
    # target motion remains uncertainty, rather than a fabricated trajectory.
    angle = .5 * (captured_yaw + yaw_rate) * age
    c, s = math.cos(angle), math.sin(angle)
    ego_distance = captured_v * age + .5 * captured_a * age * age
    if abs(angle) < 1e-6:
      ego_x, ego_y = ego_distance, 0.
    else:
      ego_x = ego_distance * s / angle
      ego_y = ego_distance * (1 - c) / angle
    prepared = []
    for i, (prob, x, y, v, a, xs, ys, vs, acs) in enumerate(decoded):
      xp = x + v * age + .5 * a * age * age - ego_x
      yp = y - ego_y
      projected_x, projected_y = c * xp + s * yp, -s * xp + c * yp
      # Keep the schema's absolute lead speed. Phone-source radard uses
      # carState.vEgo for relative conversion instead of a different model's
      # ego-speed prediction. Camera/bumper distance offset stays in radard.
      projected_v = c * (v + a * age)
      projected_a = c * a
      vx = xs * xs + (age * vs)**2 + (.5 * age * age * acs)**2
      vy = ys * ys + age * age  # unresolved lateral motion: 1 m/s noise growth
      prepared.append({'prob': prob, 'probTime': float(i * 2), 't': [0.],
                       'x': [projected_x], 'y': [projected_y], 'v': [projected_v], 'a': [projected_a],
                       'xStd': [math.sqrt(c * c * vx + s * s * vy)],
                       'yStd': [math.sqrt(s * s * vx + c * c * vy)],
                       'vStd': [math.hypot(vs, age * acs)], 'aStd': [acs]})
    if any(not math.isfinite(value) or abs(value) >= 1e30
           for lead in prepared for name in ('x', 'y', 'v', 'a', 'xStd', 'yStd', 'vStd', 'aStd') for value in lead[name]):
      return None, 'projection_invalid'
    return prepared, 'fresh_phone'
  except (KeyError, TypeError, ValueError, OverflowError):
    return None, 'phone_data_invalid'


def apply_phone_leads(model_msg, snapshot, *, ego_speed, yaw_rate, context_valid, now=None, priority_guard=None):
  """Call after fill_model_msg. A local model message exists on every frame."""
  now = boot_seconds() if now is None else now
  prepared, reason = project_phone_leads(snapshot, now=now, ego_speed=ego_speed,
                                         yaw_rate=yaw_rate, context_valid=context_valid)
  if prepared is not None and (type(snapshot.get('frame_id')) is not int or not 0 <= snapshot['frame_id'] <= 0xFFFFFFFF):
    prepared, reason = None, 'phone_frame_invalid'
  guard = priority_guard if priority_guard is not None else LeadPriorityGuard()
  guard_reason, budget, guard_active = guard.choose(model_msg, prepared, snapshot, now=now, ego_speed=ego_speed)
  if prepared is not None and guard_reason:
    prepared, reason = None, guard_reason
  source = 'local' if prepared is None else 'phone' 
  age = 0.
  if snapshot:
    capture = snapshot.get('capture_boot_s')
    if type(capture) in (int, float) and math.isfinite(capture):
      age = min(1e6, max(0., now-capture))
  info = {'source': source, 'reason': reason, 'target_hz': (snapshot or {}).get('target_hz', 0),
          'frame_id': (snapshot or {}).get('frame_id', 0),
          'max_age_s': budget, 'guard_active': guard_active,
          'age_s': age}
  if prepared is not None:
    model_msg.leadsV3 = prepared
  # Travels with the same modelV2 message, so radard can reset source-specific
  # probability filters on a fallback without a second IPC channel or Params IO.
  metadata = model_msg.jetlinkVision
  metadata.source = source
  metadata.reason = reason
  metadata.targetHz = info['target_hz'] if type(info['target_hz']) is int and 0 <= info['target_hz'] <= 65535 else 0
  metadata.frameId = info['frame_id'] if type(info['frame_id']) is int and 0 <= info['frame_id'] <= 0xFFFFFFFF else 0
  metadata.ageSeconds = info['age_s']
  metadata.maxAgeSeconds = budget
  metadata.guardActive = guard_active
  return info
