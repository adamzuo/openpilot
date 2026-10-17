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
void live_H(double *in_vec, double *out_3589750581610874277);
void live_err_fun(double *nom_x, double *delta_x, double *out_4400508739202933222);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3066621887474146689);
void live_H_mod_fun(double *state, double *out_8915466451166109650);
void live_f_fun(double *state, double dt, double *out_6663793111681161759);
void live_F_fun(double *state, double dt, double *out_7591386202345459412);
void live_h_4(double *state, double *unused, double *out_6627867492061395198);
void live_H_4(double *state, double *unused, double *out_82651809348569977);
void live_h_9(double *state, double *unused, double *out_7796051206436882836);
void live_H_9(double *state, double *unused, double *out_7369870744613017447);
void live_h_10(double *state, double *unused, double *out_6739308553818005081);
void live_H_10(double *state, double *unused, double *out_3947212066348177212);
void live_h_12(double *state, double *unused, double *out_2413922428330422709);
void live_H_12(double *state, double *unused, double *out_5102108217380531772);
void live_h_35(double *state, double *unused, double *out_1653002369470744688);
void live_H_35(double *state, double *unused, double *out_3449313866721177353);
void live_h_32(double *state, double *unused, double *out_4637120645457408829);
void live_H_32(double *state, double *unused, double *out_6154338379622927290);
void live_h_13(double *state, double *unused, double *out_5767788829072226025);
void live_H_13(double *state, double *unused, double *out_6535756240913130592);
void live_h_14(double *state, double *unused, double *out_7796051206436882836);
void live_H_14(double *state, double *unused, double *out_7369870744613017447);
void live_h_33(double *state, double *unused, double *out_2914046805339149345);
void live_H_33(double *state, double *unused, double *out_6599870871360034957);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}