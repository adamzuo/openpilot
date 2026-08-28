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
void err_fun(double *nom_x, double *delta_x, double *out_3260711326014191875) {
   out_3260711326014191875[0] = delta_x[0] + nom_x[0];
   out_3260711326014191875[1] = delta_x[1] + nom_x[1];
   out_3260711326014191875[2] = delta_x[2] + nom_x[2];
   out_3260711326014191875[3] = delta_x[3] + nom_x[3];
   out_3260711326014191875[4] = delta_x[4] + nom_x[4];
   out_3260711326014191875[5] = delta_x[5] + nom_x[5];
   out_3260711326014191875[6] = delta_x[6] + nom_x[6];
   out_3260711326014191875[7] = delta_x[7] + nom_x[7];
   out_3260711326014191875[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3461417183998723496) {
   out_3461417183998723496[0] = -nom_x[0] + true_x[0];
   out_3461417183998723496[1] = -nom_x[1] + true_x[1];
   out_3461417183998723496[2] = -nom_x[2] + true_x[2];
   out_3461417183998723496[3] = -nom_x[3] + true_x[3];
   out_3461417183998723496[4] = -nom_x[4] + true_x[4];
   out_3461417183998723496[5] = -nom_x[5] + true_x[5];
   out_3461417183998723496[6] = -nom_x[6] + true_x[6];
   out_3461417183998723496[7] = -nom_x[7] + true_x[7];
   out_3461417183998723496[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_728395279376170320) {
   out_728395279376170320[0] = 1.0;
   out_728395279376170320[1] = 0.0;
   out_728395279376170320[2] = 0.0;
   out_728395279376170320[3] = 0.0;
   out_728395279376170320[4] = 0.0;
   out_728395279376170320[5] = 0.0;
   out_728395279376170320[6] = 0.0;
   out_728395279376170320[7] = 0.0;
   out_728395279376170320[8] = 0.0;
   out_728395279376170320[9] = 0.0;
   out_728395279376170320[10] = 1.0;
   out_728395279376170320[11] = 0.0;
   out_728395279376170320[12] = 0.0;
   out_728395279376170320[13] = 0.0;
   out_728395279376170320[14] = 0.0;
   out_728395279376170320[15] = 0.0;
   out_728395279376170320[16] = 0.0;
   out_728395279376170320[17] = 0.0;
   out_728395279376170320[18] = 0.0;
   out_728395279376170320[19] = 0.0;
   out_728395279376170320[20] = 1.0;
   out_728395279376170320[21] = 0.0;
   out_728395279376170320[22] = 0.0;
   out_728395279376170320[23] = 0.0;
   out_728395279376170320[24] = 0.0;
   out_728395279376170320[25] = 0.0;
   out_728395279376170320[26] = 0.0;
   out_728395279376170320[27] = 0.0;
   out_728395279376170320[28] = 0.0;
   out_728395279376170320[29] = 0.0;
   out_728395279376170320[30] = 1.0;
   out_728395279376170320[31] = 0.0;
   out_728395279376170320[32] = 0.0;
   out_728395279376170320[33] = 0.0;
   out_728395279376170320[34] = 0.0;
   out_728395279376170320[35] = 0.0;
   out_728395279376170320[36] = 0.0;
   out_728395279376170320[37] = 0.0;
   out_728395279376170320[38] = 0.0;
   out_728395279376170320[39] = 0.0;
   out_728395279376170320[40] = 1.0;
   out_728395279376170320[41] = 0.0;
   out_728395279376170320[42] = 0.0;
   out_728395279376170320[43] = 0.0;
   out_728395279376170320[44] = 0.0;
   out_728395279376170320[45] = 0.0;
   out_728395279376170320[46] = 0.0;
   out_728395279376170320[47] = 0.0;
   out_728395279376170320[48] = 0.0;
   out_728395279376170320[49] = 0.0;
   out_728395279376170320[50] = 1.0;
   out_728395279376170320[51] = 0.0;
   out_728395279376170320[52] = 0.0;
   out_728395279376170320[53] = 0.0;
   out_728395279376170320[54] = 0.0;
   out_728395279376170320[55] = 0.0;
   out_728395279376170320[56] = 0.0;
   out_728395279376170320[57] = 0.0;
   out_728395279376170320[58] = 0.0;
   out_728395279376170320[59] = 0.0;
   out_728395279376170320[60] = 1.0;
   out_728395279376170320[61] = 0.0;
   out_728395279376170320[62] = 0.0;
   out_728395279376170320[63] = 0.0;
   out_728395279376170320[64] = 0.0;
   out_728395279376170320[65] = 0.0;
   out_728395279376170320[66] = 0.0;
   out_728395279376170320[67] = 0.0;
   out_728395279376170320[68] = 0.0;
   out_728395279376170320[69] = 0.0;
   out_728395279376170320[70] = 1.0;
   out_728395279376170320[71] = 0.0;
   out_728395279376170320[72] = 0.0;
   out_728395279376170320[73] = 0.0;
   out_728395279376170320[74] = 0.0;
   out_728395279376170320[75] = 0.0;
   out_728395279376170320[76] = 0.0;
   out_728395279376170320[77] = 0.0;
   out_728395279376170320[78] = 0.0;
   out_728395279376170320[79] = 0.0;
   out_728395279376170320[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_7241043703078136547) {
   out_7241043703078136547[0] = state[0];
   out_7241043703078136547[1] = state[1];
   out_7241043703078136547[2] = state[2];
   out_7241043703078136547[3] = state[3];
   out_7241043703078136547[4] = state[4];
   out_7241043703078136547[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_7241043703078136547[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_7241043703078136547[7] = state[7];
   out_7241043703078136547[8] = state[8];
}
void F_fun(double *state, double dt, double *out_1989643418376622314) {
   out_1989643418376622314[0] = 1;
   out_1989643418376622314[1] = 0;
   out_1989643418376622314[2] = 0;
   out_1989643418376622314[3] = 0;
   out_1989643418376622314[4] = 0;
   out_1989643418376622314[5] = 0;
   out_1989643418376622314[6] = 0;
   out_1989643418376622314[7] = 0;
   out_1989643418376622314[8] = 0;
   out_1989643418376622314[9] = 0;
   out_1989643418376622314[10] = 1;
   out_1989643418376622314[11] = 0;
   out_1989643418376622314[12] = 0;
   out_1989643418376622314[13] = 0;
   out_1989643418376622314[14] = 0;
   out_1989643418376622314[15] = 0;
   out_1989643418376622314[16] = 0;
   out_1989643418376622314[17] = 0;
   out_1989643418376622314[18] = 0;
   out_1989643418376622314[19] = 0;
   out_1989643418376622314[20] = 1;
   out_1989643418376622314[21] = 0;
   out_1989643418376622314[22] = 0;
   out_1989643418376622314[23] = 0;
   out_1989643418376622314[24] = 0;
   out_1989643418376622314[25] = 0;
   out_1989643418376622314[26] = 0;
   out_1989643418376622314[27] = 0;
   out_1989643418376622314[28] = 0;
   out_1989643418376622314[29] = 0;
   out_1989643418376622314[30] = 1;
   out_1989643418376622314[31] = 0;
   out_1989643418376622314[32] = 0;
   out_1989643418376622314[33] = 0;
   out_1989643418376622314[34] = 0;
   out_1989643418376622314[35] = 0;
   out_1989643418376622314[36] = 0;
   out_1989643418376622314[37] = 0;
   out_1989643418376622314[38] = 0;
   out_1989643418376622314[39] = 0;
   out_1989643418376622314[40] = 1;
   out_1989643418376622314[41] = 0;
   out_1989643418376622314[42] = 0;
   out_1989643418376622314[43] = 0;
   out_1989643418376622314[44] = 0;
   out_1989643418376622314[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_1989643418376622314[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_1989643418376622314[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1989643418376622314[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_1989643418376622314[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_1989643418376622314[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_1989643418376622314[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_1989643418376622314[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_1989643418376622314[53] = -9.8100000000000005*dt;
   out_1989643418376622314[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_1989643418376622314[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_1989643418376622314[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1989643418376622314[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1989643418376622314[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_1989643418376622314[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_1989643418376622314[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_1989643418376622314[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_1989643418376622314[62] = 0;
   out_1989643418376622314[63] = 0;
   out_1989643418376622314[64] = 0;
   out_1989643418376622314[65] = 0;
   out_1989643418376622314[66] = 0;
   out_1989643418376622314[67] = 0;
   out_1989643418376622314[68] = 0;
   out_1989643418376622314[69] = 0;
   out_1989643418376622314[70] = 1;
   out_1989643418376622314[71] = 0;
   out_1989643418376622314[72] = 0;
   out_1989643418376622314[73] = 0;
   out_1989643418376622314[74] = 0;
   out_1989643418376622314[75] = 0;
   out_1989643418376622314[76] = 0;
   out_1989643418376622314[77] = 0;
   out_1989643418376622314[78] = 0;
   out_1989643418376622314[79] = 0;
   out_1989643418376622314[80] = 1;
}
void h_25(double *state, double *unused, double *out_888466012610996564) {
   out_888466012610996564[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1340130006384542436) {
   out_1340130006384542436[0] = 0;
   out_1340130006384542436[1] = 0;
   out_1340130006384542436[2] = 0;
   out_1340130006384542436[3] = 0;
   out_1340130006384542436[4] = 0;
   out_1340130006384542436[5] = 0;
   out_1340130006384542436[6] = 1;
   out_1340130006384542436[7] = 0;
   out_1340130006384542436[8] = 0;
}
void h_24(double *state, double *unused, double *out_2820570096846580141) {
   out_2820570096846580141[0] = state[4];
   out_2820570096846580141[1] = state[5];
}
void H_24(double *state, double *unused, double *out_5235441800206975665) {
   out_5235441800206975665[0] = 0;
   out_5235441800206975665[1] = 0;
   out_5235441800206975665[2] = 0;
   out_5235441800206975665[3] = 0;
   out_5235441800206975665[4] = 1;
   out_5235441800206975665[5] = 0;
   out_5235441800206975665[6] = 0;
   out_5235441800206975665[7] = 0;
   out_5235441800206975665[8] = 0;
   out_5235441800206975665[9] = 0;
   out_5235441800206975665[10] = 0;
   out_5235441800206975665[11] = 0;
   out_5235441800206975665[12] = 0;
   out_5235441800206975665[13] = 0;
   out_5235441800206975665[14] = 1;
   out_5235441800206975665[15] = 0;
   out_5235441800206975665[16] = 0;
   out_5235441800206975665[17] = 0;
}
void h_30(double *state, double *unused, double *out_1033985836831651535) {
   out_1033985836831651535[0] = state[4];
}
void H_30(double *state, double *unused, double *out_5576560335107074319) {
   out_5576560335107074319[0] = 0;
   out_5576560335107074319[1] = 0;
   out_5576560335107074319[2] = 0;
   out_5576560335107074319[3] = 0;
   out_5576560335107074319[4] = 1;
   out_5576560335107074319[5] = 0;
   out_5576560335107074319[6] = 0;
   out_5576560335107074319[7] = 0;
   out_5576560335107074319[8] = 0;
}
void h_26(double *state, double *unused, double *out_4191921379003673412) {
   out_4191921379003673412[0] = state[7];
}
void H_26(double *state, double *unused, double *out_5081633325258598660) {
   out_5081633325258598660[0] = 0;
   out_5081633325258598660[1] = 0;
   out_5081633325258598660[2] = 0;
   out_5081633325258598660[3] = 0;
   out_5081633325258598660[4] = 0;
   out_5081633325258598660[5] = 0;
   out_5081633325258598660[6] = 0;
   out_5081633325258598660[7] = 1;
   out_5081633325258598660[8] = 0;
}
void h_27(double *state, double *unused, double *out_6637077262105860734) {
   out_6637077262105860734[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3401797023306649408) {
   out_3401797023306649408[0] = 0;
   out_3401797023306649408[1] = 0;
   out_3401797023306649408[2] = 0;
   out_3401797023306649408[3] = 1;
   out_3401797023306649408[4] = 0;
   out_3401797023306649408[5] = 0;
   out_3401797023306649408[6] = 0;
   out_3401797023306649408[7] = 0;
   out_3401797023306649408[8] = 0;
}
void h_29(double *state, double *unused, double *out_8172217283349668532) {
   out_8172217283349668532[0] = state[1];
}
void H_29(double *state, double *unused, double *out_6086791679421466503) {
   out_6086791679421466503[0] = 0;
   out_6086791679421466503[1] = 1;
   out_6086791679421466503[2] = 0;
   out_6086791679421466503[3] = 0;
   out_6086791679421466503[4] = 0;
   out_6086791679421466503[5] = 0;
   out_6086791679421466503[6] = 0;
   out_6086791679421466503[7] = 0;
   out_6086791679421466503[8] = 0;
}
void h_28(double *state, double *unused, double *out_4946629427370179774) {
   out_4946629427370179774[0] = state[0];
}
void H_28(double *state, double *unused, double *out_3393964720632432199) {
   out_3393964720632432199[0] = 1;
   out_3393964720632432199[1] = 0;
   out_3393964720632432199[2] = 0;
   out_3393964720632432199[3] = 0;
   out_3393964720632432199[4] = 0;
   out_3393964720632432199[5] = 0;
   out_3393964720632432199[6] = 0;
   out_3393964720632432199[7] = 0;
   out_3393964720632432199[8] = 0;
}
void h_31(double *state, double *unused, double *out_3086035387073898256) {
   out_3086035387073898256[0] = state[8];
}
void H_31(double *state, double *unused, double *out_1309484044507582008) {
   out_1309484044507582008[0] = 0;
   out_1309484044507582008[1] = 0;
   out_1309484044507582008[2] = 0;
   out_1309484044507582008[3] = 0;
   out_1309484044507582008[4] = 0;
   out_1309484044507582008[5] = 0;
   out_1309484044507582008[6] = 0;
   out_1309484044507582008[7] = 0;
   out_1309484044507582008[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_3260711326014191875) {
  err_fun(nom_x, delta_x, out_3260711326014191875);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3461417183998723496) {
  inv_err_fun(nom_x, true_x, out_3461417183998723496);
}
void car_H_mod_fun(double *state, double *out_728395279376170320) {
  H_mod_fun(state, out_728395279376170320);
}
void car_f_fun(double *state, double dt, double *out_7241043703078136547) {
  f_fun(state,  dt, out_7241043703078136547);
}
void car_F_fun(double *state, double dt, double *out_1989643418376622314) {
  F_fun(state,  dt, out_1989643418376622314);
}
void car_h_25(double *state, double *unused, double *out_888466012610996564) {
  h_25(state, unused, out_888466012610996564);
}
void car_H_25(double *state, double *unused, double *out_1340130006384542436) {
  H_25(state, unused, out_1340130006384542436);
}
void car_h_24(double *state, double *unused, double *out_2820570096846580141) {
  h_24(state, unused, out_2820570096846580141);
}
void car_H_24(double *state, double *unused, double *out_5235441800206975665) {
  H_24(state, unused, out_5235441800206975665);
}
void car_h_30(double *state, double *unused, double *out_1033985836831651535) {
  h_30(state, unused, out_1033985836831651535);
}
void car_H_30(double *state, double *unused, double *out_5576560335107074319) {
  H_30(state, unused, out_5576560335107074319);
}
void car_h_26(double *state, double *unused, double *out_4191921379003673412) {
  h_26(state, unused, out_4191921379003673412);
}
void car_H_26(double *state, double *unused, double *out_5081633325258598660) {
  H_26(state, unused, out_5081633325258598660);
}
void car_h_27(double *state, double *unused, double *out_6637077262105860734) {
  h_27(state, unused, out_6637077262105860734);
}
void car_H_27(double *state, double *unused, double *out_3401797023306649408) {
  H_27(state, unused, out_3401797023306649408);
}
void car_h_29(double *state, double *unused, double *out_8172217283349668532) {
  h_29(state, unused, out_8172217283349668532);
}
void car_H_29(double *state, double *unused, double *out_6086791679421466503) {
  H_29(state, unused, out_6086791679421466503);
}
void car_h_28(double *state, double *unused, double *out_4946629427370179774) {
  h_28(state, unused, out_4946629427370179774);
}
void car_H_28(double *state, double *unused, double *out_3393964720632432199) {
  H_28(state, unused, out_3393964720632432199);
}
void car_h_31(double *state, double *unused, double *out_3086035387073898256) {
  h_31(state, unused, out_3086035387073898256);
}
void car_H_31(double *state, double *unused, double *out_1309484044507582008) {
  H_31(state, unused, out_1309484044507582008);
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
