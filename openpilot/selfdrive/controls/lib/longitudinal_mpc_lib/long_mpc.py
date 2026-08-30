#!/usr/bin/env python3
# ============================================================
# long_mpc.py — master-c3-release 基线 + 0814-max 特性增量移植（方案 B）
#
# 移植范围（仅纯雷达/纯状态特性，保留原 API 与 traffic_stop 交通灯停车）：
#   1. Safety Stop 安全锁            —— 前车接近静止时的最后一道防线
#   2. 追车加速抑制 V4                —— 距离近/速差大时限制 MPC 最大加速度
#   3. Lead Decel Predictor Adaptive —— 前车减速/速差大时把主障碍拉近，提前刹车
#   4. 低速前车 offset                —— 前车静止时 obstacle 减 0.5m，跟停更贴
#   5. get_T_FOLLOW 按 v_ego 动态分段 —— 高速更激进的跟车时距（注意：比基线更近）
#   6. 前车起步延迟 V2（雷达近似版）    —— 起步瞬间短时锁轨迹，避免起步即给油
#
# 未移植（依赖 modelV2/leadsV3 或与 traffic_stop 冲突，见对比结论）：
#   - process_lead 模型-雷达轨迹融合、cruise 虚拟障碍物、MPC_SOURCES 加 cruise
#
# 注意：acados 模型结构（X_DIM/U_DIM/PARAM_DIM/live params）未改动，
#       c_generated_code 无需重新生成；仅 get_T_FOLLOW 默认参数值有变，
#       运行时由 update() 的 params 覆盖，不影响已编译 solver。
# ============================================================
import os
import time
import numpy as np
from openpilot.cereal import log
from opendbc.car.interfaces import ACCEL_MIN, ACCEL_MAX
from openpilot.common.realtime import DT_MDL
from openpilot.common.swaglog import cloudlog
# WARNING: imports outside of constants will not trigger a rebuild
from openpilot.selfdrive.modeld.constants import index_function
from openpilot.selfdrive.controls.radard import _LEAD_ACCEL_TAU

if __name__ == '__main__':  # generating code
  from acados.acados_template import AcadosModel, AcadosOcp, AcadosOcpSolver
else:
  from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.c_generated_code.acados_ocp_solver_pyx import AcadosOcpSolverCython

from casadi import SX, vertcat

MODEL_NAME = 'long'
LONG_MPC_DIR = os.path.dirname(os.path.abspath(__file__))
EXPORT_DIR = os.path.join(LONG_MPC_DIR, "c_generated_code")
JSON_FILE = os.path.join(LONG_MPC_DIR, "acados_ocp_long.json")

LongitudinalPlanSource = log.LongitudinalPlan.LongitudinalPlanSource
MPC_SOURCES = (LongitudinalPlanSource.lead0, LongitudinalPlanSource.lead1)

X_DIM = 3
U_DIM = 1
PARAM_DIM = 6
COST_E_DIM = 5
COST_DIM = COST_E_DIM + 1
CONSTR_DIM = 4

X_EGO_OBSTACLE_COST = 3.
X_EGO_COST = 0.
V_EGO_COST = 0.
A_EGO_COST = 0.
J_EGO_COST = 5.
A_CHANGE_COST = 200.
DANGER_ZONE_COST = 100.
CRASH_DISTANCE = .25
LEAD_DANGER_FACTOR = 0.75
LIMIT_COST = 1e6
ACADOS_SOLVER_TYPE = 'SQP_RTI'

# Fewer timestamps don't hurt performance and lead to
# much better convergence of the MPC with low iterations
N = 12
MAX_T = 10.0
T_IDXS_LST = [index_function(idx, max_val=MAX_T, max_idx=N) for idx in range(N+1)]

T_IDXS = np.array(T_IDXS_LST)
FCW_IDXS = T_IDXS < 5.0
T_DIFFS = np.diff(T_IDXS, prepend=[0.])
COMFORT_BRAKE = 2.5
STOP_DISTANCE = 6.0
MIN_X_LEAD_FACTOR = 0.5

