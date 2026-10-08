"""Full Jetlink model integration; no auxiliary inference or fusion."""

class SmallFace:
  def __init__(self,small):
    self.small=small
    self.lat_delay=0.; self.PLANPLUS_CONTROL=1.; self.desire_key='desire_pulse'
    self.numpy_inputs=small.npy
  def __getattr__(self,name): return getattr(self.small,name)
  def run(self,bufs,transforms,inputs,after_enqueue=None):
    return self.small.run(bufs,transforms,inputs,False)


class FullModel:
  """Upstream full acceleration, explicitly selected while parked only."""
  def __init__(self,small,joined):
    self.small=small; self.joined=joined
    self._proving_frames = 0
    self._last_handovers = joined.handovers
    self.vision_input_names=small.vision_input_names
  def run(self,bufs,transforms,inputs,prepare_only):
    if prepare_only:
      # Warm local history on skipped frames as dp0111pre does.
      self.small.run(bufs,transforms,inputs,True)
      return None
    result = self.joined.run(bufs,transforms,inputs)
    if self.joined.handovers != self._last_handovers:
      self._proving_frames = 0
    self._last_handovers = self.joined.handovers
    self._proving_frames = self._proving_frames + 1 if self.joined.chestnut else 0
    return result

  @property
  def proving(self):
    from jetlink.openpilot.model_state import PROVING_FRAMES
    return bool(self.joined.chestnut and self._proving_frames <= PROVING_FRAMES)


def attach_full(small,w,h):
  from dragonpilot.jetlink_adapter import _api
  api=_api()
  from jetlink.openpilot.joining import join
  return FullModel(small,join(api._parts,w,h,SmallFace(small)))
