#!/usr/bin/env python3
import numpy as np

import openpilot.cereal.messaging as messaging
from opendbc.car.interfaces import ACCEL_MIN, ACCEL_MAX
from openpilot.common.constants import CV
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.selfdrive.controls.lib.accel_boost import AccelBoost
from openpilot.selfdrive.controls.lib.longcontrol import LongCtrlState
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import LongitudinalMpc, LongitudinalPlanSource, \
  get_safe_obstacle_distance, get_T_FOLLOW
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import T_IDXS as T_IDXS_MPC
from openpilot.selfdrive.controls.lib.drive_helpers import CONTROL_N, get_accel_from_plan, should_stop
from openpilot.selfdrive.car.cruise import V_CRUISE_MAX, V_CRUISE_UNSET
from openpilot.common.swaglog import cloudlog

from openpilot.sunnypilot.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlannerSP

J_CRUISE_VALS = [1.6, 1.2, 0.8, 0.6]
J_CRUISE_BP = [0., 10.0, 25., 40.]
A_CRUISE_MIN = -0.5
A_CRUISE_T = 2 # target acceleration time horizon
CONTROL_N_T_IDX = ModelConstants.T_IDXS[:CONTROL_N]



# C3XL: 超速（松油门后速度高于定速）处理模式
#   0 = 原厂：立刻以 -0.5 m/s² 拉回定速（顿挫明显）
#   1 = 滑行优先：超速 5 km/h 内完全滑行；再多则温和减速（上限 -0.25）
#   2 = 极温和：同上但上限 -0.15
_OVERRUN_CACHE = {"t": 0.0, "mode": 1}


def _overrun_mode() -> int:
  import time
  now = time.monotonic()
  if now - _OVERRUN_CACHE["t"] > 1.0:
    _OVERRUN_CACHE["t"] = now
    try:
      from openpilot.common.params import Params
      v = Params().get("C3XLOverrun", return_default=True)
      _OVERRUN_CACHE["mode"] = int(v) if v not in (None, "") else 1
    except Exception:
      _OVERRUN_CACHE["mode"] = 1
  m = _OVERRUN_CACHE["mode"]
  return m if m in (0, 1, 2) else 1

def get_cruise_accel(v_cruise, v_ego, a_cruise_prev, dt):
  mode = _overrun_mode()
  over_kmh = (v_ego - v_cruise) * 3.6
  if mode == 0:
    target_accel = np.clip((v_cruise - v_ego) / A_CRUISE_T, A_CRUISE_MIN, ACCEL_MAX)
    jerk_scale = 1.0
  else:
    # C3XL: 超速时滑行优先，避免松油门瞬间的刹车顿挫
    cap = 0.25 if mode == 1 else 0.15
    if over_kmh <= 0.0:
      # 低于/等于定速：正常加速回到定速（不能是 0，否则定速巡航失效）
      target_accel = np.clip((v_cruise - v_ego) / A_CRUISE_T, A_CRUISE_MIN, ACCEL_MAX)
      jerk_scale = 1.0
    elif over_kmh <= 5.0:
      target_accel = 0.0                      # 轻微超速：完全滑行（不主动刹车）
      jerk_scale = 0.4 if mode == 1 else 0.25
    else:
      target_accel = -min(cap, 0.03 * (over_kmh - 5.0))
      jerk_scale = 0.4 if mode == 1 else 0.25
  j_cruise = np.interp(v_ego, J_CRUISE_BP, J_CRUISE_VALS) * jerk_scale
  target_accel = float(np.clip(target_accel, a_cruise_prev - j_cruise * dt, a_cruise_prev + j_cruise * dt))

  return target_accel


