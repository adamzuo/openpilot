#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6902255537984290412);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_971548101061258492);
void pose_H_mod_fun(double *state, double *out_5872156854311632266);
void pose_f_fun(double *state, double dt, double *out_7853950092549063317);
void pose_F_fun(double *state, double dt, double *out_2347128049005111238);
void pose_h_4(double *state, double *unused, double *out_5113645833906790079);
void pose_H_4(double *state, double *unused, double *out_2671746953210162242);
void pose_h_10(double *state, double *unused, double *out_5157045351269012104);
void pose_H_10(double *state, double *unused, double *out_6967721430408486920);
void pose_h_13(double *state, double *unused, double *out_488268826700853092);
void pose_H_13(double *state, double *unused, double *out_8164365912182688445);
void pose_h_14(double *state, double *unused, double *out_2240330411018770776);
void pose_H_14(double *state, double *unused, double *out_411041479085210054);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}