"""C3XL 跟车距离档位（设置→巡航 里切换，切换后【实时生效】不用重启）

档位 0 = 原厂（openpilot 默认，最远）
档位 1..3 = 温和 / 激进 / 折中
档位 4..6 = A / B / C（更紧，BRAKE 更高 → 假定刹车更强）
"""
PROFILES = {
  # C3XL 20261113: 0原厂 1温和 2居中 3激进 4小紧 5中紧 6大紧（避开字库缺字：折/极）
  0: ("原厂",    1.25, 7.0, 2.5),
  1: ("温和",    0.90, 5.5, 3.0),
  2: ("居中",    0.90, 5.0, 3.5),
  3: ("激进",    0.90, 4.5, 4.0),
  4: ("小紧",    0.50, 4.0, 4.0),
  5: ("中紧",    0.45, 3.5, 5.0),
  6: ("大紧",    0.40, 3.0, 5.5),
}
DEFAULT = 1


_CACHE = {"t": 0.0, "i": DEFAULT}


def _index() -> int:
  """1 秒缓存读参数（plannerd 每帧都调用，避免频繁读 Params）"""
  import time
  now = time.monotonic()
  if now - _CACHE["t"] > 1.0:
    _CACHE["t"] = now
    try:
      from openpilot.common.params import Params
      v = Params().get("C3XLFollowProfile", return_default=True)
      i = int(v) if v not in (None, "") else DEFAULT
    except Exception:
      i = DEFAULT
    _CACHE["i"] = i if i in PROFILES else DEFAULT
  return _CACHE["i"]


def name() -> str:
  return PROFILES[_index()][0]


# C3XL 20261114: 时距随车速放大（低速保持紧凑，高速自动拉开）
#   v(km/h):   0      40     80     120
_T_SPEED_BP = [0.0, 11.1, 22.2, 33.3]
_T_SPEED_F  = [1.00, 1.00, 1.55, 2.33]
# 高速保底：车速 > 80km/h 时，实际时距不小于这个值（紧档也不会贴太近）
T_HIGH_SPEED_MIN = 1.50
T_HIGH_SPEED_KNEE = 22.2      # m/s = 80 km/h


def _speed_factor(v_ego: float) -> float:
  import numpy as np
  return float(np.interp(v_ego, _T_SPEED_BP, _T_SPEED_F))


def t_follow_override(v_ego=None):
  """Non-None 时覆盖 openpilot 原厂的时距（档位 0 用原厂值）
  传入 v_ego 时按车速放大，并在高速段做保底"""
  i = _index()
  if i == 0:
    return None
  t = PROFILES[i][1]
  if v_ego is None:
    return t
  t *= _speed_factor(v_ego)
  if v_ego > T_HIGH_SPEED_KNEE:
    t = max(t, T_HIGH_SPEED_MIN)
  return t


def stop_distance() -> float:
  return PROFILES[_index()][2]


def comfort_brake() -> float:
  return PROFILES[_index()][3]
