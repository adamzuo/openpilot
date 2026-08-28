#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8980418967492834945);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5853420954923124138);
void pose_H_mod_fun(double *state, double *out_4198711007403848478);
void pose_f_fun(double *state, double dt, double *out_5112212409069008544);
void pose_F_fun(double *state, double dt, double *out_8268211090171322777);
void pose_h_4(double *state, double *unused, double *out_2358914934415246195);
void pose_H_4(double *state, double *unused, double *out_7472933086194797731);
void pose_h_10(double *state, double *unused, double *out_2019013223460407147);
void pose_H_10(double *state, double *unused, double *out_8707321851081198616);
void pose_h_13(double *state, double *unused, double *out_830035003939239818);
void pose_H_13(double *state, double *unused, double *out_4260659260862464930);
void pose_h_14(double *state, double *unused, double *out_6071747091570746666);
void pose_H_14(double *state, double *unused, double *out_3509692229855313202);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}