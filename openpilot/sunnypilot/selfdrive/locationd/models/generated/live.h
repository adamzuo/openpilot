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
void live_H(double *in_vec, double *out_690549402913475273);
void live_err_fun(double *nom_x, double *delta_x, double *out_145896516976045759);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1774710111975529926);
void live_H_mod_fun(double *state, double *out_2528461283964274105);
void live_f_fun(double *state, double dt, double *out_5576874985168186958);
void live_F_fun(double *state, double dt, double *out_3947121473726429245);
void live_h_4(double *state, double *unused, double *out_2480460839671081020);
void live_H_4(double *state, double *unused, double *out_3656684248305877712);
void live_h_9(double *state, double *unused, double *out_7304420017673863233);
void live_H_9(double *state, double *unused, double *out_3897873894935468357);
void live_h_10(double *state, double *unused, double *out_6276961836476962249);
void live_H_10(double *state, double *unused, double *out_5336893548037202121);
void live_h_12(double *state, double *unused, double *out_7697572258396532193);
void live_H_12(double *state, double *unused, double *out_8676140656337839507);
void live_h_35(double *state, double *unused, double *out_8019537204765766364);
void live_H_35(double *state, double *unused, double *out_7023346305678485088);
void live_h_32(double *state, double *unused, double *out_714225981204664957);
void live_H_32(double *state, double *unused, double *out_3751984232890235025);
void live_h_13(double *state, double *unused, double *out_1668142141483076787);
void live_H_13(double *state, double *unused, double *out_4907299011831647318);
void live_h_14(double *state, double *unused, double *out_7304420017673863233);
void live_H_14(double *state, double *unused, double *out_3897873894935468357);
void live_h_33(double *state, double *unused, double *out_8597106513359373365);
void live_H_33(double *state, double *unused, double *out_8272840763392208924);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}