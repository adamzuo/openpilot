"""
Copyright (c) 2025, Rick Lan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, and/or sublicense,
for non-commercial purposes only, subject to the following conditions:

- The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
- Commercial use (e.g. use in a product, service, or activity intended to
  generate revenue) is prohibited without explicit written permission from
  the copyright holder.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
"""

# Adaptive Experimental Mode (AEM) picks the longitudinal mode from the model's
# throttle intent (the predicted gas-press probability). experimentalMode sets the
# default; the throttle intent only ever diverts away from it:
#   * blended-first (experimental on):  use ACC when the model clearly wants throttle.
#   * acc-first    (experimental off):  use BLENDED when the model clearly eases off.
# The asymmetric thresholds leave a 0.4-0.6 deadband where each mode holds its default.
THROTTLE_ACC_PROB     = 0.6  # gasPressProb >= this -> model wants throttle -> ACC
THROTTLE_BLENDED_PROB = 0.4  # gasPressProb <= this -> model easing off     -> BLENDED


# ==============================================================================
# dp: AEM 混合模式（ACC 優先，e2e 只在明確要多煞時介入）
# ==============================================================================
# 平常 → 完全採用 MPC 求解值（與 ACC 模式同一個解：車距控制、accel_controller 加速上限、
#         提早滑行、巡航死區、滑行阻力全部保留）
# e2e 明確要比 MPC 煞得更重 → min(MPC, e2e)
# 「明確」的判斷（遲滯）：
#   進入：e2e < min(E2E_BRAKE_ON, MPC + E2E_BRAKE_ON_REL)   即 e2e 低於 -0.10，且比 MPC 至少多煞 0.10
#   解除：e2e > min(E2E_BRAKE_OFF, MPC + E2E_BRAKE_OFF_REL) 即 e2e 回到 -0.05 以上，或與 MPC 差距小於 0.05
#   相對門檻是為了尊重 accel_controller 的滑行：MPC 在滑行（例如 -0.12）時，e2e 只比它多煞一點點
#   （例如 -0.19）不介入。14 段 log 中，滑行被 e2e 改成煞車的時間由 15.7 秒降到 6.6 秒。
# log 依據（14 段 rlog，約 4.7 分鐘縱向接管）：
#   - MPC 想加速的 49 秒中，原廠 blended 壓住 27.9 秒；本規則壓住 2.5 秒
#   - 032 藍車切入：e2e 在 56.65s 觸發（雷達切入預測 57.60s、駕駛煞車 57.90s）；
#     前提是略過油門意圖閘門（BYPASS_THROTTLE_GATE），否則當時會被判定為 ACC
# 刻意偏離原廠：原廠 blended 為 min(MPC, e2e)，加速上限為 ACCEL_MAX、無彎道限制、不套 accel_controller 上限。
# AEM 關閉時（純實驗模式）維持原廠 blended，不受影響；低速時也不使用混合模式（見 HYBRID_SPEED_*）。
MODE_HYBRID = 'aem_hybrid'

# 是否略過油門意圖閘門（上方 0.6 / 0.4 門檻），AEM 開啟即一律走混合模式（不論 experimentalMode）。
#   032 藍車切入：gasPressProbs[1] 在 56.4~58.1s 大多維持 0.65~0.91（僅 57.50s 單幀 0.59），閘門判定為 ACC，
#   e2e 的煞車意圖完全被擋掉（實驗模式開要到 58.15s、關要到 58.40s 才會進混合模式，晚於駕駛煞車 57.90s）。
#   False：維持原本閘門，判定為 blended 時才換成混合模式。
BYPASS_THROTTLE_GATE = True

# 混合模式的車速範圍（與低速強制實驗模式的 20 / 30 km/h 遲滯帶對齊）：
#   車速 >= HYBRID_SPEED_ON_KPH  → 走混合模式（依 BYPASS_THROTTLE_GATE）
#   車速 <= HYBRID_SPEED_OFF_KPH → 回到 AEM 原本的 get_mode()，混合模式完全不介入
#   兩者之間維持前一狀態；啟動時從低速側開始。
# 原因：低速強制實驗模式是讓 e2e 主導停車與起步（紅燈、走走停停）。混合模式平常照 MPC，
#   若在低速啟用，前方沒有雷達前車時 MPC 會想加速到設定車速，e2e 的「不起步」就會被蓋掉。
HYBRID_SPEED_ON_KPH = 30.0
HYBRID_SPEED_OFF_KPH = 20.0