# ============================================================
# 0814-max 移植特性常量（方案B增量移植，保留原API与traffic_stop）
# ============================================================
# 前車起步延遲 V2（雷达近似版，无 modelV2 依赖）
START_DELAY_FRAMES = 8  # 起步反應太快增加延遲(20Hz)：10=0.5秒、12=0.6秒、16=0.8秒、20=1.0秒
START_RADAR_SPEED = 0.5  # 前车雷达速度超过该值视为"已在起步"
START_TRIGGER_DREL = 12.0  # 前车距离小于该值才启用（对应 dev 的 dRel<12.0）
START_TRIGGER_EGO = 2.0  # 自车速度低于该值才启用（对应 dev 的 v_ego<2.0）

# Lead Decel Predictor Adaptive V1
LEAD_HISTORY_SIZE = 4
LEAD_DECEL_COUNT = 2
LEAD_DECEL_BP = [0.2, 0.6, 1.2]
LEAD_OFFSET_BP = [1.0, 2.0, 3.0]
LEAD_DISTANCE_BP = [10.0, 15.0, 20.0, 30.0, 40.0, 55.0, 70.0, 90.0, 120.0]
LEAD_DISTANCE_SCALE = [0.2, 0.5, 0.8, 1.0, 1.2, 1.5, 2.0, 3.0, 4.0]
LEAD_MIN_DECEL_BP = [40.0, 60.0, 90.0, 120.0]
LEAD_MIN_DECEL = [0.00, 0.05, 0.12, 0.20]

# 前車極端急煞 / 接近靜止安全防護
SAFETY_STOP_TRIGGER = 5.0
SAFETY_STOP_MIN_DISTANCE = 4.0
SAFETY_STOP_RELEASE_DISTANCE = 5.0
SAFETY_STOP_EGO_SPEED = 25.0 / 3.6
SAFETY_STOP_LEAD_SPEED = 3.0 / 3.6
SAFETY_STOP_DECEL = -1.5  # 备用/可调，当前逻辑用 a_max 上限抑制实现

# 追車加速抑制 V4
ACCEL_SUPPRESS_VREL_BP = [0.0, 2.0, 5.0, 10.0, 20.0]
ACCEL_SUPPRESS_VREL_FACTOR = [1.0, 0.8, 0.6, 0.5, 0.4]
ACCEL_SUPPRESS_DIST_BP = [15.0, 25.0, 40.0, 60.0, 80.0]
ACCEL_SUPPRESS_DIST_FACTOR = [0.4, 0.5, 0.7, 0.9, 1.0]
ACCEL_SUPPRESS_MIN = 0.15


def get_jerk_factor(personality=log.LongitudinalPersonality.standard):
  if personality==log.LongitudinalPersonality.relaxed:
    return 1.0
  elif personality==log.LongitudinalPersonality.standard:
    return 1.0
  elif personality==log.LongitudinalPersonality.aggressive:
    return 0.8
  else:
    raise NotImplementedError("Longitudinal personality not supported")


def get_T_FOLLOW(personality=log.LongitudinalPersonality.standard, v_ego=0.0):
  if personality==log.LongitudinalPersonality.relaxed:
    return 1.55
  elif personality==log.LongitudinalPersonality.standard:
    return 1.25
  elif personality==log.LongitudinalPersonality.aggressive:
    v_kph = v_ego * 3.6
    if v_kph < 35:
      return 1.05
    elif v_kph < 70:
      return 0.95
    else:
      return 0.85
  else:
    raise NotImplementedError("Longitudinal personality not supported")

def get_stopped_equivalence_factor(v_lead):
  return (v_lead**2) / (2 * COMFORT_BRAKE)

def get_safe_obstacle_distance(v_ego, t_follow):
  return (v_ego**2) / (2 * COMFORT_BRAKE) + t_follow * v_ego + STOP_DISTANCE

