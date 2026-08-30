#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_7636272527841291664) {
   out_7636272527841291664[0] = delta_x[0] + nom_x[0];
   out_7636272527841291664[1] = delta_x[1] + nom_x[1];
   out_7636272527841291664[2] = delta_x[2] + nom_x[2];
   out_7636272527841291664[3] = delta_x[3] + nom_x[3];
   out_7636272527841291664[4] = delta_x[4] + nom_x[4];
   out_7636272527841291664[5] = delta_x[5] + nom_x[5];
   out_7636272527841291664[6] = delta_x[6] + nom_x[6];
   out_7636272527841291664[7] = delta_x[7] + nom_x[7];
   out_7636272527841291664[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5731341223820761712) {
   out_5731341223820761712[0] = -nom_x[0] + true_x[0];
   out_5731341223820761712[1] = -nom_x[1] + true_x[1];
   out_5731341223820761712[2] = -nom_x[2] + true_x[2];
   out_5731341223820761712[3] = -nom_x[3] + true_x[3];
   out_5731341223820761712[4] = -nom_x[4] + true_x[4];
   out_5731341223820761712[5] = -nom_x[5] + true_x[5];
   out_5731341223820761712[6] = -nom_x[6] + true_x[6];
   out_5731341223820761712[7] = -nom_x[7] + true_x[7];
   out_5731341223820761712[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_5368470991001475643) {
   out_5368470991001475643[0] = 1.0;
   out_5368470991001475643[1] = 0.0;
   out_5368470991001475643[2] = 0.0;
   out_5368470991001475643[3] = 0.0;
   out_5368470991001475643[4] = 0.0;
   out_5368470991001475643[5] = 0.0;
   out_5368470991001475643[6] = 0.0;
   out_5368470991001475643[7] = 0.0;
   out_5368470991001475643[8] = 0.0;
   out_5368470991001475643[9] = 0.0;
   out_5368470991001475643[10] = 1.0;
   out_5368470991001475643[11] = 0.0;
   out_5368470991001475643[12] = 0.0;
   out_5368470991001475643[13] = 0.0;
   out_5368470991001475643[14] = 0.0;
   out_5368470991001475643[15] = 0.0;
   out_5368470991001475643[16] = 0.0;
   out_5368470991001475643[17] = 0.0;
   out_5368470991001475643[18] = 0.0;
   out_5368470991001475643[19] = 0.0;
   out_5368470991001475643[20] = 1.0;
   out_5368470991001475643[21] = 0.0;
   out_5368470991001475643[22] = 0.0;
   out_5368470991001475643[23] = 0.0;
   out_5368470991001475643[24] = 0.0;
   out_5368470991001475643[25] = 0.0;
   out_5368470991001475643[26] = 0.0;
   out_5368470991001475643[27] = 0.0;
   out_5368470991001475643[28] = 0.0;
   out_5368470991001475643[29] = 0.0;
   out_5368470991001475643[30] = 1.0;
   out_5368470991001475643[31] = 0.0;
   out_5368470991001475643[32] = 0.0;
   out_5368470991001475643[33] = 0.0;
   out_5368470991001475643[34] = 0.0;
   out_5368470991001475643[35] = 0.0;
   out_5368470991001475643[36] = 0.0;
   out_5368470991001475643[37] = 0.0;
   out_5368470991001475643[38] = 0.0;
   out_5368470991001475643[39] = 0.0;
   out_5368470991001475643[40] = 1.0;
   out_5368470991001475643[41] = 0.0;
   out_5368470991001475643[42] = 0.0;
   out_5368470991001475643[43] = 0.0;
   out_5368470991001475643[44] = 0.0;
   out_5368470991001475643[45] = 0.0;
   out_5368470991001475643[46] = 0.0;
   out_5368470991001475643[47] = 0.0;
   out_5368470991001475643[48] = 0.0;
   out_5368470991001475643[49] = 0.0;
   out_5368470991001475643[50] = 1.0;
   out_5368470991001475643[51] = 0.0;
   out_5368470991001475643[52] = 0.0;
   out_5368470991001475643[53] = 0.0;
   out_5368470991001475643[54] = 0.0;
   out_5368470991001475643[55] = 0.0;
   out_5368470991001475643[56] = 0.0;
   out_5368470991001475643[57] = 0.0;
   out_5368470991001475643[58] = 0.0;
   out_5368470991001475643[59] = 0.0;
   out_5368470991001475643[60] = 1.0;
   out_5368470991001475643[61] = 0.0;
   out_5368470991001475643[62] = 0.0;
   out_5368470991001475643[63] = 0.0;
   out_5368470991001475643[64] = 0.0;
   out_5368470991001475643[65] = 0.0;
   out_5368470991001475643[66] = 0.0;
   out_5368470991001475643[67] = 0.0;
   out_5368470991001475643[68] = 0.0;
   out_5368470991001475643[69] = 0.0;
   out_5368470991001475643[70] = 1.0;
   out_5368470991001475643[71] = 0.0;
   out_5368470991001475643[72] = 0.0;
   out_5368470991001475643[73] = 0.0;
   out_5368470991001475643[74] = 0.0;
   out_5368470991001475643[75] = 0.0;
   out_5368470991001475643[76] = 0.0;
   out_5368470991001475643[77] = 0.0;
   out_5368470991001475643[78] = 0.0;
   out_5368470991001475643[79] = 0.0;
   out_5368470991001475643[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_2692593086736214382) {
   out_2692593086736214382[0] = state[0];
   out_2692593086736214382[1] = state[1];
   out_2692593086736214382[2] = state[2];
   out_2692593086736214382[3] = state[3];
   out_2692593086736214382[4] = state[4];
   out_2692593086736214382[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_2692593086736214382[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_2692593086736214382[7] = state[7];
   out_2692593086736214382[8] = state[8];
}
void F_fun(double *state, double dt, double *out_6312543211099338601) {
   out_6312543211099338601[0] = 1;
   out_6312543211099338601[1] = 0;
   out_6312543211099338601[2] = 0;
   out_6312543211099338601[3] = 0;
   out_6312543211099338601[4] = 0;
   out_6312543211099338601[5] = 0;
   out_6312543211099338601[6] = 0;
   out_6312543211099338601[7] = 0;
   out_6312543211099338601[8] = 0;
   out_6312543211099338601[9] = 0;
   out_6312543211099338601[10] = 1;
   out_6312543211099338601[11] = 0;
   out_6312543211099338601[12] = 0;
   out_6312543211099338601[13] = 0;
   out_6312543211099338601[14] = 0;
   out_6312543211099338601[15] = 0;
   out_6312543211099338601[16] = 0;
   out_6312543211099338601[17] = 0;
   out_6312543211099338601[18] = 0;
   out_6312543211099338601[19] = 0;
   out_6312543211099338601[20] = 1;
   out_6312543211099338601[21] = 0;
   out_6312543211099338601[22] = 0;
   out_6312543211099338601[23] = 0;
   out_6312543211099338601[24] = 0;
   out_6312543211099338601[25] = 0;
   out_6312543211099338601[26] = 0;
   out_6312543211099338601[27] = 0;
   out_6312543211099338601[28] = 0;
   out_6312543211099338601[29] = 0;
   out_6312543211099338601[30] = 1;
   out_6312543211099338601[31] = 0;
   out_6312543211099338601[32] = 0;
   out_6312543211099338601[33] = 0;
   out_6312543211099338601[34] = 0;
   out_6312543211099338601[35] = 0;
   out_6312543211099338601[36] = 0;
   out_6312543211099338601[37] = 0;
   out_6312543211099338601[38] = 0;
   out_6312543211099338601[39] = 0;
   out_6312543211099338601[40] = 1;
   out_6312543211099338601[41] = 0;
   out_6312543211099338601[42] = 0;
   out_6312543211099338601[43] = 0;
   out_6312543211099338601[44] = 0;
   out_6312543211099338601[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_6312543211099338601[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_6312543211099338601[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6312543211099338601[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_6312543211099338601[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_6312543211099338601[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_6312543211099338601[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_6312543211099338601[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_6312543211099338601[53] = -9.8100000000000005*dt;
   out_6312543211099338601[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_6312543211099338601[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_6312543211099338601[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6312543211099338601[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6312543211099338601[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_6312543211099338601[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_6312543211099338601[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_6312543211099338601[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_6312543211099338601[62] = 0;
   out_6312543211099338601[63] = 0;
   out_6312543211099338601[64] = 0;
   out_6312543211099338601[65] = 0;
   out_6312543211099338601[66] = 0;
   out_6312543211099338601[67] = 0;
   out_6312543211099338601[68] = 0;
   out_6312543211099338601[69] = 0;
   out_6312543211099338601[70] = 1;
   out_6312543211099338601[71] = 0;
   out_6312543211099338601[72] = 0;
   out_6312543211099338601[73] = 0;
   out_6312543211099338601[74] = 0;
   out_6312543211099338601[75] = 0;
   out_6312543211099338601[76] = 0;
   out_6312543211099338601[77] = 0;
   out_6312543211099338601[78] = 0;
   out_6312543211099338601[79] = 0;
   out_6312543211099338601[80] = 1;
}
void h_25(double *state, double *unused, double *out_2446433457103868726) {
   out_2446433457103868726[0] = state[6];
}
void H_25(double *state, double *unused, double *out_6581250122818046022) {
   out_6581250122818046022[0] = 0;
   out_6581250122818046022[1] = 0;
   out_6581250122818046022[2] = 0;
   out_6581250122818046022[3] = 0;
   out_6581250122818046022[4] = 0;
   out_6581250122818046022[5] = 0;
   out_6581250122818046022[6] = 1;
   out_6581250122818046022[7] = 0;
   out_6581250122818046022[8] = 0;
}
void h_24(double *state, double *unused, double *out_8050236172575186015) {
   out_8050236172575186015[0] = state[4];
   out_8050236172575186015[1] = state[5];
}
void H_24(double *state, double *unused, double *out_6591999667978493000) {
   out_6591999667978493000[0] = 0;
   out_6591999667978493000[1] = 0;
   out_6591999667978493000[2] = 0;
   out_6591999667978493000[3] = 0;
   out_6591999667978493000[4] = 1;
   out_6591999667978493000[5] = 0;
   out_6591999667978493000[6] = 0;
   out_6591999667978493000[7] = 0;
   out_6591999667978493000[8] = 0;
   out_6591999667978493000[9] = 0;
   out_6591999667978493000[10] = 0;
   out_6591999667978493000[11] = 0;
   out_6591999667978493000[12] = 0;
   out_6591999667978493000[13] = 0;
   out_6591999667978493000[14] = 1;
   out_6591999667978493000[15] = 0;
   out_6591999667978493000[16] = 0;
   out_6591999667978493000[17] = 0;
}
void h_30(double *state, double *unused, double *out_2171239394819362837) {
   out_2171239394819362837[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2053553792690437824) {
   out_2053553792690437824[0] = 0;
   out_2053553792690437824[1] = 0;
   out_2053553792690437824[2] = 0;
   out_2053553792690437824[3] = 0;
   out_2053553792690437824[4] = 1;
   out_2053553792690437824[5] = 0;
   out_2053553792690437824[6] = 0;
   out_2053553792690437824[7] = 0;
   out_2053553792690437824[8] = 0;
}
void h_26(double *state, double *unused, double *out_1227729541917700402) {
   out_1227729541917700402[0] = state[7];
}
void H_26(double *state, double *unused, double *out_2839746803943989798) {
   out_2839746803943989798[0] = 0;
   out_2839746803943989798[1] = 0;
   out_2839746803943989798[2] = 0;
   out_2839746803943989798[3] = 0;
   out_2839746803943989798[4] = 0;
   out_2839746803943989798[5] = 0;
   out_2839746803943989798[6] = 0;
   out_2839746803943989798[7] = 1;
   out_2839746803943989798[8] = 0;
}
void h_27(double *state, double *unused, double *out_8997547912812617111) {
   out_8997547912812617111[0] = state[3];
}
void H_27(double *state, double *unused, double *out_121209519109987087) {
   out_121209519109987087[0] = 0;
   out_121209519109987087[1] = 0;
   out_121209519109987087[2] = 0;
   out_121209519109987087[3] = 1;
   out_121209519109987087[4] = 0;
   out_121209519109987087[5] = 0;
   out_121209519109987087[6] = 0;
   out_121209519109987087[7] = 0;
   out_121209519109987087[8] = 0;
}
void h_29(double *state, double *unused, double *out_8840361244461487966) {
   out_8840361244461487966[0] = state[1];
}
void H_29(double *state, double *unused, double *out_2563785137004830008) {
   out_2563785137004830008[0] = 0;
   out_2563785137004830008[1] = 1;
   out_2563785137004830008[2] = 0;
   out_2563785137004830008[3] = 0;
   out_2563785137004830008[4] = 0;
   out_2563785137004830008[5] = 0;
   out_2563785137004830008[6] = 0;
   out_2563785137004830008[7] = 0;
   out_2563785137004830008[8] = 0;
}
void h_28(double *state, double *unused, double *out_864791975821295959) {
   out_864791975821295959[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2518613880064700566) {
   out_2518613880064700566[0] = 1;
   out_2518613880064700566[1] = 0;
   out_2518613880064700566[2] = 0;
   out_2518613880064700566[3] = 0;
   out_2518613880064700566[4] = 0;
   out_2518613880064700566[5] = 0;
   out_2518613880064700566[6] = 0;
   out_2518613880064700566[7] = 0;
   out_2518613880064700566[8] = 0;
}
void h_31(double *state, double *unused, double *out_2289246788752739581) {
   out_2289246788752739581[0] = state[8];
}
void H_31(double *state, double *unused, double *out_2213538701710638322) {
   out_2213538701710638322[0] = 0;
   out_2213538701710638322[1] = 0;
   out_2213538701710638322[2] = 0;
   out_2213538701710638322[3] = 0;
   out_2213538701710638322[4] = 0;
   out_2213538701710638322[5] = 0;
   out_2213538701710638322[6] = 0;
   out_2213538701710638322[7] = 0;
   out_2213538701710638322[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_7636272527841291664) {
  err_fun(nom_x, delta_x, out_7636272527841291664);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5731341223820761712) {
  inv_err_fun(nom_x, true_x, out_5731341223820761712);
}
void car_H_mod_fun(double *state, double *out_5368470991001475643) {
  H_mod_fun(state, out_5368470991001475643);
}
void car_f_fun(double *state, double dt, double *out_2692593086736214382) {
  f_fun(state,  dt, out_2692593086736214382);
}
void car_F_fun(double *state, double dt, double *out_6312543211099338601) {
  F_fun(state,  dt, out_6312543211099338601);
}
void car_h_25(double *state, double *unused, double *out_2446433457103868726) {
  h_25(state, unused, out_2446433457103868726);
}
void car_H_25(double *state, double *unused, double *out_6581250122818046022) {
  H_25(state, unused, out_6581250122818046022);
}
void car_h_24(double *state, double *unused, double *out_8050236172575186015) {
  h_24(state, unused, out_8050236172575186015);
}
void car_H_24(double *state, double *unused, double *out_6591999667978493000) {
  H_24(state, unused, out_6591999667978493000);
}
void car_h_30(double *state, double *unused, double *out_2171239394819362837) {
  h_30(state, unused, out_2171239394819362837);
}
void car_H_30(double *state, double *unused, double *out_2053553792690437824) {
  H_30(state, unused, out_2053553792690437824);
}
void car_h_26(double *state, double *unused, double *out_1227729541917700402) {
  h_26(state, unused, out_1227729541917700402);
}
void car_H_26(double *state, double *unused, double *out_2839746803943989798) {
  H_26(state, unused, out_2839746803943989798);
}
void car_h_27(double *state, double *unused, double *out_8997547912812617111) {
  h_27(state, unused, out_8997547912812617111);
}
void car_H_27(double *state, double *unused, double *out_121209519109987087) {
  H_27(state, unused, out_121209519109987087);
}
void car_h_29(double *state, double *unused, double *out_8840361244461487966) {
  h_29(state, unused, out_8840361244461487966);
}
void car_H_29(double *state, double *unused, double *out_2563785137004830008) {
  H_29(state, unused, out_2563785137004830008);
}
void car_h_28(double *state, double *unused, double *out_864791975821295959) {
  h_28(state, unused, out_864791975821295959);
}
void car_H_28(double *state, double *unused, double *out_2518613880064700566) {
  H_28(state, unused, out_2518613880064700566);
}
void car_h_31(double *state, double *unused, double *out_2289246788752739581) {
  h_31(state, unused, out_2289246788752739581);
}
void car_H_31(double *state, double *unused, double *out_2213538701710638322) {
  H_31(state, unused, out_2213538701710638322);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