class LongitudinalPlanner(LongitudinalPlannerSP):
  def __init__(self, CP, CP_SP, init_v=0.0, init_a=0.0, dt=DT_MDL):
    self.CP = CP
    self.mpc = LongitudinalMpc(dt=dt)
    LongitudinalPlannerSP.__init__(self, self.CP, CP_SP, self.mpc)
    self.fcw = False
    self.dt = dt
    self.accel_boost = AccelBoost()

    self.v_desired_filter = FirstOrderFilter(init_v, 2.0, self.dt)
    self.a_cruise = init_a
    self.output_a_target = init_a
    self.output_should_stop = False

    self.v_desired_trajectory = np.zeros(CONTROL_N)
    self.a_desired_trajectory = np.zeros(CONTROL_N)
    self.j_desired_trajectory = np.zeros(CONTROL_N)

  def update(self, sm):
    LongitudinalPlannerSP.update(self, sm)

    v_ego = sm['carState'].vEgo
    v_cruise_kph = min(sm['carState'].vCruise, V_CRUISE_MAX)
    v_cruise = v_cruise_kph * CV.KPH_TO_MS
    if sm['controlsState'].forceDecel:
      v_cruise = 0.0

    long_control_off = sm['controlsState'].longControlState == LongCtrlState.off

    # Reset current state when not engaged, or user is controlling the speed
    reset_state = long_control_off if self.CP.openpilotLongitudinalControl else not sm['selfdriveState'].enabled
    # PCM cruise speed may be updated a few cycles later, check if initialized
    v_cruise_initialized = sm['carState'].vCruise != V_CRUISE_UNSET
    reset_state = reset_state or not v_cruise_initialized

    if reset_state:
      self.v_desired_filter.x = v_ego
      self.output_a_target = np.clip(sm['carState'].aEgo, ACCEL_MIN, ACCEL_MAX)
      self.a_cruise = self.output_a_target

    # Prevent divergence, smooth in current v_ego
    self.v_desired_filter.x = max(0.0, self.v_desired_filter.update(v_ego))

    # No change cost when user is controlling the speed, or when standstill
    prev_accel_constraint = not (reset_state or sm['carState'].standstill)

    # Get new v_cruise and a_target from Smart Cruise Control and Speed Limit Assist
    v_cruise, self.output_a_target = LongitudinalPlannerSP.update_targets(self, sm, self.v_desired_filter.x, self.output_a_target, v_cruise)

    self.mpc.set_weights(prev_accel_constraint, personality=sm['selfdriveState'].personality)
    if self.CP.openpilotLongitudinalControl:
      self.mpc.set_cur_state(self.v_desired_filter.x, self.output_a_target)
    else:
      self.mpc.set_cur_state(v_ego, sm['carState'].aEgo)
    self.mpc.update(sm['radarState'], personality=sm['selfdriveState'].personality)

    self.v_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC, self.mpc.v_solution)
    self.a_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC, self.mpc.a_solution)
    self.j_desired_trajectory = np.interp(CONTROL_N_T_IDX, T_IDXS_MPC[:-1], self.mpc.j_solution)

    # TODO counter is only needed because radar is glitchy, remove once radar is gone
    self.fcw = self.mpc.crash_cnt > 2 and not sm['carState'].standstill
    if self.fcw:
      cloudlog.info("FCW triggered")

    # Save starting point for next iteration
    a_prev = self.output_a_target

    action_t =  self.CP.longitudinalActuatorDelay + DT_MDL
    output_a_target_mpc = get_accel_from_plan(self.v_desired_trajectory, self.a_desired_trajectory, CONTROL_N_T_IDX,
                                              action_t=action_t)
    output_should_stop_mpc = should_stop(v_ego, output_a_target_mpc)
    output_a_target_e2e = sm['modelV2'].action.desiredAcceleration
    output_should_stop_e2e = sm['modelV2'].action.shouldStop

    self.a_cruise = get_cruise_accel(v_cruise, v_ego, self.a_cruise, self.dt)
    cruise_should_stop = should_stop(v_ego, self.a_cruise)

    output_a_target_e2e = self.accel_boost.update(sm, output_a_target_e2e, output_a_target_mpc, self.a_cruise)

    # C3XL: 超速滑行中（Overrun>0 且实际车速高于定速）时，模型的保守加速度不再参与，
    # 否则 min() 会取模型的负加速度 → 松油门瞬间出现很强的刹车顿挫。
    over_kmh = (v_ego - v_cruise) * 3.6
    if _overrun_mode() > 0 and over_kmh > 0.0:
      output_a_target_e2e = max(output_a_target_e2e, self.a_cruise)

    # C3XL: 前车足够远（> 目标距离 1.05 倍）且模型只是轻微收油（-0.5~0）时，
    # 忽略模型的这点负加速度 —— 否则隔着 40~50 m 也会"带一点刹车"。
    lead = sm['radarState'].leadOne
    if lead.present and not output_should_stop_e2e:
      try:
        desired = get_safe_obstacle_distance(v_ego, get_T_FOLLOW(sm['selfdriveState'].personality))
        if lead.dRel > desired * 1.05 and -0.5 < output_a_target_e2e < 0.0:
          output_a_target_e2e = 0.0
        # C3XL: 前车很远（> 目标 1.15 倍）但 MPC 只是在"溜"（a > -0.05）时，
        # 让巡航的加速能赢得 min() —— 否则车永远追不上定速/拉不近车距。
        elif lead.dRel > desired * 1.15 and output_a_target_mpc > -0.05 and self.a_cruise > 0.05:
          output_a_target_mpc = max(output_a_target_mpc, self.a_cruise)
      except Exception:
        pass

    candidates = [(output_a_target_mpc, self.mpc.source, output_should_stop_mpc),
                  (self.a_cruise, LongitudinalPlanSource.cruise, cruise_should_stop),
                  (output_a_target_e2e, LongitudinalPlanSource.e2e, output_should_stop_e2e)]

    output_a_target, self.mpc.source, _ = min(candidates, key=lambda c: c[0])
    self.output_should_stop = any(should_stop for _, _, should_stop in candidates)
    self.output_a_target = np.clip(output_a_target, ACCEL_MIN, ACCEL_MAX)

    self.v_desired_filter.x = self.v_desired_filter.x + self.dt * (self.output_a_target + a_prev) / 2.0

  def publish(self, sm, pm):
    plan_send = messaging.new_message('longitudinalPlan')

    plan_send.valid = sm.all_checks()

    longitudinalPlan = plan_send.longitudinalPlan
    longitudinalPlan.modelMonoTime = sm.logMonoTime['modelV2']
    longitudinalPlan.processingDelay = (plan_send.logMonoTime / 1e9) - sm.logMonoTime['modelV2']
    longitudinalPlan.solverExecutionTime = self.mpc.solve_time

    longitudinalPlan.speeds = self.v_desired_trajectory.tolist()
    longitudinalPlan.accels = self.a_desired_trajectory.tolist()
    longitudinalPlan.jerks = self.j_desired_trajectory.tolist()

    longitudinalPlan.hasLead = sm['radarState'].leadOne.present
    longitudinalPlan.longitudinalPlanSource = self.mpc.source
    longitudinalPlan.fcw = self.fcw

    longitudinalPlan.aTarget = float(self.output_a_target)
    longitudinalPlan.accelBoost = float(self.accel_boost.total_boost)
    longitudinalPlan.shouldStop = bool(self.output_should_stop)
    longitudinalPlan.allowBrake = True

    pm.send('longitudinalPlan', plan_send)

    self.publish_longitudinal_plan_sp(sm, pm)
