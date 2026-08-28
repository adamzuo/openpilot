#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_3260711326014191875);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3461417183998723496);
void car_H_mod_fun(double *state, double *out_728395279376170320);
void car_f_fun(double *state, double dt, double *out_7241043703078136547);
void car_F_fun(double *state, double dt, double *out_1989643418376622314);
void car_h_25(double *state, double *unused, double *out_888466012610996564);
void car_H_25(double *state, double *unused, double *out_1340130006384542436);
void car_h_24(double *state, double *unused, double *out_2820570096846580141);
void car_H_24(double *state, double *unused, double *out_5235441800206975665);
void car_h_30(double *state, double *unused, double *out_1033985836831651535);
void car_H_30(double *state, double *unused, double *out_5576560335107074319);
void car_h_26(double *state, double *unused, double *out_4191921379003673412);
void car_H_26(double *state, double *unused, double *out_5081633325258598660);
void car_h_27(double *state, double *unused, double *out_6637077262105860734);
void car_H_27(double *state, double *unused, double *out_3401797023306649408);
void car_h_29(double *state, double *unused, double *out_8172217283349668532);
void car_H_29(double *state, double *unused, double *out_6086791679421466503);
void car_h_28(double *state, double *unused, double *out_4946629427370179774);
void car_H_28(double *state, double *unused, double *out_3393964720632432199);
void car_h_31(double *state, double *unused, double *out_3086035387073898256);
void car_H_31(double *state, double *unused, double *out_1309484044507582008);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}