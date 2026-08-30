#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8549168006619746742);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5584185916794207075);
void pose_H_mod_fun(double *state, double *out_8554003597594456449);
void pose_f_fun(double *state, double dt, double *out_783971397319598562);
void pose_F_fun(double *state, double dt, double *out_804064847722436257);
void pose_h_4(double *state, double *unused, double *out_4471711811158414269);
void pose_H_4(double *state, double *unused, double *out_184192634488532689);
void pose_h_10(double *state, double *unused, double *out_2904425780055764226);
void pose_H_10(double *state, double *unused, double *out_4999916326087304913);
void pose_h_13(double *state, double *unused, double *out_985319506289674804);
void pose_H_13(double *state, double *unused, double *out_3649562828813991335);
void pose_h_14(double *state, double *unused, double *out_8223911283974111787);
void pose_H_14(double *state, double *unused, double *out_2898595797806839607);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}