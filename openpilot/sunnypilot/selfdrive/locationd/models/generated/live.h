#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_4290060173839192873);
void live_err_fun(double *nom_x, double *delta_x, double *out_597782692424270358);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_698997895825241176);
void live_H_mod_fun(double *state, double *out_4524690988516914898);
void live_f_fun(double *state, double dt, double *out_7171721672199995198);
void live_F_fun(double *state, double dt, double *out_8519973484240705121);
void live_h_4(double *state, double *unused, double *out_762393658119905251);
void live_H_4(double *state, double *unused, double *out_5484012933447415813);
void live_h_9(double *state, double *unused, double *out_9078649821707292052);
void live_H_9(double *state, double *unused, double *out_5675512204997688333);
void live_h_10(double *state, double *unused, double *out_6636031549443107836);
void live_H_10(double *state, double *unused, double *out_4872550888805470838);
void live_h_12(double *state, double *unused, double *out_2728399981377051059);
void live_H_12(double *state, double *unused, double *out_5295602826579685311);
void live_h_35(double *state, double *unused, double *out_4585494250683371816);
void live_H_35(double *state, double *unused, double *out_8850674990820023189);
void live_h_32(double *state, double *unused, double *out_6932262811838659742);
void live_H_32(double *state, double *unused, double *out_7655230745556519626);
void live_h_13(double *state, double *unused, double *out_8672065483642251228);
void live_H_13(double *state, double *unused, double *out_8714949002529358183);
void live_h_14(double *state, double *unused, double *out_9078649821707292052);
void live_H_14(double *state, double *unused, double *out_5675512204997688333);
void live_h_33(double *state, double *unused, double *out_6721134624350617004);
void live_H_33(double *state, double *unused, double *out_6445512078250670823);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}