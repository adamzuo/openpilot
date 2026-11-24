"""
C3XL 红绿灯/停车线辅助（移植自 herizon1054/openpilot 的 dplcc 分支
`dragonpilot/selfdrive/controls/lib/traffic_stop.py`，其上游是 carrot (cp) fork）

数据源：modelV2 的【预测轨迹终点】
  - 不需要任何红绿灯 CAN 报文 ✓（丰田可用 ✓）
  - 不需要特斯拉车机 ✗

判定（与上游一致）：
  stop_sign = (
      model_x_end < d_rel - 3.0          # 轨迹终点比前车更近 → 前方有"看不见的障碍"
      且 model_x_end < max_detect_dist
      且 (model_v < 3.0 或 model_v < 0.7*v0)   # 模型预测会减速到停
      且 |model_y_end| < 5.0 m            # 在本车道内
  )
  连续满足 → RED

坡顶防护（上游经验，必须有）：
  上坡接近坡顶时模型看不到坡顶后方 → 预测轨迹缩短、终点速度趋 0
  → 会被误判成红灯 → 必须用 ahead_grade 抑制

本版本（阶段 1）：
  · 只输出 output_v_target（作为巡航上限）
  · 不改 acados / 不需要编译
  · 开关用文件：/data/c3xl_traffic.json  {"enabled": true, "distance_adjust_m": 0.0}
"""

from collections import deque

import numpy as np

from openpilot.common.realtime import DT_MDL

# ---------------- 状态 ----------------
CRUISE, STOPPING, STOPPED = 0, 1, 2
OFF, RED, GREEN = 0, 1, 2

# ---------------- 开关（文件，不改 params_keys.h）----------------
SWITCH_FILE = "/data/c3xl_traffic.json"

# ---------------- 常量（沿用上游 dplcc / carrot）----------------
V_TARGET_SENTINEL = 1e6

CREST_GRADE_THRESHOLD = 0.015     # 前方坡度 > 1.5% 视为上坡
CREST_HOLD_S = 4.0                # 上坡信号维持秒数（坡顶时坡度会掉到 0，需延续）
CREST_MIN_SPEED_KPH = 30.0        # 上坡期间且车速 > 30：不进入 STOPPING
UPHILL_CONFIRM_S = 0.5            # 上坡期间且车速 <= 30：红灯需连续 0.5 秒
CREST_HOLD_FRAMES = round(CREST_HOLD_S / DT_MDL)
UPHILL_CONFIRM_FRAMES = round(UPHILL_CONFIRM_S / DT_MDL)

TRAFFIC_STOP_ENTRY_STEERING_LIMIT_DEG = 50.0
TRAFFIC_STOP_DISTANCE_RATIO_SPEED_BP_KPH = (0.0, 100.0)
TRAFFIC_STOP_DISTANCE_RATIO = (1.0, 0.7)
TRAFFIC_STOP_DISTANCE_FADE_BP_M = (0.0, 50.0)

STOPPED_SPEED_MS = 0.3
STOPPED_GRACE_FRAMES = round(0.5 / DT_MDL)
GREEN_CONFIRM_FRAMES = round(0.2 / DT_MDL)
GAS_SUPPRESS_FRAMES = round(10.0 / DT_MDL)
RECALIBRATE_MIN_DISTANCE_M = 10.0
CANCEL_LEAD_MARGIN_M = 4.0
MEDIAN_WINDOW = 3
MOVING_AVG_WINDOW = 15
MODEL_V_WINDOW = 10
STOP_SIGN_MAX_SPEED_KPH = 82.0
STOP_SIGN_DETECT_DIST_BP_KPH = (60.0, 80.0)
STOP_SIGN_DETECT_DIST_M = (120.0, 150.0)
STOP_SIGN_LATERAL_TOLERANCE_M = 5.0
CAMERA_TO_FRONT_M = -1.5

PARAM_POLL_FRAMES = round(1.0 / DT_MDL)


