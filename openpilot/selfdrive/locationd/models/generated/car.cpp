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
void err_fun(double *nom_x, double *delta_x, double *out_343355515465039342) {
   out_343355515465039342[0] = delta_x[0] + nom_x[0];
   out_343355515465039342[1] = delta_x[1] + nom_x[1];
   out_343355515465039342[2] = delta_x[2] + nom_x[2];
   out_343355515465039342[3] = delta_x[3] + nom_x[3];
   out_343355515465039342[4] = delta_x[4] + nom_x[4];
   out_343355515465039342[5] = delta_x[5] + nom_x[5];
   out_343355515465039342[6] = delta_x[6] + nom_x[6];
   out_343355515465039342[7] = delta_x[7] + nom_x[7];
   out_343355515465039342[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3283826981336380955) {
   out_3283826981336380955[0] = -nom_x[0] + true_x[0];
   out_3283826981336380955[1] = -nom_x[1] + true_x[1];
   out_3283826981336380955[2] = -nom_x[2] + true_x[2];
   out_3283826981336380955[3] = -nom_x[3] + true_x[3];
   out_3283826981336380955[4] = -nom_x[4] + true_x[4];
   out_3283826981336380955[5] = -nom_x[5] + true_x[5];
   out_3283826981336380955[6] = -nom_x[6] + true_x[6];
   out_3283826981336380955[7] = -nom_x[7] + true_x[7];
   out_3283826981336380955[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_8533822980629121439) {
   out_8533822980629121439[0] = 1.0;
   out_8533822980629121439[1] = 0.0;
   out_8533822980629121439[2] = 0.0;
   out_8533822980629121439[3] = 0.0;
   out_8533822980629121439[4] = 0.0;
   out_8533822980629121439[5] = 0.0;
   out_8533822980629121439[6] = 0.0;
   out_8533822980629121439[7] = 0.0;
   out_8533822980629121439[8] = 0.0;
   out_8533822980629121439[9] = 0.0;
   out_8533822980629121439[10] = 1.0;
   out_8533822980629121439[11] = 0.0;
   out_8533822980629121439[12] = 0.0;
   out_8533822980629121439[13] = 0.0;
   out_8533822980629121439[14] = 0.0;
   out_8533822980629121439[15] = 0.0;
   out_8533822980629121439[16] = 0.0;
   out_8533822980629121439[17] = 0.0;
   out_8533822980629121439[18] = 0.0;
   out_8533822980629121439[19] = 0.0;
   out_8533822980629121439[20] = 1.0;
   out_8533822980629121439[21] = 0.0;
   out_8533822980629121439[22] = 0.0;
   out_8533822980629121439[23] = 0.0;
   out_8533822980629121439[24] = 0.0;
   out_8533822980629121439[25] = 0.0;
   out_8533822980629121439[26] = 0.0;
   out_8533822980629121439[27] = 0.0;
   out_8533822980629121439[28] = 0.0;
   out_8533822980629121439[29] = 0.0;
   out_8533822980629121439[30] = 1.0;
   out_8533822980629121439[31] = 0.0;
   out_8533822980629121439[32] = 0.0;
   out_8533822980629121439[33] = 0.0;
   out_8533822980629121439[34] = 0.0;
   out_8533822980629121439[35] = 0.0;
   out_8533822980629121439[36] = 0.0;
   out_8533822980629121439[37] = 0.0;
   out_8533822980629121439[38] = 0.0;
   out_8533822980629121439[39] = 0.0;
   out_8533822980629121439[40] = 1.0;
   out_8533822980629121439[41] = 0.0;
   out_8533822980629121439[42] = 0.0;
   out_8533822980629121439[43] = 0.0;
   out_8533822980629121439[44] = 0.0;
   out_8533822980629121439[45] = 0.0;
   out_8533822980629121439[46] = 0.0;
   out_8533822980629121439[47] = 0.0;
   out_8533822980629121439[48] = 0.0;
   out_8533822980629121439[49] = 0.0;
   out_8533822980629121439[50] = 1.0;
   out_8533822980629121439[51] = 0.0;
   out_8533822980629121439[52] = 0.0;
   out_8533822980629121439[53] = 0.0;
   out_8533822980629121439[54] = 0.0;
   out_8533822980629121439[55] = 0.0;
   out_8533822980629121439[56] = 0.0;
   out_8533822980629121439[57] = 0.0;
   out_8533822980629121439[58] = 0.0;
   out_8533822980629121439[59] = 0.0;
   out_8533822980629121439[60] = 1.0;
   out_8533822980629121439[61] = 0.0;
   out_8533822980629121439[62] = 0.0;
   out_8533822980629121439[63] = 0.0;
   out_8533822980629121439[64] = 0.0;
   out_8533822980629121439[65] = 0.0;
   out_8533822980629121439[66] = 0.0;
   out_8533822980629121439[67] = 0.0;
   out_8533822980629121439[68] = 0.0;
   out_8533822980629121439[69] = 0.0;
   out_8533822980629121439[70] = 1.0;
   out_8533822980629121439[71] = 0.0;
   out_8533822980629121439[72] = 0.0;
   out_8533822980629121439[73] = 0.0;
   out_8533822980629121439[74] = 0.0;
   out_8533822980629121439[75] = 0.0;
   out_8533822980629121439[76] = 0.0;
   out_8533822980629121439[77] = 0.0;
   out_8533822980629121439[78] = 0.0;
   out_8533822980629121439[79] = 0.0;
   out_8533822980629121439[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_9046901037257394716) {
   out_9046901037257394716[0] = state[0];
   out_9046901037257394716[1] = state[1];
   out_9046901037257394716[2] = state[2];
   out_9046901037257394716[3] = state[3];
   out_9046901037257394716[4] = state[4];
   out_9046901037257394716[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_9046901037257394716[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_9046901037257394716[7] = state[7];
   out_9046901037257394716[8] = state[8];
}
void F_fun(double *state, double dt, double *out_576595726729544398) {
   out_576595726729544398[0] = 1;
   out_576595726729544398[1] = 0;
   out_576595726729544398[2] = 0;
   out_576595726729544398[3] = 0;
   out_576595726729544398[4] = 0;
   out_576595726729544398[5] = 0;
   out_576595726729544398[6] = 0;
   out_576595726729544398[7] = 0;
   out_576595726729544398[8] = 0;
   out_576595726729544398[9] = 0;
   out_576595726729544398[10] = 1;
   out_576595726729544398[11] = 0;
   out_576595726729544398[12] = 0;
   out_576595726729544398[13] = 0;
   out_576595726729544398[14] = 0;
   out_576595726729544398[15] = 0;
   out_576595726729544398[16] = 0;
   out_576595726729544398[17] = 0;
   out_576595726729544398[18] = 0;
   out_576595726729544398[19] = 0;
   out_576595726729544398[20] = 1;
   out_576595726729544398[21] = 0;
   out_576595726729544398[22] = 0;
   out_576595726729544398[23] = 0;
   out_576595726729544398[24] = 0;
   out_576595726729544398[25] = 0;
   out_576595726729544398[26] = 0;
   out_576595726729544398[27] = 0;
   out_576595726729544398[28] = 0;
   out_576595726729544398[29] = 0;
   out_576595726729544398[30] = 1;
   out_576595726729544398[31] = 0;
   out_576595726729544398[32] = 0;
   out_576595726729544398[33] = 0;
   out_576595726729544398[34] = 0;
   out_576595726729544398[35] = 0;
   out_576595726729544398[36] = 0;
   out_576595726729544398[37] = 0;
   out_576595726729544398[38] = 0;
   out_576595726729544398[39] = 0;
   out_576595726729544398[40] = 1;
   out_576595726729544398[41] = 0;
   out_576595726729544398[42] = 0;
   out_576595726729544398[43] = 0;
   out_576595726729544398[44] = 0;
   out_576595726729544398[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_576595726729544398[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_576595726729544398[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_576595726729544398[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_576595726729544398[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_576595726729544398[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_576595726729544398[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_576595726729544398[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_576595726729544398[53] = -9.8100000000000005*dt;
   out_576595726729544398[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_576595726729544398[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_576595726729544398[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_576595726729544398[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_576595726729544398[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_576595726729544398[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_576595726729544398[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_576595726729544398[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_576595726729544398[62] = 0;
   out_576595726729544398[63] = 0;
   out_576595726729544398[64] = 0;
   out_576595726729544398[65] = 0;
   out_576595726729544398[66] = 0;
   out_576595726729544398[67] = 0;
   out_576595726729544398[68] = 0;
   out_576595726729544398[69] = 0;
   out_576595726729544398[70] = 1;
   out_576595726729544398[71] = 0;
   out_576595726729544398[72] = 0;
   out_576595726729544398[73] = 0;
   out_576595726729544398[74] = 0;
   out_576595726729544398[75] = 0;
   out_576595726729544398[76] = 0;
   out_576595726729544398[77] = 0;
   out_576595726729544398[78] = 0;
   out_576595726729544398[79] = 0;
   out_576595726729544398[80] = 1;
}
void h_25(double *state, double *unused, double *out_328219758125700353) {
   out_328219758125700353[0] = state[6];
}
void H_25(double *state, double *unused, double *out_5010842330639629762) {
   out_5010842330639629762[0] = 0;
   out_5010842330639629762[1] = 0;
   out_5010842330639629762[2] = 0;
   out_5010842330639629762[3] = 0;
   out_5010842330639629762[4] = 0;
   out_5010842330639629762[5] = 0;
   out_5010842330639629762[6] = 1;
   out_5010842330639629762[7] = 0;
   out_5010842330639629762[8] = 0;
}
void h_24(double *state, double *unused, double *out_2024779726678239199) {
   out_2024779726678239199[0] = state[4];
   out_2024779726678239199[1] = state[5];
}
void H_24(double *state, double *unused, double *out_8689392416103412820) {
   out_8689392416103412820[0] = 0;
   out_8689392416103412820[1] = 0;
   out_8689392416103412820[2] = 0;
   out_8689392416103412820[3] = 0;
   out_8689392416103412820[4] = 1;
   out_8689392416103412820[5] = 0;
   out_8689392416103412820[6] = 0;
   out_8689392416103412820[7] = 0;
   out_8689392416103412820[8] = 0;
   out_8689392416103412820[9] = 0;
   out_8689392416103412820[10] = 0;
   out_8689392416103412820[11] = 0;
   out_8689392416103412820[12] = 0;
   out_8689392416103412820[13] = 0;
   out_8689392416103412820[14] = 1;
   out_8689392416103412820[15] = 0;
   out_8689392416103412820[16] = 0;
   out_8689392416103412820[17] = 0;
}
void h_30(double *state, double *unused, double *out_3965669286379722703) {
   out_3965669286379722703[0] = state[4];
}
void H_30(double *state, double *unused, double *out_2492509372132381135) {
   out_2492509372132381135[0] = 0;
   out_2492509372132381135[1] = 0;
   out_2492509372132381135[2] = 0;
   out_2492509372132381135[3] = 0;
   out_2492509372132381135[4] = 1;
   out_2492509372132381135[5] = 0;
   out_2492509372132381135[6] = 0;
   out_2492509372132381135[7] = 0;
   out_2492509372132381135[8] = 0;
}
void h_26(double *state, double *unused, double *out_2826573687252009076) {
   out_2826573687252009076[0] = state[7];
}
void H_26(double *state, double *unused, double *out_8752345649513685986) {
   out_8752345649513685986[0] = 0;
   out_8752345649513685986[1] = 0;
   out_8752345649513685986[2] = 0;
   out_8752345649513685986[3] = 0;
   out_8752345649513685986[4] = 0;
   out_8752345649513685986[5] = 0;
   out_8752345649513685986[6] = 0;
   out_8752345649513685986[7] = 1;
   out_8752345649513685986[8] = 0;
}
void h_27(double *state, double *unused, double *out_2708566293318632332) {
   out_2708566293318632332[0] = state[3];
}
void H_27(double *state, double *unused, double *out_7314944589583294743) {
   out_7314944589583294743[0] = 0;
   out_7314944589583294743[1] = 0;
   out_7314944589583294743[2] = 0;
   out_7314944589583294743[3] = 1;
   out_7314944589583294743[4] = 0;
   out_7314944589583294743[5] = 0;
   out_7314944589583294743[6] = 0;
   out_7314944589583294743[7] = 0;
   out_7314944589583294743[8] = 0;
}
void h_29(double *state, double *unused, double *out_8967342554040568348) {
   out_8967342554040568348[0] = state[1];
}
void H_29(double *state, double *unused, double *out_9028307316452845776) {
   out_9028307316452845776[0] = 0;
   out_9028307316452845776[1] = 1;
   out_9028307316452845776[2] = 0;
   out_9028307316452845776[3] = 0;
   out_9028307316452845776[4] = 0;
   out_9028307316452845776[5] = 0;
   out_9028307316452845776[6] = 0;
   out_9028307316452845776[7] = 0;
   out_9028307316452845776[8] = 0;
}
void h_28(double *state, double *unused, double *out_1689385787962996680) {
   out_1689385787962996680[0] = state[0];
}
void H_28(double *state, double *unused, double *out_7064677044887519525) {
   out_7064677044887519525[0] = 1;
   out_7064677044887519525[1] = 0;
   out_7064677044887519525[2] = 0;
   out_7064677044887519525[3] = 0;
   out_7064677044887519525[4] = 0;
   out_7064677044887519525[5] = 0;
   out_7064677044887519525[6] = 0;
   out_7064677044887519525[7] = 0;
   out_7064677044887519525[8] = 0;
}
void h_31(double *state, double *unused, double *out_53025695841194464) {
   out_53025695841194464[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4980196368762669334) {
   out_4980196368762669334[0] = 0;
   out_4980196368762669334[1] = 0;
   out_4980196368762669334[2] = 0;
   out_4980196368762669334[3] = 0;
   out_4980196368762669334[4] = 0;
   out_4980196368762669334[5] = 0;
   out_4980196368762669334[6] = 0;
   out_4980196368762669334[7] = 0;
   out_4980196368762669334[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_343355515465039342) {
  err_fun(nom_x, delta_x, out_343355515465039342);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3283826981336380955) {
  inv_err_fun(nom_x, true_x, out_3283826981336380955);
}
void car_H_mod_fun(double *state, double *out_8533822980629121439) {
  H_mod_fun(state, out_8533822980629121439);
}
void car_f_fun(double *state, double dt, double *out_9046901037257394716) {
  f_fun(state,  dt, out_9046901037257394716);
}
void car_F_fun(double *state, double dt, double *out_576595726729544398) {
  F_fun(state,  dt, out_576595726729544398);
}
void car_h_25(double *state, double *unused, double *out_328219758125700353) {
  h_25(state, unused, out_328219758125700353);
}
void car_H_25(double *state, double *unused, double *out_5010842330639629762) {
  H_25(state, unused, out_5010842330639629762);
}
void car_h_24(double *state, double *unused, double *out_2024779726678239199) {
  h_24(state, unused, out_2024779726678239199);
}
void car_H_24(double *state, double *unused, double *out_8689392416103412820) {
  H_24(state, unused, out_8689392416103412820);
}
void car_h_30(double *state, double *unused, double *out_3965669286379722703) {
  h_30(state, unused, out_3965669286379722703);
}
void car_H_30(double *state, double *unused, double *out_2492509372132381135) {
  H_30(state, unused, out_2492509372132381135);
}
void car_h_26(double *state, double *unused, double *out_2826573687252009076) {
  h_26(state, unused, out_2826573687252009076);
}
void car_H_26(double *state, double *unused, double *out_8752345649513685986) {
  H_26(state, unused, out_8752345649513685986);
}
void car_h_27(double *state, double *unused, double *out_2708566293318632332) {
  h_27(state, unused, out_2708566293318632332);
}
void car_H_27(double *state, double *unused, double *out_7314944589583294743) {
  H_27(state, unused, out_7314944589583294743);
}
void car_h_29(double *state, double *unused, double *out_8967342554040568348) {
  h_29(state, unused, out_8967342554040568348);
}
void car_H_29(double *state, double *unused, double *out_9028307316452845776) {
  H_29(state, unused, out_9028307316452845776);
}
void car_h_28(double *state, double *unused, double *out_1689385787962996680) {
  h_28(state, unused, out_1689385787962996680);
}
void car_H_28(double *state, double *unused, double *out_7064677044887519525) {
  H_28(state, unused, out_7064677044887519525);
}
void car_h_31(double *state, double *unused, double *out_53025695841194464) {
  h_31(state, unused, out_53025695841194464);
}
void car_H_31(double *state, double *unused, double *out_4980196368762669334) {
  H_31(state, unused, out_4980196368762669334);
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
