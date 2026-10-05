"""Rate-limited auxiliary vision. Local policy always drives; link IO stays off modeld."""
from __future__ import annotations

import threading
import time
from dataclasses import dataclass

from jetlink.openpilot.model_state import JetlinkModelState
from jetlink.transport.priority import background_thread

from jetlink.vision import LAYOUT, boot_seconds, vision_rate, fresh_snapshot


@dataclass(frozen=True)
class VisionJob:
  warped: bytes
  packed: bytes
  frame_id: int
  captured: float
  ego_speed: float = 0.0
  ego_accel: float = 0.0
  yaw_rate: float = 0.0
  context_valid: bool = False


class VisionWorker:
  """At most one job, no camera backlog. Only the worker uses the client."""
  def __init__(self, client, publish):
    self.client = client
    self.hz = vision_rate(client.server_info)
    if client.server_info.get("vision_layout") != LAYOUT:
      raise ValueError("Android vision layout needs update: non_mhp_split72_v2 required")
    self.publish = publish
    self.next_at = 0.0
    self.error = None
    self.snapshot = None
    self._job = None
    self._busy = False
    self._stop = False
    self._cv = threading.Condition()
    self._publish_lock = threading.Lock()
    self._reset = True
    self._thread = threading.Thread(target=self._run, name='jetlink-vision', daemon=True)
    self._thread.start()

  def due(self, now):
    if self.error is not None:
      raise RuntimeError('auxiliary vision link failed') from self.error
    return not self._stop and not self._busy and now >= self.next_at

  def submit(self, job):
    with self._cv:
      if not self.due(time.monotonic()):
        return False
      self._busy = True
      self._job = job
      self._cv.notify()
      return True

  def _run(self):
    background_thread()
    while True:
      with self._cv:
        self._cv.wait_for(lambda: self._stop or self._job is not None)
        if self._stop:
          return
        job, self._job = self._job, None
      try:
        # Use actual send start for scheduling: the client's limiter may wait.
        started = time.monotonic()
        result = self.client.infer_vision(job.warped, job.packed, job.frame_id, reset=self._reset)
        self._reset = False
        snapshot = {'valid': True, 'target_hz': self.hz, 'frame_id': job.frame_id,
                    'capture_boot_s': job.captured, 'receive_boot_s': boot_seconds(),
                    'ego_speed_capture': job.ego_speed, 'ego_accel_capture': job.ego_accel,
                    'yaw_rate_capture': job.yaw_rate, 'context_valid': job.context_valid, 'layout': LAYOUT,
                    'coordinates': 'model', 'leads': {k: v.tolist() for k, v in result.items()}}
        snapshot['valid'] = fresh_snapshot(snapshot)
        with self._cv:
          if self._stop:
            return
          self.snapshot = snapshot
        with self._publish_lock:
          if not self._stop:
            self.publish(snapshot)
        with self._cv:
          self.next_at = max(getattr(self.client, '_last_vision_sent', started) + 1.0 / self.hz, time.monotonic())
          self._busy = False
          self._cv.notify_all()
      except Exception as e:
        with self._cv:
          self.error = e
          self.snapshot = None
          self._busy = False
        with self._publish_lock:
          self.publish({'valid': False, 'target_hz': self.hz, 'reason': 'link_failed'})
        return

  def close(self):
    with self._cv:
      self._stop = True
      self.snapshot = None
      self._job = None
      self._cv.notify()
    with self._publish_lock:
      self.publish({'valid': False, 'target_hz': self.hz, 'reason': 'disconnected'})
    self.client.close()
    self._thread.join(timeout=1.5)


class VisionModelState(JetlinkModelState):
  auxiliary_only = True

  def __init__(self, *args, publish, **kwargs):
    super().__init__(*args, **kwargs)
    self.worker = VisionWorker(self.client, publish)

  def tick(self, bufs, transforms, inputs):
    now = time.monotonic()
    if not self.worker.due(now):
      return
    # inherited prepare edits desire[0]; preserve the local model's inputs.
    self.prepare(bufs, transforms, {k: v.copy() for k, v in inputs.items()})
    self._frame_id += 1
    context = inputs.get('jetlink_context')
    if context is None or len(context) != 5:
      captured, ego_speed, ego_accel, yaw_rate, valid = boot_seconds(), 0., 0., 0., False
    else:
      captured, ego_speed, ego_accel, yaw_rate, valid = context
    job = VisionJob(bytes(self._frame.data), self.packed.tobytes(), self._frame_id,
                    float(captured), float(ego_speed), float(ego_accel), float(yaw_rate), bool(valid))
    self.worker.submit(job)
    self._frame = None

  def close(self):
    self.worker.close()
