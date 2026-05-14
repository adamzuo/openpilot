import time
import numpy as np
# 匯入 Openpilot 原廠 MPC 相關公式與常數
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import (
    STOP_DISTANCE, get_safe_obstacle_distance, get_stopped_equivalence_factor
)

# =========================================================
# 參數設定區 (Configuration Parameters)
# =========================================================

# --- 加速意圖偵測參數 ---
TRAJECTORY_HORIZON  = 6      # 預判原廠規劃軌跡的前 6 個點 (約涵蓋 0.6 秒的未來預測)
INTENT_LOOKAHEAD    = 3      # 在這 6 個點中，若有 3 個點符合加速閾值，即判定車輛「有意圖」加速
INTENT_V_LOW        = 0.0    # 低速基準線 (0 km/h)
INTENT_V_HIGH       = 22.22  # 高速基準線 (約 80 km/h, 單位: m/s)
INTENT_FRAMES_LOW   = 0      # 低速時，需連續 0 幀立刻放行
INTENT_FRAMES_HIGH  = 5     # 高速時，需要連續  5幀偵測到加速才放行

# --- 坡度相關參數 ---
PITCH_UPHILL_THRESHOLD = 0.050    # 判定為上坡的閾值 (5% 坡度)
PITCH_DOWNHILL_THRESHOLD = -0.030 # 判定為下坡的閾值 (-3% 坡度)
ASC_PITCH_MAX = 0.080             # 完全取消 ASC 的最大陡坡閾值 (過陡時交給原廠處理，防止滑動)

# --- 防震盪與動態保護參數 ---
VREL_DEBOUNCE_TIME = 0.5          # 高速差防震盪計時器 (秒)，防止前車時速突然跳動導致誤判

# --- 物理距離比例閾值 ---
RATIO_ENTER_THRESHOLD = 1.00      # 空間充裕界線：大於 100% 時，徹底解除滑行上限限制
RATIO_EXIT_THRESHOLD = 0.90       # 重新進入滑行判斷區間的界線
ASC_RANGE_MAX = 0.98              # 進入純滑行的最高界線 (低於目標 98% 啟動 ASC 鎖定，壓制加速)
ASC_RANGE_MIN = 0.70              # 交還控制權的最低界線 (低於 70% 解鎖，交給原廠處理)

ASC_HYSTERESIS_TIME = 1.0         # 狀態切換的滯後防震盪時間 (秒)
TARGET_FACTOR_FILTER_ALPHA = 0.3  # 最終輸出煞車因子的平滑濾波係數 (數值越小越平滑)

