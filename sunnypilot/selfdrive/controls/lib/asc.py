import time
import numpy as np
# 匯入 Openpilot 原廠 MPC 相關公式與常數
# 確保我們計算安全距離與煞停距離的邏輯與原廠底層 100% 同步
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
INTENT_FRAMES_LOW   = 0      # 低速時，需連續 0 幀 (即只要當下這 1 幀偵測到加速指令，立刻 0 延遲放行)
INTENT_FRAMES_HIGH  = 20     # 高速時，需要連續 20 幀偵測到加速才放行 (防止高速巡航時的微小震盪誤判)

# --- 坡度相關參數 ---
PITCH_UPHILL_THRESHOLD = 0.050    # 判定為上坡的閾值 (5% 坡度)
PITCH_DOWNHILL_THRESHOLD = -0.030 # 判定為下坡的閾值 (-3% 坡度)
SOFT_HOLD_PITCH_START = 0.050     # 開始遞減 Soft Hold 輔助微煞車力道的坡度起點 (因為上坡自帶阻力)
SOFT_HOLD_PITCH_MAX = 0.080       # 完全取消 Soft Hold 的最大陡坡閾值 (過陡時交給原廠處理)

# --- 防震盪與動態保護參數 ---
VREL_DEBOUNCE_TIME = 0.6          # 高速差防震盪計時器 (秒)，防止前車時速突然跳動導致誤判
# 微煞車力道對照表：車速 (km/h) 對應系統幫您輕踩的加速度 (m/s^2)
SOFT_HOLD_SPEED_BP = [0.0, 10.0, 20.0, 30.0, 40.0, 50.0]
SOFT_HOLD_ACCEL_V  = [1.1,  0.90,  0.70,  0.50,  0.30,  0.10]

# --- 物理距離比例閾值 ---
# 這些數值代表「實體距離佔原廠動態安全目標距離的百分比」
RATIO_ENTER_THRESHOLD = 1.02      # 空間充裕界線：實體距離大於目標 102% 時，徹底解除滑行允許補油門
RATIO_EXIT_THRESHOLD = 0.98       # 重新進入滑行判斷區間的界線
SOFT_HOLD_RANGE_MAX = 0.98        # 進入純滑行的最高界線 (低於目標距離 98% 啟動 Soft Hold 鎖定，斷開原廠煞車)
SOFT_HOLD_RANGE_MIN = 0.70        # 交還控制權的最低界線 (低於目標距離 70% 代表太近了，交給原廠重煞)

SOFT_HOLD_HYSTERESIS_TIME = 1.0   # 狀態切換的滯後防震盪時間 (秒)，防止系統在取消/啟動之間頻繁切換
TARGET_FACTOR_FILTER_ALPHA = 0.3  # 最終輸出煞車因子的平滑濾波係數 (數值越小越平滑)

