"""Pure capability and capture-age checks for auxiliary lead data."""
import time

RATES = (5, 7, 8, 10)
LAYOUT = "non_mhp_split72_v2"


def boot_seconds():
  """Match camerad/common/timing.h, including time spent suspended on Linux."""
  return time.clock_gettime(getattr(time, "CLOCK_BOOTTIME", time.CLOCK_MONOTONIC))


def vision_rate(info):
  hz = info.get('max_inference_hz', 10)
  if not info.get('vision_only_v1') or type(hz) is not int or hz not in RATES:
    raise ValueError('unsupported auxiliary vision capability or frequency')
  return hz


def fresh_snapshot(snapshot, now=None):
  """Consumers must check capture age, not receive age, before using a prediction."""
  try:
    hz = snapshot['target_hz']
    age = (boot_seconds() if now is None else now) - snapshot['capture_boot_s']
    return (hz in RATES and snapshot['valid'] is True and
            0 <= age <= max(0.3, 2.5 / hz))
  except (TypeError, KeyError):
    return False


