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
void car_err_fun(double *nom_x, double *delta_x, double *out_7636272527841291664);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_5731341223820761712);
void car_H_mod_fun(double *state, double *out_5368470991001475643);
void car_f_fun(double *state, double dt, double *out_2692593086736214382);
void car_F_fun(double *state, double dt, double *out_6312543211099338601);
void car_h_25(double *state, double *unused, double *out_2446433457103868726);
void car_H_25(double *state, double *unused, double *out_6581250122818046022);
void car_h_24(double *state, double *unused, double *out_8050236172575186015);
void car_H_24(double *state, double *unused, double *out_6591999667978493000);
void car_h_30(double *state, double *unused, double *out_2171239394819362837);
void car_H_30(double *state, double *unused, double *out_2053553792690437824);
void car_h_26(double *state, double *unused, double *out_1227729541917700402);
void car_H_26(double *state, double *unused, double *out_2839746803943989798);
void car_h_27(double *state, double *unused, double *out_8997547912812617111);
void car_H_27(double *state, double *unused, double *out_121209519109987087);
void car_h_29(double *state, double *unused, double *out_8840361244461487966);
void car_H_29(double *state, double *unused, double *out_2563785137004830008);
void car_h_28(double *state, double *unused, double *out_864791975821295959);
void car_H_28(double *state, double *unused, double *out_2518613880064700566);
void car_h_31(double *state, double *unused, double *out_2289246788752739581);
void car_H_31(double *state, double *unused, double *out_2213538701710638322);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}