def gen_long_model():
  model = AcadosModel()
  model.name = MODEL_NAME

  # states
  x_ego, v_ego, a_ego = SX.sym('x_ego'), SX.sym('v_ego'), SX.sym('a_ego')
  model.x = vertcat(x_ego, v_ego, a_ego)

  # controls
  j_ego = SX.sym('j_ego')
  model.u = vertcat(j_ego)

  # xdot
  x_ego_dot = SX.sym('x_ego_dot')
  v_ego_dot = SX.sym('v_ego_dot')
  a_ego_dot = SX.sym('a_ego_dot')
  model.xdot = vertcat(x_ego_dot, v_ego_dot, a_ego_dot)

  # live parameters
  a_min = SX.sym('a_min')
  a_max = SX.sym('a_max')
  x_obstacle = SX.sym('x_obstacle')
  a_prev = SX.sym('a_prev')
  lead_t_follow = SX.sym('lead_t_follow')
  lead_danger_factor = SX.sym('lead_danger_factor')
  model.p = vertcat(a_min, a_max, x_obstacle, a_prev, lead_t_follow, lead_danger_factor)

  # dynamics model
  f_expl = vertcat(v_ego, a_ego, j_ego)
  model.f_impl_expr = model.xdot - f_expl
  model.f_expl_expr = f_expl
  return model

def gen_long_ocp():
  ocp = AcadosOcp()
  ocp.model = gen_long_model()

  Tf = T_IDXS[-1]

  # set dimensions
  ocp.dims.N = N

  # set cost module
  ocp.cost.cost_type = 'NONLINEAR_LS'
  ocp.cost.cost_type_e = 'NONLINEAR_LS'

  QR = np.zeros((COST_DIM, COST_DIM))
  Q = np.zeros((COST_E_DIM, COST_E_DIM))

  ocp.cost.W = QR
  ocp.cost.W_e = Q

  x_ego, v_ego, a_ego = ocp.model.x[0], ocp.model.x[1], ocp.model.x[2]
  j_ego = ocp.model.u[0]

  a_min, a_max = ocp.model.p[0], ocp.model.p[1]
  x_obstacle = ocp.model.p[2]
  a_prev = ocp.model.p[3]
  lead_t_follow = ocp.model.p[4]
  lead_danger_factor = ocp.model.p[5]

  ocp.cost.yref = np.zeros((COST_DIM, ))
  ocp.cost.yref_e = np.zeros((COST_E_DIM, ))

  desired_dist_comfort = get_safe_obstacle_distance(v_ego, lead_t_follow)

  # The main cost in normal operation is how close you are to the "desired" distance
  # from an obstacle at every timestep. This obstacle can be a lead car
  # or other object. In e2e mode we can use x_position targets as a cost
  # instead.
  costs = [((x_obstacle - x_ego) - (desired_dist_comfort)) / (v_ego + 10.),
           x_ego,
           v_ego,
           a_ego,
           a_ego - a_prev,
           j_ego]
  ocp.model.cost_y_expr = vertcat(*costs)
  ocp.model.cost_y_expr_e = vertcat(*costs[:-1])

  # Constraints on speed, acceleration and desired distance to
  # the obstacle, which is treated as a slack constraint so it
  # behaves like an asymmetrical cost.
  constraints = vertcat(v_ego,
                        (a_ego - a_min),
                        (a_max - a_ego),
                        ((x_obstacle - x_ego) - lead_danger_factor * (desired_dist_comfort)) / (v_ego + 10.))
  ocp.model.con_h_expr = constraints

  x0 = np.zeros(X_DIM)
  ocp.constraints.x0 = x0
  ocp.parameter_values = np.array([-1.2, 1.2, 0.0, 0.0, get_T_FOLLOW(v_ego=0.0), LEAD_DANGER_FACTOR])


  # We put all constraint cost weights to 0 and only set them at runtime
  cost_weights = np.zeros(CONSTR_DIM)
  ocp.cost.zl = cost_weights
  ocp.cost.Zl = cost_weights
  ocp.cost.Zu = cost_weights
  ocp.cost.zu = cost_weights

  ocp.constraints.lh = np.zeros(CONSTR_DIM)
  ocp.constraints.uh = 1e4*np.ones(CONSTR_DIM)
  ocp.constraints.idxsh = np.arange(CONSTR_DIM)

  # The HPIPM solver can give decent solutions even when it is stopped early
  # Which is critical for our purpose where compute time is strictly bounded
  # We use HPIPM in the SPEED_ABS mode, which ensures fastest runtime. This
  # does not cause issues since the problem is well bounded.
  ocp.solver_options.qp_solver = 'PARTIAL_CONDENSING_HPIPM'
  ocp.solver_options.hessian_approx = 'GAUSS_NEWTON'
  ocp.solver_options.integrator_type = 'ERK'
  ocp.solver_options.nlp_solver_type = ACADOS_SOLVER_TYPE
  ocp.solver_options.qp_solver_cond_N = 1

  # More iterations take too much time and less lead to inaccurate convergence in
  # some situations. Ideally we would run just 1 iteration to ensure fixed runtime.
  ocp.solver_options.qp_solver_iter_max = 10
  ocp.solver_options.qp_tol = 1e-3

  # set prediction horizon
  ocp.solver_options.tf = Tf
  ocp.solver_options.shooting_nodes = T_IDXS

  ocp.code_export_directory = EXPORT_DIR
  return ocp


