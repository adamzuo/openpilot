"""C3XL 跟车距离档位（可在 设置→巡航 里切换；切换后需重启 openpilot 生效）

档位 0 = 原厂（openpilot 默认，最远）
档位 1..3 = 更近的调校（时距/停止距离/刹车假设）
"""
PROFILES = {
  # C3XL 20261110: STOP_DISTANCE 从 3.5/6.0 调大到 5.5/7.0
  # 原实测跟停只剩 1~2 m（MPC 软代价会收敛到比目标更近约 2 m）
  # 目标：实际跟停 3~4 m  →  目标值设 5.5 m（1=温和）
  0: ("原厂(远)",   1.25, 8.5, 2.5),
  1: ("P1 温和",    0.90, 7.0, 3.0),
  2: ("P2 激进",    0.90, 6.0, 4.0),
  3: ("P3 折中",    0.90, 6.5, 3.5),
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