E2E_BRAKE_ON = -0.10       # e2e 絕對門檻：低於此值才可能介入
E2E_BRAKE_ON_REL = -0.10   # e2e 相對門檻：至少比 MPC 多煞 0.10 才介入
E2E_BRAKE_OFF = -0.05      # 解除：e2e 高於此值
E2E_BRAKE_OFF_REL = -0.05  # 解除：或與 MPC 的差距小於 0.05


# A: Immediate braking entry; delayed release and upward-only recovery limiting.
E2E_MIN_HOLD_S = 1.0
E2E_RELEASE_CONFIRM_S = 0.5
E2E_RECOVERY_JERK = 1.0  # m/s^3; never limits a request for stronger braking


class AEM:
  def __init__(self, dt=0.05):
    self.dt = dt
    self._throttle_prob = 1.0
    self.reset_hybrid()
    self._e2e_brake = False  # dp: 混合模式中 e2e 是否處於「明確要煞車」狀態
    self._v_ego = 0.0
    self._hybrid_speed_ok = False  # dp: 車速是否在混合模式範圍內（遲滯）

  def update_states(self, model_msg, radar_msg, v_ego):
    # Probability the model wants to be on throttle (same signal the planner uses
    # for allow_throttle). High -> accelerate/cruise; low -> coast/slow.
    probs = model_msg.meta.disengagePredictions.gasPressProbs
    self._throttle_prob = probs[1] if len(probs) > 1 else 1.0
    # dp: 混合模式的車速遲滯
    self._v_ego = v_ego
    v_kph = v_ego * 3.6
    if v_kph >= HYBRID_SPEED_ON_KPH:
      self._hybrid_speed_ok = True
    elif v_kph <= HYBRID_SPEED_OFF_KPH:
      self._hybrid_speed_ok = False

  def get_mode(self, mode):
    if mode == 'blended':  # blended-first: borrow ACC only when clearly wanting throttle
      return 'acc' if self._throttle_prob >= THROTTLE_ACC_PROB else 'blended'
    # acc-first: borrow BLENDED only when clearly easing off
    return 'blended' if self._throttle_prob <= THROTTLE_BLENDED_PROB else 'acc'

  # dp: 規劃器用的模式選擇。get_mode() 保持原樣（test_aem.py 依賴其行為），這裡再套用混合模式。
  #   低速（見 HYBRID_SPEED_*）：完全使用原本的 get_mode()，不換成混合模式。
  def get_planner_mode(self, mode):
    if not self._hybrid_speed_ok:
      return self.get_mode(mode)
    if BYPASS_THROTTLE_GATE:
      return MODE_HYBRID
    mode = self.get_mode(mode)
    return MODE_HYBRID if mode == 'blended' else mode

  # dp: 混合模式的輸出組合。回傳 (output_a_target, e2e 是否介入)
  def get_hybrid_accel(self, a_mpc, a_e2e):
    brake_on = min(E2E_BRAKE_ON, a_mpc + E2E_BRAKE_ON_REL)
    brake_off = min(E2E_BRAKE_OFF, a_mpc + E2E_BRAKE_OFF_REL)
    if not self._e2e_brake and a_e2e < brake_on:
      self._e2e_brake = True
      self._brake_elapsed = 0.0
      self._release_elapsed = 0.0

    if self._e2e_brake:
      self._brake_elapsed += self.dt
      self._release_elapsed = self._release_elapsed + self.dt if a_e2e > brake_off else 0.0
      if (self._brake_elapsed + 1e-9 >= E2E_MIN_HOLD_S and
          self._release_elapsed + 1e-9 >= E2E_RELEASE_CONFIRM_S):
        self._e2e_brake = False
      self._recovering = True

    a = min(a_mpc, a_e2e) if self._e2e_brake else a_mpc
    # Also smooth e2e recovery while still latched, not only the release frame.
    # min preserves immediate MPC/DTSC/e2e requests for stronger deceleration.
    if self._recovering and self._last_output is not None:
      a = min(a, self._last_output + E2E_RECOVERY_JERK * self.dt)
    if not self._e2e_brake and a >= a_mpc:
      self._recovering = False
    self._last_output = a
    return a, a < a_mpc

  def observe_output(self, acceleration):
    # Use the actual planner output after final clipping as the next reference.
    self._last_output = float(acceleration)

  def reset_hybrid(self):
    self._e2e_brake = False
    self._brake_elapsed = 0.0
    self._release_elapsed = 0.0
    self._recovering = False
    self._last_output = None
