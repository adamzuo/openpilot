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
void err_fun(double *nom_x, double *delta_x, double *out_4282015564213134329) {
   out_4282015564213134329[0] = delta_x[0] + nom_x[0];
   out_4282015564213134329[1] = delta_x[1] + nom_x[1];
   out_4282015564213134329[2] = delta_x[2] + nom_x[2];
   out_4282015564213134329[3] = delta_x[3] + nom_x[3];
   out_4282015564213134329[4] = delta_x[4] + nom_x[4];
   out_4282015564213134329[5] = delta_x[5] + nom_x[5];
   out_4282015564213134329[6] = delta_x[6] + nom_x[6];
   out_4282015564213134329[7] = delta_x[7] + nom_x[7];
   out_4282015564213134329[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_8415782197654491238) {
   out_8415782197654491238[0] = -nom_x[0] + true_x[0];
   out_8415782197654491238[1] = -nom_x[1] + true_x[1];
   out_8415782197654491238[2] = -nom_x[2] + true_x[2];
   out_8415782197654491238[3] = -nom_x[3] + true_x[3];
   out_8415782197654491238[4] = -nom_x[4] + true_x[4];
   out_8415782197654491238[5] = -nom_x[5] + true_x[5];
   out_8415782197654491238[6] = -nom_x[6] + true_x[6];
   out_8415782197654491238[7] = -nom_x[7] + true_x[7];
   out_8415782197654491238[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6430858302346550466) {
   out_6430858302346550466[0] = 1.0;
   out_6430858302346550466[1] = 0.0;
   out_6430858302346550466[2] = 0.0;
   out_6430858302346550466[3] = 0.0;
   out_6430858302346550466[4] = 0.0;
   out_6430858302346550466[5] = 0.0;
   out_6430858302346550466[6] = 0.0;
   out_6430858302346550466[7] = 0.0;
   out_6430858302346550466[8] = 0.0;
   out_6430858302346550466[9] = 0.0;
   out_6430858302346550466[10] = 1.0;
   out_6430858302346550466[11] = 0.0;
   out_6430858302346550466[12] = 0.0;
   out_6430858302346550466[13] = 0.0;
   out_6430858302346550466[14] = 0.0;
   out_6430858302346550466[15] = 0.0;
   out_6430858302346550466[16] = 0.0;
   out_6430858302346550466[17] = 0.0;
   out_6430858302346550466[18] = 0.0;
   out_6430858302346550466[19] = 0.0;
   out_6430858302346550466[20] = 1.0;
   out_6430858302346550466[21] = 0.0;
   out_6430858302346550466[22] = 0.0;
   out_6430858302346550466[23] = 0.0;
   out_6430858302346550466[24] = 0.0;
   out_6430858302346550466[25] = 0.0;
   out_6430858302346550466[26] = 0.0;
   out_6430858302346550466[27] = 0.0;
   out_6430858302346550466[28] = 0.0;
   out_6430858302346550466[29] = 0.0;
   out_6430858302346550466[30] = 1.0;
   out_6430858302346550466[31] = 0.0;
   out_6430858302346550466[32] = 0.0;
   out_6430858302346550466[33] = 0.0;
   out_6430858302346550466[34] = 0.0;
   out_6430858302346550466[35] = 0.0;
   out_6430858302346550466[36] = 0.0;
   out_6430858302346550466[37] = 0.0;
   out_6430858302346550466[38] = 0.0;
   out_6430858302346550466[39] = 0.0;
   out_6430858302346550466[40] = 1.0;
   out_6430858302346550466[41] = 0.0;
   out_6430858302346550466[42] = 0.0;
   out_6430858302346550466[43] = 0.0;
   out_6430858302346550466[44] = 0.0;
   out_6430858302346550466[45] = 0.0;
   out_6430858302346550466[46] = 0.0;
   out_6430858302346550466[47] = 0.0;
   out_6430858302346550466[48] = 0.0;
   out_6430858302346550466[49] = 0.0;
   out_6430858302346550466[50] = 1.0;
   out_6430858302346550466[51] = 0.0;
   out_6430858302346550466[52] = 0.0;
   out_6430858302346550466[53] = 0.0;
   out_6430858302346550466[54] = 0.0;
   out_6430858302346550466[55] = 0.0;
   out_6430858302346550466[56] = 0.0;
   out_6430858302346550466[57] = 0.0;
   out_6430858302346550466[58] = 0.0;
   out_6430858302346550466[59] = 0.0;
   out_6430858302346550466[60] = 1.0;
   out_6430858302346550466[61] = 0.0;
   out_6430858302346550466[62] = 0.0;
   out_6430858302346550466[63] = 0.0;
   out_6430858302346550466[64] = 0.0;
   out_6430858302346550466[65] = 0.0;
   out_6430858302346550466[66] = 0.0;
   out_6430858302346550466[67] = 0.0;
   out_6430858302346550466[68] = 0.0;
   out_6430858302346550466[69] = 0.0;
   out_6430858302346550466[70] = 1.0;
   out_6430858302346550466[71] = 0.0;
   out_6430858302346550466[72] = 0.0;
   out_6430858302346550466[73] = 0.0;
   out_6430858302346550466[74] = 0.0;
   out_6430858302346550466[75] = 0.0;
   out_6430858302346550466[76] = 0.0;
   out_6430858302346550466[77] = 0.0;
   out_6430858302346550466[78] = 0.0;
   out_6430858302346550466[79] = 0.0;
   out_6430858302346550466[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1318014898722362373) {
   out_1318014898722362373[0] = state[0];
   out_1318014898722362373[1] = state[1];
   out_1318014898722362373[2] = state[2];
   out_1318014898722362373[3] = state[3];
   out_1318014898722362373[4] = state[4];
   out_1318014898722362373[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1318014898722362373[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1318014898722362373[7] = state[7];
   out_1318014898722362373[8] = state[8];
}
void F_fun(double *state, double dt, double *out_7446125825375545094) {
   out_7446125825375545094[0] = 1;
   out_7446125825375545094[1] = 0;
   out_7446125825375545094[2] = 0;
   out_7446125825375545094[3] = 0;
   out_7446125825375545094[4] = 0;
   out_7446125825375545094[5] = 0;
   out_7446125825375545094[6] = 0;
   out_7446125825375545094[7] = 0;
   out_7446125825375545094[8] = 0;
   out_7446125825375545094[9] = 0;
   out_7446125825375545094[10] = 1;
   out_7446125825375545094[11] = 0;
   out_7446125825375545094[12] = 0;
   out_7446125825375545094[13] = 0;
   out_7446125825375545094[14] = 0;
   out_7446125825375545094[15] = 0;
   out_7446125825375545094[16] = 0;
   out_7446125825375545094[17] = 0;
   out_7446125825375545094[18] = 0;
   out_7446125825375545094[19] = 0;
   out_7446125825375545094[20] = 1;
   out_7446125825375545094[21] = 0;
   out_7446125825375545094[22] = 0;
   out_7446125825375545094[23] = 0;
   out_7446125825375545094[24] = 0;
   out_7446125825375545094[25] = 0;
   out_7446125825375545094[26] = 0;
   out_7446125825375545094[27] = 0;
   out_7446125825375545094[28] = 0;
   out_7446125825375545094[29] = 0;
   out_7446125825375545094[30] = 1;
   out_7446125825375545094[31] = 0;
   out_7446125825375545094[32] = 0;
   out_7446125825375545094[33] = 0;
   out_7446125825375545094[34] = 0;
   out_7446125825375545094[35] = 0;
   out_7446125825375545094[36] = 0;
   out_7446125825375545094[37] = 0;
   out_7446125825375545094[38] = 0;
   out_7446125825375545094[39] = 0;
   out_7446125825375545094[40] = 1;
   out_7446125825375545094[41] = 0;
   out_7446125825375545094[42] = 0;
   out_7446125825375545094[43] = 0;
   out_7446125825375545094[44] = 0;
   out_7446125825375545094[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_7446125825375545094[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_7446125825375545094[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7446125825375545094[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_7446125825375545094[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_7446125825375545094[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_7446125825375545094[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_7446125825375545094[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_7446125825375545094[53] = -9.8100000000000005*dt;
   out_7446125825375545094[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_7446125825375545094[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_7446125825375545094[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7446125825375545094[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7446125825375545094[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_7446125825375545094[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_7446125825375545094[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_7446125825375545094[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_7446125825375545094[62] = 0;
   out_7446125825375545094[63] = 0;
   out_7446125825375545094[64] = 0;
   out_7446125825375545094[65] = 0;
   out_7446125825375545094[66] = 0;
   out_7446125825375545094[67] = 0;
   out_7446125825375545094[68] = 0;
   out_7446125825375545094[69] = 0;
   out_7446125825375545094[70] = 1;
   out_7446125825375545094[71] = 0;
   out_7446125825375545094[72] = 0;
   out_7446125825375545094[73] = 0;
   out_7446125825375545094[74] = 0;
   out_7446125825375545094[75] = 0;
   out_7446125825375545094[76] = 0;
   out_7446125825375545094[77] = 0;
   out_7446125825375545094[78] = 0;
   out_7446125825375545094[79] = 0;
   out_7446125825375545094[80] = 1;
}
void h_25(double *state, double *unused, double *out_8567843834345653255) {
   out_8567843834345653255[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4362333016585837710) {
   out_4362333016585837710[0] = 0;
   out_4362333016585837710[1] = 0;
   out_4362333016585837710[2] = 0;
   out_4362333016585837710[3] = 0;
   out_4362333016585837710[4] = 0;
   out_4362333016585837710[5] = 0;
   out_4362333016585837710[6] = 1;
   out_4362333016585837710[7] = 0;
   out_4362333016585837710[8] = 0;
}
void h_24(double *state, double *unused, double *out_8048913789316591682) {
   out_8048913789316591682[0] = state[4];
   out_8048913789316591682[1] = state[5];
}
void H_24(double *state, double *unused, double *out_7508839250532195805) {
   out_7508839250532195805[0] = 0;
   out_7508839250532195805[1] = 0;
   out_7508839250532195805[2] = 0;
   out_7508839250532195805[3] = 0;
   out_7508839250532195805[4] = 1;
   out_7508839250532195805[5] = 0;
   out_7508839250532195805[6] = 0;
   out_7508839250532195805[7] = 0;
   out_7508839250532195805[8] = 0;
   out_7508839250532195805[9] = 0;
   out_7508839250532195805[10] = 0;
   out_7508839250532195805[11] = 0;
   out_7508839250532195805[12] = 0;
   out_7508839250532195805[13] = 0;
   out_7508839250532195805[14] = 1;
   out_7508839250532195805[15] = 0;
   out_7508839250532195805[16] = 0;
   out_7508839250532195805[17] = 0;
}
void h_30(double *state, double *unused, double *out_2413745266717556569) {
   out_2413745266717556569[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6880665975093086337) {
   out_6880665975093086337[0] = 0;
   out_6880665975093086337[1] = 0;
   out_6880665975093086337[2] = 0;
   out_6880665975093086337[3] = 0;
   out_6880665975093086337[4] = 1;
   out_6880665975093086337[5] = 0;
   out_6880665975093086337[6] = 0;
   out_6880665975093086337[7] = 0;
   out_6880665975093086337[8] = 0;
}
void h_26(double *state, double *unused, double *out_1054117735708186741) {
   out_1054117735708186741[0] = state[7];
}
void H_26(double *state, double *unused, double *out_620829697711781486) {
   out_620829697711781486[0] = 0;
   out_620829697711781486[1] = 0;
   out_620829697711781486[2] = 0;
   out_620829697711781486[3] = 0;
   out_620829697711781486[4] = 0;
   out_620829697711781486[5] = 0;
   out_620829697711781486[6] = 0;
   out_620829697711781486[7] = 1;
   out_620829697711781486[8] = 0;
}
void h_27(double *state, double *unused, double *out_4877226971703052716) {
   out_4877226971703052716[0] = state[3];
}
void H_27(double *state, double *unused, double *out_9104260046277029554) {
   out_9104260046277029554[0] = 0;
   out_9104260046277029554[1] = 0;
   out_9104260046277029554[2] = 0;
   out_9104260046277029554[3] = 1;
   out_9104260046277029554[4] = 0;
   out_9104260046277029554[5] = 0;
   out_9104260046277029554[6] = 0;
   out_9104260046277029554[7] = 0;
   out_9104260046277029554[8] = 0;
}
void h_29(double *state, double *unused, double *out_5034413640054181861) {
   out_5034413640054181861[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7390897319407478521) {
   out_7390897319407478521[0] = 0;
   out_7390897319407478521[1] = 1;
   out_7390897319407478521[2] = 0;
   out_7390897319407478521[3] = 0;
   out_7390897319407478521[4] = 0;
   out_7390897319407478521[5] = 0;
   out_7390897319407478521[6] = 0;
   out_7390897319407478521[7] = 0;
   out_7390897319407478521[8] = 0;
}
void h_28(double *state, double *unused, double *out_6358607615752277389) {
   out_6358607615752277389[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2308498302337947947) {
   out_2308498302337947947[0] = 1;
   out_2308498302337947947[1] = 0;
   out_2308498302337947947[2] = 0;
   out_2308498302337947947[3] = 0;
   out_2308498302337947947[4] = 0;
   out_2308498302337947947[5] = 0;
   out_2308498302337947947[6] = 0;
   out_2308498302337947947[7] = 0;
   out_2308498302337947947[8] = 0;
}
void h_31(double *state, double *unused, double *out_4930394306091630905) {
   out_4930394306091630905[0] = state[8];
}
void H_31(double *state, double *unused, double *out_4392978978462798138) {
   out_4392978978462798138[0] = 0;
   out_4392978978462798138[1] = 0;
   out_4392978978462798138[2] = 0;
   out_4392978978462798138[3] = 0;
   out_4392978978462798138[4] = 0;
   out_4392978978462798138[5] = 0;
   out_4392978978462798138[6] = 0;
   out_4392978978462798138[7] = 0;
   out_4392978978462798138[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_4282015564213134329) {
  err_fun(nom_x, delta_x, out_4282015564213134329);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8415782197654491238) {
  inv_err_fun(nom_x, true_x, out_8415782197654491238);
}
void car_H_mod_fun(double *state, double *out_6430858302346550466) {
  H_mod_fun(state, out_6430858302346550466);
}
void car_f_fun(double *state, double dt, double *out_1318014898722362373) {
  f_fun(state,  dt, out_1318014898722362373);
}
void car_F_fun(double *state, double dt, double *out_7446125825375545094) {
  F_fun(state,  dt, out_7446125825375545094);
}
void car_h_25(double *state, double *unused, double *out_8567843834345653255) {
  h_25(state, unused, out_8567843834345653255);
}
void car_H_25(double *state, double *unused, double *out_4362333016585837710) {
  H_25(state, unused, out_4362333016585837710);
}
void car_h_24(double *state, double *unused, double *out_8048913789316591682) {
  h_24(state, unused, out_8048913789316591682);
}
void car_H_24(double *state, double *unused, double *out_7508839250532195805) {
  H_24(state, unused, out_7508839250532195805);
}
void car_h_30(double *state, double *unused, double *out_2413745266717556569) {
  h_30(state, unused, out_2413745266717556569);
}
void car_H_30(double *state, double *unused, double *out_6880665975093086337) {
  H_30(state, unused, out_6880665975093086337);
}
void car_h_26(double *state, double *unused, double *out_1054117735708186741) {
  h_26(state, unused, out_1054117735708186741);
}
void car_H_26(double *state, double *unused, double *out_620829697711781486) {
  H_26(state, unused, out_620829697711781486);
}
void car_h_27(double *state, double *unused, double *out_4877226971703052716) {
  h_27(state, unused, out_4877226971703052716);
}
void car_H_27(double *state, double *unused, double *out_9104260046277029554) {
  H_27(state, unused, out_9104260046277029554);
}
void car_h_29(double *state, double *unused, double *out_5034413640054181861) {
  h_29(state, unused, out_5034413640054181861);
}
void car_H_29(double *state, double *unused, double *out_7390897319407478521) {
  H_29(state, unused, out_7390897319407478521);
}
void car_h_28(double *state, double *unused, double *out_6358607615752277389) {
  h_28(state, unused, out_6358607615752277389);
}
void car_H_28(double *state, double *unused, double *out_2308498302337947947) {
  H_28(state, unused, out_2308498302337947947);
}
void car_h_31(double *state, double *unused, double *out_4930394306091630905) {
  h_31(state, unused, out_4930394306091630905);
}
void car_H_31(double *state, double *unused, double *out_4392978978462798138) {
  H_31(state, unused, out_4392978978462798138);
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
