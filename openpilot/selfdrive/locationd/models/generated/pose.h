#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_3945029041185085555);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6546093222035050191);
void pose_H_mod_fun(double *state, double *out_6602467143110751234);
void pose_f_fun(double *state, double dt, double *out_4817465423454828778);
void pose_F_fun(double *state, double dt, double *out_2368631440291744086);
void pose_h_4(double *state, double *unused, double *out_6519370834537087993);
void pose_H_4(double *state, double *unused, double *out_8570054851807851129);
void pose_h_10(double *state, double *unused, double *out_5225915248190564975);
void pose_H_10(double *state, double *unused, double *out_8776126540151652793);
void pose_h_13(double *state, double *unused, double *out_5413857800886136922);
void pose_H_13(double *state, double *unused, double *out_2266058013584999558);
void pose_h_14(double *state, double *unused, double *out_4123985795765896642);
void pose_H_14(double *state, double *unused, double *out_5913448365562215958);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}