# ---------------- 纯函数（与上游逐行一致）----------------
def is_traffic_stop_entry_allowed(steering_angle_deg: float) -> bool:
  """大转角 => 在转弯，不是在接近停止线。只阻止【新的】进入。"""
  return abs(steering_angle_deg) < TRAFFIC_STOP_ENTRY_STEERING_LIMIT_DEG


def get_traffic_stop_reference_speed(v_ego_kph: float, previous_reference_kph: float | None) -> float:
  """锁存本次停车过程中的最高车速（单调不减）"""
  return max(0.0, v_ego_kph, previous_reference_kph or 0.0)


def get_virtual_traffic_stop_distance(model_distance: float, v_ego_kph: float) -> float:
  """速度越快越早开始刹（100% @0kph，70% @100kph），但最后 50m 内比值回到 100%"""
  distance_ratio = np.interp(v_ego_kph, TRAFFIC_STOP_DISTANCE_RATIO_SPEED_BP_KPH, TRAFFIC_STOP_DISTANCE_RATIO)
  applied_ratio = np.interp(model_distance, TRAFFIC_STOP_DISTANCE_FADE_BP_M, [1.0, distance_ratio])
  return max(0.0, model_distance * applied_ratio)


def get_traffic_stop_obstacle_distance(stop_distance: float, distance_adjust: float) -> float:
  """应用（固定物理 + 用户）距离修正，只调用一次"""
  return max(0.0, stop_distance + distance_adjust)


def check_model_stopping(hist: deque, stop_sign_count: int, start_sign_count: int, state: int,
                         v_cruise: float, model_v_traj, v_ego: float, a_ego: float,
                         model_x_end: float, model_y_end: float, d_rel: float) -> tuple[int, int, int]:
  """返回 (traffic_state, new_stop_sign_count, new_start_sign_count)"""
  v_ego_kph = v_ego * 3.6

  hist.append(model_v_traj[-1])
  model_v = sum(hist) / len(hist)

  start_sign = model_v > 5.0 or model_v > (model_v_traj[0] + 2)

  if v_ego_kph < 1.0:
    stop_sign = model_x_end < 20.0 and model_v < 10.0
  elif v_ego_kph < STOP_SIGN_MAX_SPEED_KPH:
    max_detect_dist = np.interp(model_v_traj[0] * 3.6, STOP_SIGN_DETECT_DIST_BP_KPH, STOP_SIGN_DETECT_DIST_M)
    stop_sign = (
      model_x_end < d_rel - 3.0 and
      model_x_end < max_detect_dist and
      (model_v < 3.0 or model_v < model_v_traj[0] * 0.7) and
      abs(model_y_end) < STOP_SIGN_LATERAL_TOLERANCE_M
    )
    if v_cruise != 0 and state == CRUISE and a_ego < -1.0:
      stop_sign = False
  else:
    stop_sign = False

  stop_sign_count = stop_sign_count + 1 if stop_sign else 0
  start_sign_count = start_sign_count + 1 if (start_sign and not stop_sign) else 0

  if stop_sign_count * DT_MDL > 0.0:
    return RED, stop_sign_count, start_sign_count
  if start_sign_count * DT_MDL > 0.2:
    return GREEN, stop_sign_count, start_sign_count
  return OFF, stop_sign_count, start_sign_count


