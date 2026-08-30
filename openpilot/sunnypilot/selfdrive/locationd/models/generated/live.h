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
void live_H(double *in_vec, double *out_1686275390483288342);
void live_err_fun(double *nom_x, double *delta_x, double *out_4204369337113210975);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_7593890448643766585);
void live_H_mod_fun(double *state, double *out_5639723240877718265);
void live_f_fun(double *state, double dt, double *out_7920419734128609707);
void live_F_fun(double *state, double dt, double *out_2531061305853351460);
void live_h_4(double *state, double *unused, double *out_3147360118262070147);
void live_H_4(double *state, double *unused, double *out_8207345065733290579);
void live_h_9(double *state, double *unused, double *out_3838094261891484992);
void live_H_9(double *state, double *unused, double *out_2952180072711813567);
void live_h_10(double *state, double *unused, double *out_6152858618848367018);
void live_H_10(double *state, double *unused, double *out_2753547377614867985);
void live_h_12(double *state, double *unused, double *out_6451887935286351215);
void live_H_12(double *state, double *unused, double *out_8828444090780884246);
void live_h_35(double *state, double *unused, double *out_157655733334568578);
void live_H_35(double *state, double *unused, double *out_6872736950603653661);
void live_h_32(double *state, double *unused, double *out_2653994444260524322);
void live_H_32(double *state, double *unused, double *out_4167712437701903724);
void live_h_13(double *state, double *unused, double *out_3805841268892977435);
void live_H_13(double *state, double *unused, double *out_3211394775125286984);
void live_h_14(double *state, double *unused, double *out_3838094261891484992);
void live_H_14(double *state, double *unused, double *out_2952180072711813567);
void live_h_33(double *state, double *unused, double *out_3413040511775125163);
void live_H_33(double *state, double *unused, double *out_3722179945964796057);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}