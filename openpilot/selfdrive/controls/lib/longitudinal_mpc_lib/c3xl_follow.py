"""C3XL 跟车距离档位（可在 设置→巡航 里切换；切换后需重启 openpilot 生效）

档位 0 = 原厂（openpilot 默认，最远）
档位 1..3 = 更近的调校（时距/停止距离/刹车假设）
"""
PROFILES = {
  # C3XL 20261111 晚: 用户实测最舒服的组合
  #   T_FOLLOW 1.25 / COMFORT_BRAKE 3.0（昨天的手感）/ STOP 4.0（停 3~4 m）
  # 参考：官方 BRAKE 2.5 / STOP 6.0，16km/h 目标 15.5m（用户觉得减速太早）
  0: ("原厂(远)",   1.25, 6.0, 2.5),
  1: ("P1 温和",    1.25, 4.0, 3.0),
  2: ("P2 激进",    1.25, 3.0, 3.5),
  3: ("P3 折中",    1.25, 3.5, 3.25),
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