# ---------------- 控制器 ----------------
class C3XLTrafficStop:
  """从文件开关读取启用状态；阶段 1 只输出 v_target"""

  def __init__(self):
    self.is_enabled = False
    self.distance_adjust_m = 0.0
    self._poll_frame = 0
    self._load_switch()

    self.state = CRUISE
    self.stop_sign_count = 0
    self.start_sign_count = 0
    self.model_v_hist = deque(maxlen=MODEL_V_WINDOW)

    self.stop_dist_m = None
    self.near_stop_dist_m = None
    self.stop_model_x_raw = 0.0
    self.stop_model_x_rl = 0.0
    self.reference_speed_kph = 0.0
    self.actual_stop_distance = 0.0
    self.gas_suppress_frames = 0
    self.stopped_grace_frames = 0

    self.uphill_hold_frames = 0
    self.ahead_grade = 0.0
    self.output_v_target = V_TARGET_SENTINEL
    self.output_a_target = 0.0
    self.output_should_stop = False

  # ---------- 文件开关 ----------
  def _load_switch(self):
    import json
    try:
      with open(SWITCH_FILE) as f:
        d = json.load(f)
      self.is_enabled = bool(d.get("enabled", False))
      self.distance_adjust_m = float(np.clip(float(d.get("distance_adjust_m", 0.0)), -5.0, 5.0))
    except Exception:
      self.is_enabled = False
      self.distance_adjust_m = 0.0

  def _poll_params(self):
    self._poll_frame += 1
    if self._poll_frame >= PARAM_POLL_FRAMES:
      self._poll_frame = 0
      self._load_switch()

  # ---------- 内部 ----------
  def _reset(self):
    self.state = CRUISE
    self.stop_sign_count = 0
    self.start_sign_count = 0
    self.actual_stop_distance = 0.0
    self.reference_speed_kph = 0.0
    self.stopped_grace_frames = 0
    self.gas_suppress_frames = 0
    self.stop_dist_m = None
    self.near_stop_dist_m = None
    self.output_v_target = V_TARGET_SENTINEL
    self.output_a_target = 0.0
    self.output_should_stop = False
    self.model_v_hist.clear()

  def _update_stop_model_x(self, raw_x: float, v_ego: float) -> tuple[float, float]:
    """中位数 + 移动平均平滑"""
    self._xbuf.append(raw_x)
    med = float(np.median(list(self._xbuf)))
    self._xma.append(med)
    ma = float(np.mean(list(self._xma)))
    return med, ma

  # ---------- 主入口 ----------
  def update(self, model_v2, car_state, radar_state, v_ego: float, a_ego: float, v_cruise: float):
    self._poll_params()

    if not hasattr(self, "_xbuf"):
      self._xbuf = deque(maxlen=MEDIAN_WINDOW)
      self._xma = deque(maxlen=MOVING_AVG_WINDOW)

    if not self.is_enabled:
      self._reset()
      return

    model_x_traj = list(model_v2.position.x)
    model_y_traj = list(model_v2.position.y)
    model_v_traj = list(model_v2.velocity.x)
    if len(model_x_traj) < 2 or len(model_v_traj) == 0 or len(model_y_traj) == 0:
      self.stop_dist_m = None
      self.near_stop_dist_m = None
      self.output_v_target = V_TARGET_SENTINEL
      self.output_a_target = 0.0
      self.output_should_stop = False
      return
    model_x_end = model_x_traj[-1]

    # 坡顶防护：前方坡度 = 模型预测高度差 / 预测距离
    model_z_traj = list(getattr(model_v2.position, "z", ()))
    if len(model_z_traj) == len(model_x_traj):
      self.ahead_grade = (model_z_traj[-1] - model_z_traj[0]) / max(model_x_end, 1.0)
    else:
      self.ahead_grade = 0.0
    if self.ahead_grade > CREST_GRADE_THRESHOLD:
      self.uphill_hold_frames = CREST_HOLD_FRAMES
    elif self.uphill_hold_frames > 0:
      self.uphill_hold_frames -= 1

    lead = radar_state.leadOne
    lead_present = bool(lead.present)
    d_rel = lead.dRel if lead_present else 1000.0

    steering_angle_deg = car_state.steeringAngleDeg
    gas_pressed = car_state.gasPressed
    left_blinker = car_state.leftBlinker

    self.stop_model_x_raw, self.stop_model_x_rl = self._update_stop_model_x(model_x_traj[-2], v_ego)

    traffic_state, self.stop_sign_count, self.start_sign_count = check_model_stopping(
      self.model_v_hist, self.stop_sign_count, self.start_sign_count, self.state,
      v_cruise, model_v_traj, v_ego, a_ego, model_x_end, model_y_traj[-1], d_rel)

    if gas_pressed and self.state == STOPPING:
      self.gas_suppress_frames = GAS_SUPPRESS_FRAMES
    elif self.gas_suppress_frames > 0:
      self.gas_suppress_frames -= 1

    lead_cancels = lead_present and (d_rel - self.stop_model_x_raw) < CANCEL_LEAD_MARGIN_M

    if self.state == CRUISE:
      entry_allowed = is_traffic_stop_entry_allowed(steering_angle_deg)
      uphill = self.uphill_hold_frames > 0
      crest_ok = not uphill or (v_ego * 3.6 <= CREST_MIN_SPEED_KPH
                                and self.stop_sign_count >= UPHILL_CONFIRM_FRAMES)
      if (not lead_present and traffic_state == RED
            and entry_allowed and self.gas_suppress_frames == 0 and crest_ok):
        self.state = STOPPING
        self.reference_speed_kph = get_traffic_stop_reference_speed(v_ego * 3.6, None)
        self.actual_stop_distance = get_virtual_traffic_stop_distance(
          self.stop_model_x_rl, self.reference_speed_kph)

    elif self.state == STOPPING:
      if gas_pressed:
        self.state = CRUISE
      elif lead_cancels:
        self.state = CRUISE
      elif traffic_state == GREEN:
        self.state = CRUISE
      else:
        self.reference_speed_kph = get_traffic_stop_reference_speed(v_ego * 3.6, self.reference_speed_kph)
        candidate = get_virtual_traffic_stop_distance(self.stop_model_x_rl, self.reference_speed_kph)
        if candidate > RECALIBRATE_MIN_DISTANCE_M:
          self.actual_stop_distance = candidate
        if v_ego < STOPPED_SPEED_MS:
          self.state = STOPPED
          self.stopped_grace_frames = STOPPED_GRACE_FRAMES

    elif self.state == STOPPED:
      if gas_pressed:
        self.state = CRUISE
      elif lead_cancels:
        self.state = CRUISE
      else:
        if self.stopped_grace_frames == 0:
          if traffic_state == GREEN and not left_blinker:
            self.state = CRUISE
        self.stopped_grace_frames = max(0, self.stopped_grace_frames - 1)

    # 离开 STOPPING/STOPPED → 清空
    if self.state == CRUISE:
      self.actual_stop_distance = 0.0
      self.reference_speed_kph = 0.0
      self.stopped_grace_frames = 0
      self.stop_dist_m = None
      self.near_stop_dist_m = None
      self.output_v_target = V_TARGET_SENTINEL
      self.output_a_target = 0.0
      self.output_should_stop = False
      return

    # 用本车速度对停止线做航位推算（停止线固定在地面上）
    self.actual_stop_distance = max(0.0, self.actual_stop_distance - v_ego * DT_MDL)

    if traffic_state in (OFF, GREEN):
      self.actual_stop_distance = 0.0
      self.output_v_target = V_TARGET_SENTINEL
      self.output_a_target = 0.0
      self.output_should_stop = v_ego < STOPPED_SPEED_MS
      if self.near_stop_dist_m is not None:
        self.stop_dist_m = None
      return

    # RED：
    stop_distance = get_traffic_stop_obstacle_distance(self.actual_stop_distance, self.distance_adjust_m)
    self.near_stop_dist_m = stop_distance
    if stop_distance < 0.5:
      stop_distance = 0.0

    self.stop_dist_m = stop_distance
    # 阶段 1：只输出速度上限（越接近停止线，上限越低）
    if stop_distance < 0.3:
      self.output_v_target = 0.0
      self.output_should_stop = True
    else:
      self.output_v_target = min(v_cruise if v_cruise > 0 else 30.0, max(0.0, stop_distance / 2.0))
      self.output_should_stop = False
    self.output_a_target = 0.0
