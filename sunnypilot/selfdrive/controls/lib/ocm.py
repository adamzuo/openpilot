import time
import numpy as np
from openpilot.common.swaglog import cloudlog

# =========================================================
# OCM 參數設定區
# =========================================================
OVERTAKE_THRESHOLD = 20.0 / 3.6  # 20 km/h - 進入超車滑行的門檻
HYSTERESIS_OFFSET = 2.0 / 3.6    # 2 km/h - 保持滑行直到接近定速時才解除
TTC_THRESHOLD = 3.0              # 秒 - 前方 3 秒內有車即停用

# 緊急安全防線
EMERGENCY_TTC = 2.0
EMERGENCY_RELATIVE_SPEED = 10.0
EMERGENCY_DECEL_THRESHOLD = -1.5 # 煞車力道超過此值視為緊急狀況，立刻交還控制權

# 動態安全距離參數 (移植自新版 ACM)
SPEED_BP = [0., 10., 20., 30.]
MIN_DIST_V = [5., 10., 15., 20.]

# 坡度參數 (移植自新版 ACM)
PITCH_SMOOTH_ALPHA_UP = 0.30           
# ++ 修改：提高下坡反應靈敏度，從 0.05 提升至 0.15 ++
PITCH_SMOOTH_ALPHA_DOWN = 0.15
PITCH_DOWNHILL_THRESHOLD = -0.030      # 判定為下坡的閾值 (-3% 坡度)


class OCM:
  def __init__(self):
    self.enabled = False
    self.active = False
    self.just_disabled = False
    
    self._is_speed_over_cruise = False
    self._has_lead = False
    self._active_prev = False
    self._last_lead_time = 0.0
    
    # 坡度記憶
    self.current_pitch = 0.0              
    self._is_first_pitch = True           

  def _update_pitch(self, orientation_ned):
    """更新並平滑化道路坡度資訊"""
    if len(orientation_ned) == 3:
      new_pitch = orientation_ned[1]
      if self._is_first_pitch:
          self.current_pitch = new_pitch
          self._is_first_pitch = False
      else:
          alpha = PITCH_SMOOTH_ALPHA_UP if new_pitch > self.current_pitch else PITCH_SMOOTH_ALPHA_DOWN
          self.current_pitch = alpha * new_pitch + (1.0 - alpha) * self.current_pitch

  def _check_emergency_conditions(self, lead, v_ego, current_time):
    """緊急狀況判斷 (結合動態安全距離)"""
    if not lead or not lead.status:
      return False
      
    closing_speed = max(v_ego - lead.vLead, 0.1)
    self.lead_ttc = lead.dRel / closing_speed
    relative_speed = v_ego - lead.vLead
    
    # 動態最小安全距離
    min_dist_for_speed = np.interp(v_ego, SPEED_BP, MIN_DIST_V)

    # 如果極度危險，或在最小距離內且速差大於 0，立刻強制解除
    if (self.lead_ttc < EMERGENCY_TTC) or \
       (relative_speed > EMERGENCY_RELATIVE_SPEED) or \
       (lead.dRel < min_dist_for_speed and relative_speed > 0):
      self._last_lead_time = current_time
      if self.active:
        cloudlog.warning(f"OCM emergency disable: dRel={lead.dRel:.1f}m, TTC={self.lead_ttc:.1f}s")
      return True
      
    return False

  def _update_lead_status(self, lead, v_ego, current_time):
    if lead and lead.status:
      closing_speed = max(v_ego - lead.vLead, 0.1)
      self.lead_ttc = lead.dRel / closing_speed
      if self.lead_ttc < TTC_THRESHOLD:
        self._has_lead = True
        self._last_lead_time = current_time
      else:
        self._has_lead = False
    else:
      self._has_lead = False

  def _should_activate(self, user_ctrl_lon, v_ego, v_cruise, in_cooldown):
    # 坡度安全防護：如果是明顯下坡 (-3%以上)，重力會讓車速飆升，不允許純滑行
    if self.current_pitch < PITCH_DOWNHILL_THRESHOLD:
        return False

    # 狀態鎖定邏輯 (確保滑行不中斷)
    if self.active:
      self._is_speed_over_cruise = v_ego > (v_cruise + HYSTERESIS_OFFSET)
    else:
      self._is_speed_over_cruise = v_ego >= (v_cruise + OVERTAKE_THRESHOLD)

    return (not user_ctrl_lon and not self._has_lead and 
            not in_cooldown and self._is_speed_over_cruise)

  def update_states(self, cc, rs, user_ctrl_lon, v_ego, v_cruise, sccv_active):
    if not self.enabled:
      self.active = False
      return
      
    # =========================================================
    # ++ 新增：如果 SCC-V 正在過彎主動介入，強制關閉 OCM 停止滑行 ++
    # =========================================================
    if sccv_active:
      self.active = False
      self._active_prev = False
      return

    # 每次更新先讀取坡度
    self._update_pitch(cc.orientationNED)
      
    current_time = time.monotonic()
    lead = rs.leadOne

    if self._check_emergency_conditions(lead, v_ego, current_time):
      self.active = False
      self._active_prev = False
      return

    self._update_lead_status(lead, v_ego, current_time)
    in_cooldown = (current_time - self._last_lead_time) < 0.5
    
    self.active = self._should_activate(user_ctrl_lon, v_ego, v_cruise, in_cooldown)

    self.just_disabled = self._active_prev and not self.active
    self._active_prev = self.active

  def update_a_desired_trajectory(self, a_desired_trajectory):
    if not self.active:
      return a_desired_trajectory

    # 安全檢查：若 MPC 模型判定需要急煞，不予攔截
    min_accel = np.min(a_desired_trajectory)
    if min_accel < EMERGENCY_DECEL_THRESHOLD:
      self.active = False
      return a_desired_trajectory

    # 取消滑行下限，實現平順滑行
    modified = np.copy(a_desired_trajectory)
    for i in range(len(modified)):
      # 保留 -1.0 的彎道/微煞車安全底線，確保過彎穩定性
      if -1.0 < modified[i] < 0:
        modified[i] = 0.0
        
    return modified
