"""C3XL 跟车距离档位（可在 设置→巡航 里切换；切换后需重启 openpilot 生效）

档位 0 = 原厂（openpilot 默认，最远）
档位 1..3 = 更近的调校（时距/停止距离/刹车假设）
"""
PROFILES = {
  # C3XL 20261112: 用户按【小模型 CHEDDAR】微调（手机未连接）
  # A 档 = B5.0 / T0.9 / STOP3.0 → 50km/h 目标 35m ≈ 2.5s 时距
  0: ("原厂(远)",   1.25, 6.0, 2.5),
  1: ("P1 温和",    0.90, 3.0, 5.0),
  2: ("P2 激进",    0.90, 2.5, 6.0),
  3: ("P3 折中",    0.90, 3.5, 4.0),
}
DEFAULT = 1


def _index() -> int:
  try:
    from openpilot.common.params import Params
    v = Params().get("C3XLFollowProfile", return_default=True)
    i = int(v) if v not in (None, "") else DEFAULT
  except Exception:
    i = DEFAULT
  return i if i in PROFILES else DEFAULT


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