# =========================================================
# 邏輯模組：自適應車速控制 (ASC - Adaptive Speed Control)
# 核心目標：作為原廠 MPC 輸出的「純加速天花板」。
#         只在特定區間壓制原廠的微弱加速(強迫滑行)，而完全保留原廠所有的微弱與強烈煞車。
# =========================================================
class ASCLogic:
    def __init__(self):
        self._asc_factor = 1.0          
        self._vrel_high_start_time = 0.0      
        self._vrel_high_active = False        
        self._last_lead_time = 0.0            
        self._last_target_factor = 1.0        

        self.accel_intent_counter = 0
        self.intent_accelerating = False
        
        self._last_v_ego = None
        self._last_time = None
        self._a_ego_estimate = 0.0 
        
        self._ratio_hysteresis_state = False  
        self._target_factor_smooth = 1.0      
        self._last_stable_cancel_state = False
        self._state_change_time = 0.0
        
        self._is_asc_active = False

    def process_trajectory(self, a_desired_trajectory, v_ego, lead, current_pitch, t_follow):
        should_cancel_asc = False
        current_time = time.monotonic()
        v_ego_kph = v_ego * 3.6 
        
        # --- ⏱️ 自車物理加速度推算 (判斷是否為減速狀態) ---
        if self._last_time is None:
            self._last_v_ego = v_ego
            self._last_time = current_time
        else:
            dt = current_time - self._last_time
            if dt > 0.001:
                raw_a = (v_ego - self._last_v_ego) / dt
                self._a_ego_estimate = self._a_ego_estimate * 0.8 + raw_a * 0.2
            self._last_v_ego = v_ego
            self._last_time = current_time
        
        is_ego_decelerating = self._a_ego_estimate < -0.1

        # --- 1. 動態意圖偵測 ---
        recent_trajectory = a_desired_trajectory[:TRAJECTORY_HORIZON]
        has_valid_lead = lead is not None and lead.status
        
        v_ratio = max(0.0, min((v_ego - INTENT_V_LOW) / (INTENT_V_HIGH - INTENT_V_LOW), 1.0))
        dynamic_intent_frames = int(round(INTENT_FRAMES_LOW + v_ratio * (INTENT_FRAMES_HIGH - INTENT_FRAMES_LOW)))
        
        moment_accel = sum(1 for a in recent_trajectory if a > 0.05) >= INTENT_LOOKAHEAD and (lead.vRel > 0.05 if has_valid_lead else True)

        target_factor = 1.0   
        skip_state_2 = False

        # --- 2. 狀態機與意圖直接放行區 ---
        if not has_valid_lead:
            self._vrel_high_active = False
            self.accel_intent_counter = 0
            self.intent_accelerating = False

            if (current_time - self._last_lead_time) < 0.5:
                target_factor = self._last_target_factor
                should_cancel_asc = False
                skip_state_2 = True 
            else:
                should_cancel_asc = True
                skip_state_2 = True
        else:
            self._last_lead_time = current_time 
            
            if moment_accel:
                self.accel_intent_counter += 1
            else:
                self.accel_intent_counter = 0

            if moment_accel and self.accel_intent_counter >= dynamic_intent_frames:
                self.intent_accelerating = True
                should_cancel_asc = True
            else:
                self.intent_accelerating = False

            if lead.vRel > 0.5:
                if not self._vrel_high_active:
                    self._vrel_high_active = True
                    self._vrel_high_start_time = current_time
                elif (current_time - self._vrel_high_start_time) > VREL_DEBOUNCE_TIME:
                    should_cancel_asc = True
            else:
                self._vrel_high_active = False
                
            if current_pitch > ASC_PITCH_MAX:
                should_cancel_asc = True

        # --- 3. 核心物理距離計算與判定區 ---
        if not skip_state_2:
            closing_speed = max(v_ego - lead.vLead, 0.1)
            current_ttc = lead.dRel / closing_speed
            
            safe_v_lead = max(0.0, lead.vLead)
            mpc_target = get_safe_obstacle_distance(v_ego, t_follow) - get_stopped_equivalence_factor(safe_v_lead)
            dynamic_target_dist = max(mpc_target, STOP_DISTANCE)
            ratio = lead.dRel / dynamic_target_dist
            
            if not should_cancel_asc:
                if ratio > RATIO_ENTER_THRESHOLD:
                    self._ratio_hysteresis_state = True
                elif ratio < RATIO_EXIT_THRESHOLD:
                    self._ratio_hysteresis_state = False
                
                if self._ratio_hysteresis_state:
                    should_cancel_asc = True
            
            if lead.dRel <= STOP_DISTANCE:
                should_cancel_asc = True
                self._is_asc_active = False
                self._ratio_hysteresis_state = True

            if v_ego_kph < 10.0 and is_ego_decelerating:
                should_cancel_asc = True
                self._is_asc_active = False

        # --- 4. 狀態機防震盪計時器 (Debounce) ---
        if should_cancel_asc != self._last_stable_cancel_state:
            if self._state_change_time == 0.0:
                self._state_change_time = current_time
            elif (current_time - self._state_change_time) > (ASC_HYSTERESIS_TIME / 2):
                self._last_stable_cancel_state = should_cancel_asc
                self._state_change_time = 0.0
        else:
            self._state_change_time = 0.0

        should_cancel_asc = self._last_stable_cancel_state

        # --- 5. 最終動力因子的計算 ---
        if should_cancel_asc:
            target_factor = 1.0  
            alpha = 0.60 if self.intent_accelerating else 0.30 
            self._is_asc_active = False 
            
        elif not skip_state_2: 
            distance_factor = 1.0 
            if current_pitch <= ASC_PITCH_MAX:
                if ratio <= ASC_RANGE_MIN:
                    self._is_asc_active = False 
                elif ratio >= RATIO_ENTER_THRESHOLD:
                    self._is_asc_active = False 
                elif ratio < ASC_RANGE_MAX and current_ttc <= 2.0 and v_ego_kph >= 15.0:
                    self._is_asc_active = True

                if self._is_asc_active:
                    distance_factor = 0.0

            v_rel_factor = np.interp(lead.vRel, [-2.0, 0.5], [0.0, 1.0])
            target_factor = max(distance_factor, v_rel_factor)
            alpha = 0.10 if target_factor > self._asc_factor else 0.20 
        else:
            alpha = 0.10 if target_factor > self._asc_factor else 0.20 

        self._last_target_factor = target_factor
        
        self._asc_factor = (1.0 - alpha) * self._asc_factor + alpha * target_factor
        self._target_factor_smooth = (1.0 - TARGET_FACTOR_FILTER_ALPHA) * self._target_factor_smooth + TARGET_FACTOR_FILTER_ALPHA * self._asc_factor

        # --- 6. 軌跡合成 (Trajectory Ceiling Limits) ---
        traj = np.copy(a_desired_trajectory)
        
        if self._target_factor_smooth < 0.99:
            # 核心防禦：只抓出原廠想要「加速」的部分 (大於 0 的數值) 來進行比例壓縮
            # 若原廠想要煞車 (小於 0)，np.maximum 會將其視為 0
            dynamic_limit = np.maximum(traj, 0.0) * self._target_factor_smooth
            
            # 使用 np.minimum 完美放行所有煞車！
            # 因為原廠的煞車(負數) 永遠會小於或等於 dynamic_limit(大於等於 0)
            traj = np.minimum(traj, dynamic_limit)

        return traj