class LongitudinalMpc:
  def __init__(self, dt=DT_MDL):
    self.dt = dt
    self.solver = AcadosOcpSolverCython(MODEL_NAME, ACADOS_SOLVER_TYPE, N)
    self.reset()
    self.source = LongitudinalPlanSource.cruise

  def reset(self):
    self.solver.reset()

    self.x_sol = np.zeros((N+1, X_DIM))
    self.u_sol = np.zeros((N, 1))
    self.v_solution = np.zeros(N+1)
    self.a_solution = np.zeros(N+1)
    self.j_solution = np.zeros(N)
    self.a_prev = np.array(self.a_solution)
    self.yref = np.zeros((N+1, COST_DIM))

    for i in range(N):
      self.solver.cost_set(i, "yref", self.yref[i])
    self.solver.cost_set(N, "yref", self.yref[N][:COST_E_DIM])

    self.params = np.zeros((N+1, PARAM_DIM))
    for i in range(N+1):
      self.solver.set(i, 'x', np.zeros(X_DIM))

    self.last_cloudlog_t = 0
    self.crash_cnt = 0.0
    self.solution_status = 0
    # timers
    self.solve_time = 0.0
    self.x0 = np.zeros(X_DIM)
    self.set_weights()
    # 0814-max 移植特性状态
    self.lead_v_history = []
    self.lead_start_counter = 0
    self.safety_stop_active = False

  def set_cost_weights(self, cost_weights, constraint_cost_weights):
    W = np.asfortranarray(np.diag(cost_weights))
    for i in range(N):
      # TODO don't hardcode A_CHANGE_COST idx
      # reduce the cost on (a-a_prev) later in the horizon.
      W[4,4] = cost_weights[4] * np.interp(T_IDXS[i], [0.0, 1.0, 2.0], [1.0, 1.0, 0.0])
      self.solver.cost_set(i, 'W', W)
    # Setting the slice without the copy make the array not contiguous,
    # causing issues with the C interface.
    self.solver.cost_set(N, 'W', np.copy(W[:COST_E_DIM, :COST_E_DIM]))

    # Set L2 slack cost on lower bound constraints
    Zl = np.array(constraint_cost_weights)
    for i in range(N):
      self.solver.cost_set(i, 'Zl', Zl)

  def set_weights(self, prev_accel_constraint=True, personality=log.LongitudinalPersonality.standard):
    jerk_factor = get_jerk_factor(personality)
    a_change_cost = A_CHANGE_COST if prev_accel_constraint else 0
    cost_weights = [X_EGO_OBSTACLE_COST, X_EGO_COST, V_EGO_COST, A_EGO_COST, jerk_factor * a_change_cost, jerk_factor * J_EGO_COST]
    constraint_cost_weights = [LIMIT_COST, LIMIT_COST, LIMIT_COST, DANGER_ZONE_COST]
    self.set_cost_weights(cost_weights, constraint_cost_weights)

  def set_cur_state(self, v, a):
    v_prev = self.x0[1]
    self.x0[1] = v
    self.x0[2] = a
    if abs(v_prev - v) > 2.:  # probably only helps if v < v_prev
      for i in range(N+1):
        self.solver.set(i, 'x', self.x0)

  @staticmethod
  def extrapolate_lead(x_lead, v_lead, a_lead, a_lead_tau):
    a_lead_traj = a_lead * np.exp(-a_lead_tau * (T_IDXS**2)/2.)
    v_lead_traj = np.clip(v_lead + np.cumsum(T_DIFFS * a_lead_traj), 0.0, 1e8)
    x_lead_traj = x_lead + np.cumsum(T_DIFFS * v_lead_traj)
    lead_xv = np.column_stack((x_lead_traj, v_lead_traj))
    return lead_xv

  @staticmethod
  def _clip_lead_lead(lead_xv, v_ego):
    """MPC won't converge on immediate crashes; lift h=0 to the minimum braking distance."""
    v_lead_0 = lead_xv[0, 1]
    min_x_lead = MIN_X_LEAD_FACTOR * (v_ego + v_lead_0) * (v_ego - v_lead_0) / (-ACCEL_MIN * 2)
    lead_xv[0, 0] = max(lead_xv[0, 0], min_x_lead)
    lead_xv[:, 1] = np.clip(lead_xv[:, 1], 0.0, 1e8)
    return lead_xv

  def process_lead(self, lead):
    v_ego = self.x0[1]
    if lead is not None and lead.present:
      x_lead = lead.dRel
      v_lead = lead.vLead
      a_lead = lead.aLeadK
      a_lead_tau = lead.aLeadTau

      # 前車起步延遲 V2（雷达近似版）：前车从静止起步的瞬间短时锁定整条轨迹，
      # 避免 MPC 因预测前车加速而立刻给油。dev 原版用 modelV2 判断"前车在移动"，
      # 此处以雷达 vLead 阈值近似（触发条件 dRel<12m 且 v_ego<2.0 同 dev）。
      if lead.dRel < START_TRIGGER_DREL and v_ego < START_TRIGGER_EGO:
        if v_lead > START_RADAR_SPEED:
          self.lead_start_counter += 1
        else:
          self.lead_start_counter = 0
        if self.lead_start_counter < START_DELAY_FRAMES:
          lead_xv = np.column_stack((np.full(N + 1, x_lead), np.full(N + 1, v_lead)))
          return self._clip_lead_lead(lead_xv, v_ego)
      else:
        self.lead_start_counter = 0
    else:
      # Fake a fast lead car, so mpc can keep running in the same mode
      self.lead_start_counter = 0
      x_lead = 50.0
      v_lead = v_ego + 10.0
      a_lead = 0.0
      a_lead_tau = _LEAD_ACCEL_TAU

    # MPC will not converge if immediate crash is expected
    # Clip lead distance to what is still possible to brake for
    min_x_lead = MIN_X_LEAD_FACTOR * (v_ego + v_lead) * (v_ego - v_lead) / (-ACCEL_MIN * 2)
    x_lead = np.clip(x_lead, min_x_lead, 1e8)
    v_lead = np.clip(v_lead, 0.0, 1e8)
    a_lead = np.clip(a_lead, -10., 5.)
    lead_xv = self.extrapolate_lead(x_lead, v_lead, a_lead, a_lead_tau)
    return lead_xv

  def update_safety_stop(self, radarstate, v_ego):
    """前車極端急煞 / 接近靜止安全防護：低速且前车极近时锁定为最后防线。"""
    lead = radarstate.leadOne
    if not lead.present:
      self.safety_stop_active = False
      return
    d_rel = float(lead.dRel)
    v_lead = max(float(lead.vLead), 0.0)
    if self.safety_stop_active:
      if d_rel >= SAFETY_STOP_RELEASE_DISTANCE or v_lead > SAFETY_STOP_LEAD_SPEED * 1.5:
        self.safety_stop_active = False
    elif (
      d_rel <= SAFETY_STOP_TRIGGER and
      v_ego <= SAFETY_STOP_EGO_SPEED and
      v_lead <= SAFETY_STOP_LEAD_SPEED
    ):
      self.safety_stop_active = True

  def apply_safety_stop(self, lead_xv_0):
    """把 MPC 使用的主要前車 obstacle 压到至少 4m，同时视为低速前车。"""
    if not self.safety_stop_active:
      return
    lead_xv_0[:, 0] = np.minimum(lead_xv_0[:, 0], SAFETY_STOP_MIN_DISTANCE)
    lead_xv_0[:, 1] = np.minimum(lead_xv_0[:, 1], SAFETY_STOP_LEAD_SPEED)

  def _update_lead_history(self, radarstate):
    lead = radarstate.leadOne
    if lead.present:
      self.lead_v_history.append(float(lead.vLead))
      if len(self.lead_v_history) > LEAD_HISTORY_SIZE:
        self.lead_v_history.pop(0)
    else:
      self.lead_v_history.clear()

  def _lead_decel_offset(self, radarstate, v_ego):
    """Lead Decel Predictor Adaptive V1：前车减速/速差大时把主障碍拉近，提前刹车。"""
    lead = radarstate.leadOne
    if not (lead.present and len(self.lead_v_history) == LEAD_HISTORY_SIZE):
      return 0.0
    near_lead = lead.dRel <= 40.0
    min_decel = 0.0 if near_lead else np.interp(
      lead.dRel, LEAD_MIN_DECEL_BP, LEAD_MIN_DECEL)
    decel_count = sum(
      (self.lead_v_history[i] - self.lead_v_history[i + 1]) > min_decel
      for i in range(LEAD_HISTORY_SIZE - 1))
    lead_total_decel = 0.0
    if decel_count >= LEAD_DECEL_COUNT and lead.vLead < v_ego:
      lead_total_decel = self.lead_v_history[0] - self.lead_v_history[-1]
    closing_kph = max((v_ego - lead.vLead) * 3.6, 0.0)
    closing_total_decel = np.interp(
      closing_kph, [0.0, 10.0, 20.0, 30.0], [0.0, 0.2, 0.6, 1.2])
    total_decel = max(lead_total_decel, closing_total_decel)
    if total_decel >= LEAD_DECEL_BP[0]:
      base_offset = np.interp(total_decel, LEAD_DECEL_BP, LEAD_OFFSET_BP)
      distance_scale = np.interp(lead.dRel, LEAD_DISTANCE_BP, LEAD_DISTANCE_SCALE)
      return base_offset * distance_scale
    return 0.0

  def _suppress_accel(self, a_max, radarstate, v_ego):
    """追車加速抑制 V4：距离越近 / 速差越大 -> 限制 MPC 最大加速度。"""
    lead = radarstate.leadOne
    if not (lead.present and ((v_ego - lead.vLead) * 3.6 > 1.0)):
      return a_max
    v_rel_kph = max((v_ego - lead.vLead) * 3.6, 0.0)
    speed_factor = np.interp(
      v_rel_kph, ACCEL_SUPPRESS_VREL_BP, ACCEL_SUPPRESS_VREL_FACTOR)
    distance_factor = np.interp(
      lead.dRel, ACCEL_SUPPRESS_DIST_BP, ACCEL_SUPPRESS_DIST_FACTOR)
    reduction = max(speed_factor * distance_factor, ACCEL_SUPPRESS_MIN)
    return a_max * reduction

  def update(self, radarstate, personality=log.LongitudinalPersonality.standard, traffic_stop_obstacle_m=None):
    v_ego = self.x0[1]
    t_follow = get_T_FOLLOW(personality, v_ego)
    self.update_safety_stop(radarstate, v_ego)

    lead_xv_0 = self.process_lead(radarstate.leadOne)
    lead_xv_1 = self.process_lead(radarstate.leadTwo)

    # To estimate a safe distance from a moving lead, we calculate how much stopping
    # distance that lead needs as a minimum. We can add that to the current distance
    # and then treat that as a stopped car/obstacle at this new distance.
    lead_stop_offset_0 = np.where(lead_xv_0[:, 1] < 1.0, 0.5, 0.0)
    lead_stop_offset_1 = np.where(lead_xv_1[:, 1] < 1.0, 0.5, 0.0)

    lead_0_obstacle = (
      lead_xv_0[:, 0]
      + get_stopped_equivalence_factor(lead_xv_0[:, 1])
      - lead_stop_offset_0
    )
    lead_1_obstacle = (
      lead_xv_1[:, 0]
      + get_stopped_equivalence_factor(lead_xv_1[:, 1])
      - lead_stop_offset_1
    )

    # Lead Decel Predictor Adaptive V1
    self._update_lead_history(radarstate)
    lead_0_obstacle = lead_0_obstacle - self._lead_decel_offset(radarstate, v_ego)

    self.apply_safety_stop(lead_xv_0)
    if self.safety_stop_active:
      lead_0_obstacle = np.minimum(lead_0_obstacle, SAFETY_STOP_MIN_DISTANCE)

    obstacle_cols = [lead_0_obstacle, lead_1_obstacle]
    obstacle_sources = list(MPC_SOURCES)

    if traffic_stop_obstacle_m is not None:
      # Traffic-light / stop-sign virtual stop-line obstacle (sunnypilot addition, ported from
      # carrot). Represented as an N+1-timestep column of the same constant distance -- i.e. a
      # lead car with zero relative speed that never moves -- so it collapses into the same
      # np.min() below as any other obstacle. No acados model change needed.
      # NOTE: LongitudinalPlanSource has no dedicated enum value for this source; `cruise` is
      # borrowed purely for telemetry/debug display and has no effect on the actual braking.
      # traffic_stop_obstacle_m already has TrafficStopDistanceAdjust applied by
      # TrafficStopController -- do not apply any further offset here.
      obstacle_cols.append(np.full(N + 1, traffic_stop_obstacle_m))
      obstacle_sources.append(LongitudinalPlanSource.cruise)

    x_obstacles = np.column_stack(obstacle_cols)
    self.source = obstacle_sources[int(np.argmin(x_obstacles[0]))]

    self.yref[:,:] = 0.0
    for i in range(N):
      self.solver.set(i, "yref", self.yref[i])
    self.solver.set(N, "yref", self.yref[N][:COST_E_DIM])

    self.params[:,0] = ACCEL_MIN
    self.params[:,1] = ACCEL_MAX
    self.params[:,2] = np.min(x_obstacles, axis=1)
    self.params[:,3] = np.copy(self.a_prev)
    self.params[:,4] = t_follow
    self.params[:,5] = LEAD_DANGER_FACTOR

    if self.safety_stop_active:
      # 不允許 MPC 在安全鎖定期間產生正加速度。
      self.params[:,1] = np.minimum(self.params[:,1], 0.0)

    # 追車加速抑制 V4
    self.params[:,1] = self._suppress_accel(self.params[:,1], radarstate, v_ego)

    self.run()
    if (np.any(lead_xv_0[FCW_IDXS,0] - self.x_sol[FCW_IDXS,0] < CRASH_DISTANCE) and
            radarstate.leadOne.modelProb > 0.9):
      self.crash_cnt += 1
    else:
      self.crash_cnt = 0

  def run(self):
    for i in range(N+1):
      self.solver.set(i, 'p', self.params[i])
    self.solver.constraints_set(0, "lbx", self.x0)
    self.solver.constraints_set(0, "ubx", self.x0)

    self.solution_status = self.solver.solve()
    self.solve_time = float(self.solver.get_stats('time_tot')[0])

    for i in range(N+1):
      self.x_sol[i] = self.solver.get(i, 'x')
    for i in range(N):
      self.u_sol[i] = self.solver.get(i, 'u')

    self.v_solution = self.x_sol[:,1]
    self.a_solution = self.x_sol[:,2]
    self.j_solution = self.u_sol[:,0]

    self.a_prev = np.interp(T_IDXS + self.dt, T_IDXS, self.a_solution)

    t = time.monotonic()
    if self.solution_status != 0:
      if t > self.last_cloudlog_t + 5.0:
        self.last_cloudlog_t = t
        cloudlog.warning(f"Long mpc reset, solution_status: {self.solution_status}")
      self.reset()


if __name__ == "__main__":
  ocp = gen_long_ocp()
  AcadosOcpSolver.generate(ocp, json_file=JSON_FILE)
