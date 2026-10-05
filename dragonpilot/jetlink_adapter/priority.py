"""Short-lived scene guards for a slower lead-only host; thresholds require road validation."""
import math

from jetlink.vision import RATES

LOCAL_CONFIDENCE = .65
PHONE_CONFIDENCE = .5
NEAR_TIME = 2.0
SCENE_HOLD = .25
AGE_HOLD = .15
REENTRY_MARGIN = .10


def speed_budget(speed):
  points = ((0., .5), (15., .4), (25., .32), (35., .25))
  speed = max(0., speed)
  for (x0, y0), (x1, y1) in zip(points, points[1:]):
    if speed <= x1:
      return y0+(y1-y0)*(speed-x0)/(x1-x0)
  return .25


def lead_values(lead):
  try:
    if isinstance(lead, dict):
      result = (float(lead['prob']), *(float(lead[k][0]) for k in ('x', 'y', 'v')))
    else:
      result = (float(lead.prob), float(lead.x[0]), float(lead.y[0]), float(lead.v[0]))
    return result if all(math.isfinite(v) for v in result) else None
  except (AttributeError, IndexError, KeyError, TypeError, ValueError):
    return None


class LeadPriorityGuard:
  def __init__(self):
    self.blocked_until = 0.
    self.blocked_capture = -math.inf
    self.blocked_reason = ''
    self.was_phone = False
    self.last_local_present = None
    self.last_local_target = None
    self.local_absent_since = None

  def _block(self, now, capture, reason, hold):
    self.blocked_until = max(self.blocked_until, now+hold)
    self.blocked_capture = max(self.blocked_capture, capture)
    self.blocked_reason = reason
    self.was_phone = False

  def choose(self, model_msg, phone, snapshot, *, now, ego_speed):
    """Return rejection reason (None means allowed), effective age budget, guard state."""
    local = [lead_values(lead) for lead in list(model_msg.leadsV3)[:2]]
    projected = [lead_values(lead) for lead in (phone or [])[:2]]
    local_capture = float(model_msg.timestampEof)/1e9
    if not 0 < local_capture <= now or now-local_capture > 1.:
      local_capture = now
    main = local[0] if local else None
    if main and main[0] >= LOCAL_CONFIDENCE and main[1] > 0:
      self.last_local_present = now
      self.last_local_target = main
      self.local_absent_since = None
    elif main and main[0] <= .2:
      if self.local_absent_since is None:
        self.local_absent_since = now
    else:
      self.local_absent_since = None

    if phone is None:
      if self.was_phone:
        self._block(now, local_capture, 'phone_unavailable', AGE_HOLD)
      self.was_phone = False
      return None, 0., bool(self.blocked_reason)

    hz = snapshot['target_hz']
    if type(hz) is not int or hz not in RATES:
      return 'phone_rate_invalid', 0., True
    base = max(.3, 2.5/hz)
    speed_limit = speed_budget(ego_speed)
    closing = max((ego_speed-p[3] for p in projected if p and p[0] >= PHONE_CONFIDENCE), default=0.)
    # Local lead.v uses local model velocity as its ego baseline.
    model_speed = float(model_msg.velocity.x[0]) if len(model_msg.velocity.x) else ego_speed
    if math.isfinite(model_speed):
      closing = max(closing, max((model_speed-p[3] for p in local if p and p[0] >= LOCAL_CONFIDENCE), default=0.))
    closing_limit = max(.10, 2.0/closing) if closing > 0 else base
    budget = min(base, speed_limit, closing_limit)
    age = now-snapshot['capture_boot_s']
    if age > budget:
      reason = 'phone_age_closing' if closing_limit < min(base, speed_limit) else 'phone_age_high_speed'
      self._block(now, local_capture, reason, AGE_HOLD)
      return reason, budget, True

    near = min(70., max(12., 2.*ego_speed))
    reason = None
    local_is_newer = local_capture-snapshot['capture_boot_s'] >= .025
    for own, remote in zip(local, projected):
      if not local_is_newer or not own or own[0] < LOCAL_CONFIDENCE or own[1] <= 0:
        continue
      tolerance = max(5., .2*own[1])
      if own[1] <= near and (not remote or remote[0] < PHONE_CONFIDENCE or
                             own[1] < remote[1]-tolerance):
        reason = 'local_new_near_lead'
        break
      if remote and remote[0] >= PHONE_CONFIDENCE and (
          abs(own[1]-remote[1]) > tolerance or abs(own[2]-remote[2]) > .8):
        reason = 'local_phone_target_disagreement'
        break

    # A weaker local model which never saw the phone's target is not evidence
    # of a cut-out. Require a recently seen local target and sustained absence.
    if reason is None and local_is_newer and main and main[0] <= .2 and projected and projected[0] and projected[0][0] >= .65:
      if (self.last_local_present is not None and now-self.last_local_present <= .6 and
          self.local_absent_since is not None and now-self.local_absent_since >= .075 and
          snapshot['capture_boot_s'] < self.local_absent_since):
        prior = self.last_local_target
        if abs(prior[1]-projected[0][1]) <= max(6., .2*prior[1]):
          reason = 'local_recent_cutout'
    if reason:
      self._block(now, local_capture, reason, SCENE_HOLD)
      return reason, budget, True
    if now < self.blocked_until or snapshot['capture_boot_s'] <= self.blocked_capture:
      return 'guard_hold_'+self.blocked_reason, budget, True
    if (self.blocked_reason or not self.was_phone) and budget-age < REENTRY_MARGIN:
      return 'guard_wait_fresher_phone', budget, True
    self.blocked_reason = ''
    self.was_phone = True
    return None, budget, False
