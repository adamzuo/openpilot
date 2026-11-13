"""C3XL 跟车距离档位（设置→巡航 里切换，切换后【实时生效】不用重启）

档位 0 = 原厂（openpilot 默认，最远）
档位 1..3 = 温和 / 激进 / 折中
档位 4..6 = A / B / C（更紧，BRAKE 更高 → 假定刹车更强）
"""
PROFILES = {
  # C3XL 20261113: 恢复用户喜欢的距离手感（d385318 的 T0.90 / BRAKE3.0 / STOP5.5）
  # 同时保留提速修复、滑行 e2e 保护、前车远死区
  0: ("原厂(远)",   1.25, 7.0, 2.5),
  1: ("P1 温和",    0.90, 5.5, 3.0),
  2: ("P2 激进",    0.90, 4.5, 4.0),
  3: ("P3 折中",    0.90, 5.0, 3.5),
  # C3XL 20261113: 更紧的三档（BRAKE 5.0/5.5 超出车辆实际 0.4g 能力，慎用）
  4: ("A 紧",       0.45, 3.5, 5.0),   # 50km/h ~29m / 2.1s
  5: ("B 更紧",     0.40, 3.0, 5.5),   # 50km/h ~26m / 1.8s
  6: ("C 折中紧",   0.50, 4.0, 4.0),   # 50km/h ~35m / 2.5s
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


def t_follow_override():
  """Non-None 时覆盖 openpilot 原厂的时距（档位 0 用原厂值）"""
  i = _index()
  return None if i == 0 else PROFILES[i][1]


def stop_distance() -> float:
  return PROFILES[_index()][2]


def comfort_brake() -> float:
  return PROFILES[_index()][3]
