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
void car_err_fun(double *nom_x, double *delta_x, double *out_343355515465039342);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3283826981336380955);
void car_H_mod_fun(double *state, double *out_8533822980629121439);
void car_f_fun(double *state, double dt, double *out_9046901037257394716);
void car_F_fun(double *state, double dt, double *out_576595726729544398);
void car_h_25(double *state, double *unused, double *out_328219758125700353);
void car_H_25(double *state, double *unused, double *out_5010842330639629762);
void car_h_24(double *state, double *unused, double *out_2024779726678239199);
void car_H_24(double *state, double *unused, double *out_8689392416103412820);
void car_h_30(double *state, double *unused, double *out_3965669286379722703);
void car_H_30(double *state, double *unused, double *out_2492509372132381135);
void car_h_26(double *state, double *unused, double *out_2826573687252009076);
void car_H_26(double *state, double *unused, double *out_8752345649513685986);
void car_h_27(double *state, double *unused, double *out_2708566293318632332);
void car_H_27(double *state, double *unused, double *out_7314944589583294743);
void car_h_29(double *state, double *unused, double *out_8967342554040568348);
void car_H_29(double *state, double *unused, double *out_9028307316452845776);
void car_h_28(double *state, double *unused, double *out_1689385787962996680);
void car_H_28(double *state, double *unused, double *out_7064677044887519525);
void car_h_31(double *state, double *unused, double *out_53025695841194464);
void car_H_31(double *state, double *unused, double *out_4980196368762669334);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}