# =========================================================
# 邏輯模組：物理級距柔和滑行控制器 (獨立版)
# 核心目標：在安全的 TTC 與距離下，抹平原廠過於神經質的微弱煞車，達到柔和滑行的體感
# =========================================================
class SoftHoldLogic:
    def __init__(self):
        # 煞車控制因子平滑變數
        self._soft_hold_factor = 1.0          
        # 高速差保護狀態記錄
        self._vrel_high_start_time = 0.0      
        self._vrel_high_active = False        
        
        # 前車狀態記憶與上一幀的控制輸出
        self._last_lead_time = 0.0            
        self._last_target_factor = 1.0        
        self._last_soft_hold_accel = 0.0      

        # 動態加速意圖計數器
        self.accel_intent_counter = 0
        self.intent_accelerating = False
        
        # 自車加速度計算變數 (用於判斷是否處於負加速狀態)
        self._last_v_ego = None
        self._last_time = None
        self._a_ego_estimate = 0.0 
        
        # 狀態機防震盪變數
        self._ratio_hysteresis_state = False  
        self._target_factor_smooth = 1.0      
        self._last_stable_cancel_state = False
        self._state_change_time = 0.0
        
        # 核心狀態鎖：標記目前是否正在執行 Soft Hold (強制斷開原廠動力/煞車)
        self._is_soft_holding = False

    def process_trajectory(self, a_desired_trajectory, v_ego, lead, current_pitch, t_follow):
        """
        處理軌跡，實現柔和跟車滑行
        a_desired_trajectory: 原廠 MPC 規劃的未來加速度軌跡
        v_ego: 自車時速 (m/s)
        lead: 前車雷達資料 (通常為 leadOne)
        current_pitch: 當前車輛俯仰角 (坡度)
        t_follow: 當前選擇的駕駛性格 (Personality) 所對應的跟車秒距
        """
        # should_cancel_soft_hold 為 True 時，代表系統判定當下不安全或不適合滑行，
        # 會將控制權 100% 交還給原廠 (target_factor = 1.0)
        should_cancel_soft_hold = False
        current_time = time.monotonic()
        v_ego_kph = v_ego * 3.6  # 將底層的 m/s 轉換為 km/h
        
        # ==========================================================
        # ⏱️ 自車物理加速度推算 (判斷是否為減速狀態)
        # ==========================================================
        if self._last_time is None:
            self._last_v_ego = v_ego
            self._last_time = current_time
        else:
            dt = current_time - self._last_time
            if dt > 0.001: # 避免除以零的微小錯誤
                raw_a = (v_ego - self._last_v_ego) / dt
                # 利用指數移動平均 (EMA) 進行低通濾波，過濾速度讀數的瞬間雜訊
                self._a_ego_estimate = self._a_ego_estimate * 0.8 + raw_a * 0.2
            self._last_v_ego = v_ego
            self._last_time = current_time
        
        # 判定當下是否處於「負加速狀態」(設定 -0.1 避免平滑巡航時的微小浮動誤判)
        is_ego_decelerating = self._a_ego_estimate < -0.1

        # --- 1. 動態意圖偵測 (預判原廠是否準備加速) ---
        # 截取未來 0.6 秒的軌跡
        recent_trajectory = a_desired_trajectory[:TRAJECTORY_HORIZON]
        has_valid_lead = lead is not None and lead.status
        
        # 依據車速動態計算需要的觀察幀數 (低速 0 幀立刻反應，高速 20 幀防雜訊)
        v_ratio = max(0.0, min((v_ego - INTENT_V_LOW) / (INTENT_V_HIGH - INTENT_V_LOW), 1.0))
        dynamic_intent_frames = int(round(INTENT_FRAMES_LOW + v_ratio * (INTENT_FRAMES_HIGH - INTENT_FRAMES_LOW)))
        
        # 偵測原廠軌跡中是否包含明確的加速指令 (大於 0.05 m/s^2 的點數量大於等於 3)
        moment_accel = sum(1 for a in recent_trajectory if a > 0.05) >= INTENT_LOOKAHEAD and (lead.vRel > 0.05 if has_valid_lead else True)

        target_factor = 1.0   
        # 依據車速查表，決定系統自帶的微弱煞車力道 (若需要輔助煞車時使用)
        current_soft_hold_accel = np.interp(v_ego_kph, SOFT_HOLD_SPEED_BP, SOFT_HOLD_ACCEL_V)
        is_lead_braking_strict = False
        skip_state_2 = False

        # --- 2. 狀態機與意圖直接放行區 ---
        if not has_valid_lead:
            # 【無前車狀態】
            self._vrel_high_active = False
            self.accel_intent_counter = 0
            self.intent_accelerating = False

            # 短暫失去目標時 (雷達閃爍) 維持上一幀狀態，防止頻繁頓挫
            if (current_time - self._last_lead_time) < 0.5:
                if self._last_soft_hold_accel >= 0.0:
                    target_factor = self._last_target_factor
                    current_soft_hold_accel = self._last_soft_hold_accel
                else:
                    target_factor = 0.0
                    current_soft_hold_accel = 0.0
                should_cancel_soft_hold = False
                skip_state_2 = True 
            else:
                # 真的沒車了，取消 Soft Hold，交由巡航系統控制
                should_cancel_soft_hold = True
                skip_state_2 = True
        else:
            # 【有前車狀態】
            self._last_lead_time = current_time 
            
            # 乾淨的連續計數器：只要有加速意圖就累加，一沒有就立刻斷開歸零
            if moment_accel:
                self.accel_intent_counter += 1
            else:
                self.accel_intent_counter = 0

            # 條件達成即刻放行：累積幀數達到門檻，判定準備加速，強制取消 Soft Hold 補油門
            if moment_accel and self.accel_intent_counter >= dynamic_intent_frames:
                self.intent_accelerating = True
                should_cancel_soft_hold = True
            else:
                self.intent_accelerating = False

            # 速差過大保護 (防止高速逼近前車)
            if lead.vRel > 1.0:
                if not self._vrel_high_active:
                    self._vrel_high_active = True
                    self._vrel_high_start_time = current_time
                # 若速差持續超過 VREL_DEBOUNCE_TIME，強制交還原廠控制
                elif (current_time - self._vrel_high_start_time) > VREL_DEBOUNCE_TIME:
                    should_cancel_soft_hold = True
            else:
                self._vrel_high_active = False
                
            # 坡度過大 (陡坡) 保護，交由原廠控制避免滑動
            if current_pitch > SOFT_HOLD_PITCH_MAX:
                should_cancel_soft_hold = True

        # --- 3. 核心物理距離計算與判定區 ---
        if not skip_state_2:
            # 判斷前車是否處於嚴格煞車或靜止狀態，用以決定後續是否要疊加微弱的輔助煞車
            is_lead_stopped = (lead.vLead < 1.0) and (lead.vRel <= 0.3)
            # 根據不同車速區間，定義多嚴重的減速才算 "嚴格煞車"
            if v_ego_kph <= 10.0:
                is_lead_braking_strict = (lead.aLeadK < -0.1 or is_lead_stopped) and (lead.vRel < 0.5)
            elif v_ego_kph <= 30.0:
                is_lead_braking_strict = (lead.aLeadK < -0.5 or is_lead_stopped) and (lead.vRel < 0.5)
            elif v_ego_kph <= 40.0:
                is_lead_braking_strict = lead.aLeadK < -1.0 or is_lead_stopped
            else: 
                is_lead_braking_strict = lead.aLeadK < -1.25 or is_lead_stopped

            # 計算接近速度與 TTC (碰撞時間)
            closing_speed = max(v_ego - lead.vLead, 0.1) # 加上 0.1 防止除以零
            current_ttc = lead.dRel / closing_speed
            
            # 🚀 真實動態目標距離計算 (與 Openpilot MPC 底層物理同步)
            safe_v_lead = max(0.0, lead.vLead)
            # MPC 目標距離 = (自車基於跟車秒距的煞停距離) - (前車滑行/煞停預計距離)
            mpc_target = get_safe_obstacle_distance(v_ego, t_follow) - get_stopped_equivalence_factor(safe_v_lead)
            # 安全防線：目標距離絕對不能小於原廠設定的絕對煞停距離 (STOP_DISTANCE)
            dynamic_target_dist = max(mpc_target, STOP_DISTANCE)
            # 比例因子：當前實際距離佔安全目標距離的比例
            ratio = lead.dRel / dynamic_target_dist
            
            # 防震盪區間狀態維護 (Hysteresis)
            if not should_cancel_soft_hold:
                if ratio > RATIO_ENTER_THRESHOLD:
                    self._ratio_hysteresis_state = True
                elif ratio < RATIO_EXIT_THRESHOLD:
                    self._ratio_hysteresis_state = False
                
                # 如果處於退出狀態區間，維持取消 Soft Hold
                if self._ratio_hysteresis_state:
                    should_cancel_soft_hold = True
            
            # ==========================================================
            # 🔴 絕對棄權防線 1：距離過近 (逼近物理極限距離)
            # ==========================================================
            if lead.dRel <= STOP_DISTANCE:
                should_cancel_soft_hold = True
                self._is_soft_holding = False
                self._ratio_hysteresis_state = True

            # ==========================================================
            # 🔴 絕對棄權防線 2：車速低於 10 km/h 且 處於負加速狀態 (減速中)
            # (確保低速煞停階段精準切換，不會在低速巡航時誤觸)
            # ==========================================================
            if v_ego_kph < 10.0 and is_ego_decelerating:
                should_cancel_soft_hold = True
                self._is_soft_holding = False

        # --- 4. 狀態機防震盪計時器 (Debounce) ---
        # 防止 should_cancel_soft_hold 在 True/False 之間快速跳動造成車輛頻繁加減速
        if should_cancel_soft_hold != self._last_stable_cancel_state:
            if self._state_change_time == 0.0:
                self._state_change_time = current_time
            # 必須穩定維持新狀態達半個 HYSTERESIS_TIME 才會真正切換
            elif (current_time - self._state_change_time) > (SOFT_HOLD_HYSTERESIS_TIME / 2):
                self._last_stable_cancel_state = should_cancel_soft_hold
                self._state_change_time = 0.0
        else:
            self._state_change_time = 0.0

        # 將穩定過後的狀態覆寫回去
        should_cancel_soft_hold = self._last_stable_cancel_state

        # --- 5. 最終動力與煞車因子的計算 ---
        if should_cancel_soft_hold:
            # 狀態為取消時，因子設為 1.0 (完全聽從原廠 MPC 的軌跡)
            target_factor = 1.0  
            # 動態調整平滑濾波係數：若是準備加速，反應快一點 (0.6)；否則慢一點 (0.3)
            alpha = 0.60 if self.intent_accelerating else 0.30 
            self._is_soft_holding = False # 確保鎖定狀態解除
            
        elif not skip_state_2: 
            # 準備進入 Soft Hold 計算
            distance_factor = 1.0 
            if current_pitch <= SOFT_HOLD_PITCH_MAX:
                # 依據實體距離比例切換鎖定狀態
                if ratio <= SOFT_HOLD_RANGE_MIN:
                    self._is_soft_holding = False # 太近了，解鎖交給原廠重煞
                elif ratio >= RATIO_ENTER_THRESHOLD:
                    self._is_soft_holding = False # 空間充裕，解鎖準備加速
                    
                # ==========================================================
                # 🚀 您的專屬啟動門檻：
                # 在距離合理區間 (ratio < 0.98) 且 TTC 在 2 秒內 且 車速大於等於 15 km/h
                # 同時滿足這三個條件，才正式啟動 Soft Hold 鎖定 (開始滑行)
                # ==========================================================
                elif ratio < SOFT_HOLD_RANGE_MAX and current_ttc <= 2.0 and v_ego_kph >= 15.0:
                    self._is_soft_holding = True

                # 若處於鎖定狀態，距離因子設為 0 (準備覆寫並抹平原廠動力)
                if self._is_soft_holding:
                    distance_factor = 0.0

            # 依據相對速差計算安全因子 (防禦性設計)
            v_rel_factor = np.interp(lead.vRel, [-2.0, 0.5], [0.0, 1.0])
            # 取最保守的值作為目標因子
            target_factor = max(distance_factor, v_rel_factor)

            # 決定是否疊加模組自帶的柔和煞車力道 (Soft Hold Accel)
            # 只有在鎖定滑行狀態，且前車有明顯煞車意圖時才疊加
            if self._is_soft_holding and is_lead_braking_strict:
                if current_pitch > SOFT_HOLD_PITCH_START:
                    # 如果遇到上坡，上坡阻力大，按比例遞減系統輔助煞車力道
                    smooth_factor = float(np.interp(current_pitch, [SOFT_HOLD_PITCH_START, SOFT_HOLD_PITCH_MAX], [0.0, 1.0]))
                    target_factor = smooth_factor  
                    current_soft_hold_accel = current_soft_hold_accel * smooth_factor 
                else:
                    if is_lead_stopped:
                        # 前車靜止時，依自車速給予基礎滑行微弱煞車
                        current_soft_hold_accel = float(np.interp(v_ego_kph, [0.0, 150.0], [0.0, -0.30]))
                    elif v_ego_kph >= 50.0:
                        # 高速狀態且前車急煞時，給予較多的輔助煞車力道
                        if lead.vRel < -0.1 and lead.aLeadK <= -1.5:
                            dynamic_brake = lead.aLeadK * 0.30
                            current_soft_hold_accel = np.clip(dynamic_brake, -1.0, 0.0)
                        else:
                            current_soft_hold_accel = 0.0 
                    else:
                        current_soft_hold_accel = 0.0
                    target_factor = 0.0 

            # 收斂時的平滑係數
            alpha = 0.10 if target_factor > self._soft_hold_factor else 0.20 
        else:
            alpha = 0.10 if target_factor > self._soft_hold_factor else 0.20 

        # 記憶當前數值供下一幀使用
        self._last_target_factor = target_factor
        self._last_soft_hold_accel = current_soft_hold_accel
        
        # 進行低通濾波平滑，確保控制指令的切換滑順不頓挫
        self._soft_hold_factor = (1.0 - alpha) * self._soft_hold_factor + alpha * target_factor
        self._target_factor_smooth = (1.0 - TARGET_FACTOR_FILTER_ALPHA) * self._target_factor_smooth + TARGET_FACTOR_FILTER_ALPHA * self._soft_hold_factor

        # --- 6. 軌跡合成 (Trajectory Blending) ---
        # 將原廠 MPC 的軌跡與我們計算出的 Soft Hold 因子與微煞車進行融合
        traj = np.copy(a_desired_trajectory)
        if self._target_factor_smooth < 0.99: # 代表 Soft Hold 有介入 (因子小於 1.0)
            hold_strength = 1.0 - self._target_factor_smooth
            # 動態限制線：原廠加速力道 * 平滑因子 + 系統微煞車 * 介入強度
            dynamic_limit = np.maximum(traj, 0.0) * self._target_factor_smooth + current_soft_hold_accel * hold_strength
            blend_factor = 0.5
            
            # 若原廠的軌跡 (通常是微弱的加減速或神經質的波動) 大於我們的動態限制線，
            # 則對其進行降維抹平，實現柔和滑行。
            # (注意：如果原廠軌跡是要重踩煞車，例如 -2.0，則不會大於 dynamic_limit，這時就會保留原廠重煞)
            exceeds_mask = traj > dynamic_limit
            traj = np.where(exceeds_mask, dynamic_limit * blend_factor + traj * (1.0 - blend_factor), traj)

        # 回傳處理完畢的新軌跡給底層控制器
        return traj