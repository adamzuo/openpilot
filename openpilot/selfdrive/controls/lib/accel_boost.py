import time

import numpy as np
from openpilot.common.constants import CV
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL

ACCEL_BOOST_MAX = 0.2
ACCEL_BOOST_RATE = 0.025
ACCEL_BOOST_PER_OVERRIDE = 0.05
ACCEL_BOOST_MIN_SPEED = 10 * CV.MPH_TO_MS
# C3XL: 模型 e2e 加速度低于此值（在要求刹车）时，补偿才衰减；否则保持
ACCEL_BOOST_DECAY_E2E = -0.3

# C3XL: 大模型加速补偿档位 (名称, 上限 m/s², 爬升速度 m/s²/s, 是否自动, 回落速度)
C3XL_PROFILES = {
  0: ("原厂（踩油门补偿）", 0.2,  0.025,  False, 0.025),
  1: ("温和（0.2 / 16 秒）",  0.2,  0.0125, True,  0.0125),
  2: ("标准（0.3 / 10 秒）",  0.3,  0.03,   True,  0.03),
  3: ("强（0.5 / 8 秒）",     0.5,  0.0625, True,  0.0625),
}
DEFAULT_PROFILE = 1


class AccelBoost:
  def __init__(self):
    self.total_boost = 0.0
    self.boost_this_override = 0.0
    self.boost_eligible = False
    self._params = Params()
    self._idx = DEFAULT_PROFILE
    self._t = 0.0

  def _profile(self):
    """1 秒刷新一次参数，避免每帧读 Params"""
    now = time.monotonic()
    if now - self._t > 1.0:
      self._t = now
      try:
        v = self._params.get("C3XLAccelComp", return_default=True)
        idx = int(v) if v not in (None, "") else DEFAULT_PROFILE
      except Exception:
        idx = DEFAULT_PROFILE
      self._idx = idx if idx in C3XL_PROFILES else DEFAULT_PROFILE
    return C3XL_PROFILES[self._idx]

  def update(self, sm, output_a_target_e2e, output_a_target_mpc, a_cruise):
    enabled = sm['selfdriveState'].enabled
    gas_pressed = sm['carState'].gasPressed
    v_ego = sm['carState'].vEgo
    _, boost_max, ramp_up, auto, ramp_down = self._profile()
    speed_scale = np.interp(v_ego, [ACCEL_BOOST_MIN_SPEED, 2 * ACCEL_BOOST_MIN_SPEED], [0.0, 1.0])
    boost_scale = speed_scale * np.interp(output_a_target_e2e, [-1.0, -0.5, 5.0], [0.0, 1.0, 1.0], right=0.0)
    model_limited = output_a_target_e2e + self.total_boost * boost_scale < min(output_a_target_mpc, a_cruise) - 0.1

    if not enabled or not gas_pressed:
      self.boost_this_override = 0.0
      # Latch eligibility before the override changes the lead plan.
      self.boost_eligible = enabled and model_limited

    if not enabled:
      self.total_boost = 0.0
    elif auto:
      # C3XL: 三态（避免补偿被反复清零，又不削弱刹车）
      #   ① 被大模型压住（需要加速）→ 缓慢爬升
      #   ② 模型在要求刹车（e2e 很负）→ 缓慢衰减
      #   ③ 其它（稳态跟车）→ 保持不动
      if model_limited and v_ego > ACCEL_BOOST_MIN_SPEED:
        self.total_boost = min(boost_max, self.total_boost + ramp_up * DT_MDL)
      elif output_a_target_e2e < ACCEL_BOOST_DECAY_E2E:
        self.total_boost = max(0.0, self.total_boost - ramp_down * DT_MDL)
    elif gas_pressed and self.boost_eligible:
      increase = min(ACCEL_BOOST_RATE * DT_MDL * speed_scale,
                     ACCEL_BOOST_PER_OVERRIDE - self.boost_this_override, ACCEL_BOOST_MAX - self.total_boost)
      self.total_boost += increase
      self.boost_this_override += increase

    return output_a_target_e2e + self.total_boost * boost_scale
