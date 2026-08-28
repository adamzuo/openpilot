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
void car_err_fun(double *nom_x, double *delta_x, double *out_4282015564213134329);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8415782197654491238);
void car_H_mod_fun(double *state, double *out_6430858302346550466);
void car_f_fun(double *state, double dt, double *out_1318014898722362373);
void car_F_fun(double *state, double dt, double *out_7446125825375545094);
void car_h_25(double *state, double *unused, double *out_8567843834345653255);
void car_H_25(double *state, double *unused, double *out_4362333016585837710);
void car_h_24(double *state, double *unused, double *out_8048913789316591682);
void car_H_24(double *state, double *unused, double *out_7508839250532195805);
void car_h_30(double *state, double *unused, double *out_2413745266717556569);
void car_H_30(double *state, double *unused, double *out_6880665975093086337);
void car_h_26(double *state, double *unused, double *out_1054117735708186741);
void car_H_26(double *state, double *unused, double *out_620829697711781486);
void car_h_27(double *state, double *unused, double *out_4877226971703052716);
void car_H_27(double *state, double *unused, double *out_9104260046277029554);
void car_h_29(double *state, double *unused, double *out_5034413640054181861);
void car_H_29(double *state, double *unused, double *out_7390897319407478521);
void car_h_28(double *state, double *unused, double *out_6358607615752277389);
void car_H_28(double *state, double *unused, double *out_2308498302337947947);
void car_h_31(double *state, double *unused, double *out_4930394306091630905);
void car_H_31(double *state, double *unused, double *out_4392978978462798138);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}