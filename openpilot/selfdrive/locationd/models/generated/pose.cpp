#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6902255537984290412) {
   out_6902255537984290412[0] = delta_x[0] + nom_x[0];
   out_6902255537984290412[1] = delta_x[1] + nom_x[1];
   out_6902255537984290412[2] = delta_x[2] + nom_x[2];
   out_6902255537984290412[3] = delta_x[3] + nom_x[3];
   out_6902255537984290412[4] = delta_x[4] + nom_x[4];
   out_6902255537984290412[5] = delta_x[5] + nom_x[5];
   out_6902255537984290412[6] = delta_x[6] + nom_x[6];
   out_6902255537984290412[7] = delta_x[7] + nom_x[7];
   out_6902255537984290412[8] = delta_x[8] + nom_x[8];
   out_6902255537984290412[9] = delta_x[9] + nom_x[9];
   out_6902255537984290412[10] = delta_x[10] + nom_x[10];
   out_6902255537984290412[11] = delta_x[11] + nom_x[11];
   out_6902255537984290412[12] = delta_x[12] + nom_x[12];
   out_6902255537984290412[13] = delta_x[13] + nom_x[13];
   out_6902255537984290412[14] = delta_x[14] + nom_x[14];
   out_6902255537984290412[15] = delta_x[15] + nom_x[15];
   out_6902255537984290412[16] = delta_x[16] + nom_x[16];
   out_6902255537984290412[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_971548101061258492) {
   out_971548101061258492[0] = -nom_x[0] + true_x[0];
   out_971548101061258492[1] = -nom_x[1] + true_x[1];
   out_971548101061258492[2] = -nom_x[2] + true_x[2];
   out_971548101061258492[3] = -nom_x[3] + true_x[3];
   out_971548101061258492[4] = -nom_x[4] + true_x[4];
   out_971548101061258492[5] = -nom_x[5] + true_x[5];
   out_971548101061258492[6] = -nom_x[6] + true_x[6];
   out_971548101061258492[7] = -nom_x[7] + true_x[7];
   out_971548101061258492[8] = -nom_x[8] + true_x[8];
   out_971548101061258492[9] = -nom_x[9] + true_x[9];
   out_971548101061258492[10] = -nom_x[10] + true_x[10];
   out_971548101061258492[11] = -nom_x[11] + true_x[11];
   out_971548101061258492[12] = -nom_x[12] + true_x[12];
   out_971548101061258492[13] = -nom_x[13] + true_x[13];
   out_971548101061258492[14] = -nom_x[14] + true_x[14];
   out_971548101061258492[15] = -nom_x[15] + true_x[15];
   out_971548101061258492[16] = -nom_x[16] + true_x[16];
   out_971548101061258492[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_5872156854311632266) {
   out_5872156854311632266[0] = 1.0;
   out_5872156854311632266[1] = 0.0;
   out_5872156854311632266[2] = 0.0;
   out_5872156854311632266[3] = 0.0;
   out_5872156854311632266[4] = 0.0;
   out_5872156854311632266[5] = 0.0;
   out_5872156854311632266[6] = 0.0;
   out_5872156854311632266[7] = 0.0;
   out_5872156854311632266[8] = 0.0;
   out_5872156854311632266[9] = 0.0;
   out_5872156854311632266[10] = 0.0;
   out_5872156854311632266[11] = 0.0;
   out_5872156854311632266[12] = 0.0;
   out_5872156854311632266[13] = 0.0;
   out_5872156854311632266[14] = 0.0;
   out_5872156854311632266[15] = 0.0;
   out_5872156854311632266[16] = 0.0;
   out_5872156854311632266[17] = 0.0;
   out_5872156854311632266[18] = 0.0;
   out_5872156854311632266[19] = 1.0;
   out_5872156854311632266[20] = 0.0;
   out_5872156854311632266[21] = 0.0;
   out_5872156854311632266[22] = 0.0;
   out_5872156854311632266[23] = 0.0;
   out_5872156854311632266[24] = 0.0;
   out_5872156854311632266[25] = 0.0;
   out_5872156854311632266[26] = 0.0;
   out_5872156854311632266[27] = 0.0;
   out_5872156854311632266[28] = 0.0;
   out_5872156854311632266[29] = 0.0;
   out_5872156854311632266[30] = 0.0;
   out_5872156854311632266[31] = 0.0;
   out_5872156854311632266[32] = 0.0;
   out_5872156854311632266[33] = 0.0;
   out_5872156854311632266[34] = 0.0;
   out_5872156854311632266[35] = 0.0;
   out_5872156854311632266[36] = 0.0;
   out_5872156854311632266[37] = 0.0;
   out_5872156854311632266[38] = 1.0;
   out_5872156854311632266[39] = 0.0;
   out_5872156854311632266[40] = 0.0;
   out_5872156854311632266[41] = 0.0;
   out_5872156854311632266[42] = 0.0;
   out_5872156854311632266[43] = 0.0;
   out_5872156854311632266[44] = 0.0;
   out_5872156854311632266[45] = 0.0;
   out_5872156854311632266[46] = 0.0;
   out_5872156854311632266[47] = 0.0;
   out_5872156854311632266[48] = 0.0;
   out_5872156854311632266[49] = 0.0;
   out_5872156854311632266[50] = 0.0;
   out_5872156854311632266[51] = 0.0;
   out_5872156854311632266[52] = 0.0;
   out_5872156854311632266[53] = 0.0;
   out_5872156854311632266[54] = 0.0;
   out_5872156854311632266[55] = 0.0;
   out_5872156854311632266[56] = 0.0;
   out_5872156854311632266[57] = 1.0;
   out_5872156854311632266[58] = 0.0;
   out_5872156854311632266[59] = 0.0;
   out_5872156854311632266[60] = 0.0;
   out_5872156854311632266[61] = 0.0;
   out_5872156854311632266[62] = 0.0;
   out_5872156854311632266[63] = 0.0;
   out_5872156854311632266[64] = 0.0;
   out_5872156854311632266[65] = 0.0;
   out_5872156854311632266[66] = 0.0;
   out_5872156854311632266[67] = 0.0;
   out_5872156854311632266[68] = 0.0;
   out_5872156854311632266[69] = 0.0;
   out_5872156854311632266[70] = 0.0;
   out_5872156854311632266[71] = 0.0;
   out_5872156854311632266[72] = 0.0;
   out_5872156854311632266[73] = 0.0;
   out_5872156854311632266[74] = 0.0;
   out_5872156854311632266[75] = 0.0;
   out_5872156854311632266[76] = 1.0;
   out_5872156854311632266[77] = 0.0;
   out_5872156854311632266[78] = 0.0;
   out_5872156854311632266[79] = 0.0;
   out_5872156854311632266[80] = 0.0;
   out_5872156854311632266[81] = 0.0;
   out_5872156854311632266[82] = 0.0;
   out_5872156854311632266[83] = 0.0;
   out_5872156854311632266[84] = 0.0;
   out_5872156854311632266[85] = 0.0;
   out_5872156854311632266[86] = 0.0;
   out_5872156854311632266[87] = 0.0;
   out_5872156854311632266[88] = 0.0;
   out_5872156854311632266[89] = 0.0;
   out_5872156854311632266[90] = 0.0;
   out_5872156854311632266[91] = 0.0;
   out_5872156854311632266[92] = 0.0;
   out_5872156854311632266[93] = 0.0;
   out_5872156854311632266[94] = 0.0;
   out_5872156854311632266[95] = 1.0;
   out_5872156854311632266[96] = 0.0;
   out_5872156854311632266[97] = 0.0;
   out_5872156854311632266[98] = 0.0;
   out_5872156854311632266[99] = 0.0;
   out_5872156854311632266[100] = 0.0;
   out_5872156854311632266[101] = 0.0;
   out_5872156854311632266[102] = 0.0;
   out_5872156854311632266[103] = 0.0;
   out_5872156854311632266[104] = 0.0;
   out_5872156854311632266[105] = 0.0;
   out_5872156854311632266[106] = 0.0;
   out_5872156854311632266[107] = 0.0;
   out_5872156854311632266[108] = 0.0;
   out_5872156854311632266[109] = 0.0;
   out_5872156854311632266[110] = 0.0;
   out_5872156854311632266[111] = 0.0;
   out_5872156854311632266[112] = 0.0;
   out_5872156854311632266[113] = 0.0;
   out_5872156854311632266[114] = 1.0;
   out_5872156854311632266[115] = 0.0;
   out_5872156854311632266[116] = 0.0;
   out_5872156854311632266[117] = 0.0;
   out_5872156854311632266[118] = 0.0;
   out_5872156854311632266[119] = 0.0;
   out_5872156854311632266[120] = 0.0;
   out_5872156854311632266[121] = 0.0;
   out_5872156854311632266[122] = 0.0;
   out_5872156854311632266[123] = 0.0;
   out_5872156854311632266[124] = 0.0;
   out_5872156854311632266[125] = 0.0;
   out_5872156854311632266[126] = 0.0;
   out_5872156854311632266[127] = 0.0;
   out_5872156854311632266[128] = 0.0;
   out_5872156854311632266[129] = 0.0;
   out_5872156854311632266[130] = 0.0;
   out_5872156854311632266[131] = 0.0;
   out_5872156854311632266[132] = 0.0;
   out_5872156854311632266[133] = 1.0;
   out_5872156854311632266[134] = 0.0;
   out_5872156854311632266[135] = 0.0;
   out_5872156854311632266[136] = 0.0;
   out_5872156854311632266[137] = 0.0;
   out_5872156854311632266[138] = 0.0;
   out_5872156854311632266[139] = 0.0;
   out_5872156854311632266[140] = 0.0;
   out_5872156854311632266[141] = 0.0;
   out_5872156854311632266[142] = 0.0;
   out_5872156854311632266[143] = 0.0;
   out_5872156854311632266[144] = 0.0;
   out_5872156854311632266[145] = 0.0;
   out_5872156854311632266[146] = 0.0;
   out_5872156854311632266[147] = 0.0;
   out_5872156854311632266[148] = 0.0;
   out_5872156854311632266[149] = 0.0;
   out_5872156854311632266[150] = 0.0;
   out_5872156854311632266[151] = 0.0;
   out_5872156854311632266[152] = 1.0;
   out_5872156854311632266[153] = 0.0;
   out_5872156854311632266[154] = 0.0;
   out_5872156854311632266[155] = 0.0;
   out_5872156854311632266[156] = 0.0;
   out_5872156854311632266[157] = 0.0;
   out_5872156854311632266[158] = 0.0;
   out_5872156854311632266[159] = 0.0;
   out_5872156854311632266[160] = 0.0;
   out_5872156854311632266[161] = 0.0;
   out_5872156854311632266[162] = 0.0;
   out_5872156854311632266[163] = 0.0;
   out_5872156854311632266[164] = 0.0;
   out_5872156854311632266[165] = 0.0;
   out_5872156854311632266[166] = 0.0;
   out_5872156854311632266[167] = 0.0;
   out_5872156854311632266[168] = 0.0;
   out_5872156854311632266[169] = 0.0;
   out_5872156854311632266[170] = 0.0;
   out_5872156854311632266[171] = 1.0;
   out_5872156854311632266[172] = 0.0;
   out_5872156854311632266[173] = 0.0;
   out_5872156854311632266[174] = 0.0;
   out_5872156854311632266[175] = 0.0;
   out_5872156854311632266[176] = 0.0;
   out_5872156854311632266[177] = 0.0;
   out_5872156854311632266[178] = 0.0;
   out_5872156854311632266[179] = 0.0;
   out_5872156854311632266[180] = 0.0;
   out_5872156854311632266[181] = 0.0;
   out_5872156854311632266[182] = 0.0;
   out_5872156854311632266[183] = 0.0;
   out_5872156854311632266[184] = 0.0;
   out_5872156854311632266[185] = 0.0;
   out_5872156854311632266[186] = 0.0;
   out_5872156854311632266[187] = 0.0;
   out_5872156854311632266[188] = 0.0;
   out_5872156854311632266[189] = 0.0;
   out_5872156854311632266[190] = 1.0;
   out_5872156854311632266[191] = 0.0;
   out_5872156854311632266[192] = 0.0;
   out_5872156854311632266[193] = 0.0;
   out_5872156854311632266[194] = 0.0;
   out_5872156854311632266[195] = 0.0;
   out_5872156854311632266[196] = 0.0;
   out_5872156854311632266[197] = 0.0;
   out_5872156854311632266[198] = 0.0;
   out_5872156854311632266[199] = 0.0;
   out_5872156854311632266[200] = 0.0;
   out_5872156854311632266[201] = 0.0;
   out_5872156854311632266[202] = 0.0;
   out_5872156854311632266[203] = 0.0;
   out_5872156854311632266[204] = 0.0;
   out_5872156854311632266[205] = 0.0;
   out_5872156854311632266[206] = 0.0;
   out_5872156854311632266[207] = 0.0;
   out_5872156854311632266[208] = 0.0;
   out_5872156854311632266[209] = 1.0;
   out_5872156854311632266[210] = 0.0;
   out_5872156854311632266[211] = 0.0;
   out_5872156854311632266[212] = 0.0;
   out_5872156854311632266[213] = 0.0;
   out_5872156854311632266[214] = 0.0;
   out_5872156854311632266[215] = 0.0;
   out_5872156854311632266[216] = 0.0;
   out_5872156854311632266[217] = 0.0;
   out_5872156854311632266[218] = 0.0;
   out_5872156854311632266[219] = 0.0;
   out_5872156854311632266[220] = 0.0;
   out_5872156854311632266[221] = 0.0;
   out_5872156854311632266[222] = 0.0;
   out_5872156854311632266[223] = 0.0;
   out_5872156854311632266[224] = 0.0;
   out_5872156854311632266[225] = 0.0;
   out_5872156854311632266[226] = 0.0;
   out_5872156854311632266[227] = 0.0;
   out_5872156854311632266[228] = 1.0;
   out_5872156854311632266[229] = 0.0;
   out_5872156854311632266[230] = 0.0;
   out_5872156854311632266[231] = 0.0;
   out_5872156854311632266[232] = 0.0;
   out_5872156854311632266[233] = 0.0;
   out_5872156854311632266[234] = 0.0;
   out_5872156854311632266[235] = 0.0;
   out_5872156854311632266[236] = 0.0;
   out_5872156854311632266[237] = 0.0;
   out_5872156854311632266[238] = 0.0;
   out_5872156854311632266[239] = 0.0;
   out_5872156854311632266[240] = 0.0;
   out_5872156854311632266[241] = 0.0;
   out_5872156854311632266[242] = 0.0;
   out_5872156854311632266[243] = 0.0;
   out_5872156854311632266[244] = 0.0;
   out_5872156854311632266[245] = 0.0;
   out_5872156854311632266[246] = 0.0;
   out_5872156854311632266[247] = 1.0;
   out_5872156854311632266[248] = 0.0;
   out_5872156854311632266[249] = 0.0;
   out_5872156854311632266[250] = 0.0;
   out_5872156854311632266[251] = 0.0;
   out_5872156854311632266[252] = 0.0;
   out_5872156854311632266[253] = 0.0;
   out_5872156854311632266[254] = 0.0;
   out_5872156854311632266[255] = 0.0;
   out_5872156854311632266[256] = 0.0;
   out_5872156854311632266[257] = 0.0;
   out_5872156854311632266[258] = 0.0;
   out_5872156854311632266[259] = 0.0;
   out_5872156854311632266[260] = 0.0;
   out_5872156854311632266[261] = 0.0;
   out_5872156854311632266[262] = 0.0;
   out_5872156854311632266[263] = 0.0;
   out_5872156854311632266[264] = 0.0;
   out_5872156854311632266[265] = 0.0;
   out_5872156854311632266[266] = 1.0;
   out_5872156854311632266[267] = 0.0;
   out_5872156854311632266[268] = 0.0;
   out_5872156854311632266[269] = 0.0;
   out_5872156854311632266[270] = 0.0;
   out_5872156854311632266[271] = 0.0;
   out_5872156854311632266[272] = 0.0;
   out_5872156854311632266[273] = 0.0;
   out_5872156854311632266[274] = 0.0;
   out_5872156854311632266[275] = 0.0;
   out_5872156854311632266[276] = 0.0;
   out_5872156854311632266[277] = 0.0;
   out_5872156854311632266[278] = 0.0;
   out_5872156854311632266[279] = 0.0;
   out_5872156854311632266[280] = 0.0;
   out_5872156854311632266[281] = 0.0;
   out_5872156854311632266[282] = 0.0;
   out_5872156854311632266[283] = 0.0;
   out_5872156854311632266[284] = 0.0;
   out_5872156854311632266[285] = 1.0;
   out_5872156854311632266[286] = 0.0;
   out_5872156854311632266[287] = 0.0;
   out_5872156854311632266[288] = 0.0;
   out_5872156854311632266[289] = 0.0;
   out_5872156854311632266[290] = 0.0;
   out_5872156854311632266[291] = 0.0;
   out_5872156854311632266[292] = 0.0;
   out_5872156854311632266[293] = 0.0;
   out_5872156854311632266[294] = 0.0;
   out_5872156854311632266[295] = 0.0;
   out_5872156854311632266[296] = 0.0;
   out_5872156854311632266[297] = 0.0;
   out_5872156854311632266[298] = 0.0;
   out_5872156854311632266[299] = 0.0;
   out_5872156854311632266[300] = 0.0;
   out_5872156854311632266[301] = 0.0;
   out_5872156854311632266[302] = 0.0;
   out_5872156854311632266[303] = 0.0;
   out_5872156854311632266[304] = 1.0;
   out_5872156854311632266[305] = 0.0;
   out_5872156854311632266[306] = 0.0;
   out_5872156854311632266[307] = 0.0;
   out_5872156854311632266[308] = 0.0;
   out_5872156854311632266[309] = 0.0;
   out_5872156854311632266[310] = 0.0;
   out_5872156854311632266[311] = 0.0;
   out_5872156854311632266[312] = 0.0;
   out_5872156854311632266[313] = 0.0;
   out_5872156854311632266[314] = 0.0;
   out_5872156854311632266[315] = 0.0;
   out_5872156854311632266[316] = 0.0;
   out_5872156854311632266[317] = 0.0;
   out_5872156854311632266[318] = 0.0;
   out_5872156854311632266[319] = 0.0;
   out_5872156854311632266[320] = 0.0;
   out_5872156854311632266[321] = 0.0;
   out_5872156854311632266[322] = 0.0;
   out_5872156854311632266[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_7853950092549063317) {
   out_7853950092549063317[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_7853950092549063317[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_7853950092549063317[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_7853950092549063317[3] = dt*state[12] + state[3];
   out_7853950092549063317[4] = dt*state[13] + state[4];
   out_7853950092549063317[5] = dt*state[14] + state[5];
   out_7853950092549063317[6] = state[6];
   out_7853950092549063317[7] = state[7];
   out_7853950092549063317[8] = state[8];
   out_7853950092549063317[9] = state[9];
   out_7853950092549063317[10] = state[10];
   out_7853950092549063317[11] = state[11];
   out_7853950092549063317[12] = state[12];
   out_7853950092549063317[13] = state[13];
   out_7853950092549063317[14] = state[14];
   out_7853950092549063317[15] = state[15];
   out_7853950092549063317[16] = state[16];
   out_7853950092549063317[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2347128049005111238) {
   out_2347128049005111238[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2347128049005111238[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2347128049005111238[2] = 0;
   out_2347128049005111238[3] = 0;
   out_2347128049005111238[4] = 0;
   out_2347128049005111238[5] = 0;
   out_2347128049005111238[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2347128049005111238[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2347128049005111238[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2347128049005111238[9] = 0;
   out_2347128049005111238[10] = 0;
   out_2347128049005111238[11] = 0;
   out_2347128049005111238[12] = 0;
   out_2347128049005111238[13] = 0;
   out_2347128049005111238[14] = 0;
   out_2347128049005111238[15] = 0;
   out_2347128049005111238[16] = 0;
   out_2347128049005111238[17] = 0;
   out_2347128049005111238[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2347128049005111238[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2347128049005111238[20] = 0;
   out_2347128049005111238[21] = 0;
   out_2347128049005111238[22] = 0;
   out_2347128049005111238[23] = 0;
   out_2347128049005111238[24] = 0;
   out_2347128049005111238[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2347128049005111238[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2347128049005111238[27] = 0;
   out_2347128049005111238[28] = 0;
   out_2347128049005111238[29] = 0;
   out_2347128049005111238[30] = 0;
   out_2347128049005111238[31] = 0;
   out_2347128049005111238[32] = 0;
   out_2347128049005111238[33] = 0;
   out_2347128049005111238[34] = 0;
   out_2347128049005111238[35] = 0;
   out_2347128049005111238[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2347128049005111238[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2347128049005111238[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2347128049005111238[39] = 0;
   out_2347128049005111238[40] = 0;
   out_2347128049005111238[41] = 0;
   out_2347128049005111238[42] = 0;
   out_2347128049005111238[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2347128049005111238[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2347128049005111238[45] = 0;
   out_2347128049005111238[46] = 0;
   out_2347128049005111238[47] = 0;
   out_2347128049005111238[48] = 0;
   out_2347128049005111238[49] = 0;
   out_2347128049005111238[50] = 0;
   out_2347128049005111238[51] = 0;
   out_2347128049005111238[52] = 0;
   out_2347128049005111238[53] = 0;
   out_2347128049005111238[54] = 0;
   out_2347128049005111238[55] = 0;
   out_2347128049005111238[56] = 0;
   out_2347128049005111238[57] = 1;
   out_2347128049005111238[58] = 0;
   out_2347128049005111238[59] = 0;
   out_2347128049005111238[60] = 0;
   out_2347128049005111238[61] = 0;
   out_2347128049005111238[62] = 0;
   out_2347128049005111238[63] = 0;
   out_2347128049005111238[64] = 0;
   out_2347128049005111238[65] = 0;
   out_2347128049005111238[66] = dt;
   out_2347128049005111238[67] = 0;
   out_2347128049005111238[68] = 0;
   out_2347128049005111238[69] = 0;
   out_2347128049005111238[70] = 0;
   out_2347128049005111238[71] = 0;
   out_2347128049005111238[72] = 0;
   out_2347128049005111238[73] = 0;
   out_2347128049005111238[74] = 0;
   out_2347128049005111238[75] = 0;
   out_2347128049005111238[76] = 1;
   out_2347128049005111238[77] = 0;
   out_2347128049005111238[78] = 0;
   out_2347128049005111238[79] = 0;
   out_2347128049005111238[80] = 0;
   out_2347128049005111238[81] = 0;
   out_2347128049005111238[82] = 0;
   out_2347128049005111238[83] = 0;
   out_2347128049005111238[84] = 0;
   out_2347128049005111238[85] = dt;
   out_2347128049005111238[86] = 0;
   out_2347128049005111238[87] = 0;
   out_2347128049005111238[88] = 0;
   out_2347128049005111238[89] = 0;
   out_2347128049005111238[90] = 0;
   out_2347128049005111238[91] = 0;
   out_2347128049005111238[92] = 0;
   out_2347128049005111238[93] = 0;
   out_2347128049005111238[94] = 0;
   out_2347128049005111238[95] = 1;
   out_2347128049005111238[96] = 0;
   out_2347128049005111238[97] = 0;
   out_2347128049005111238[98] = 0;
   out_2347128049005111238[99] = 0;
   out_2347128049005111238[100] = 0;
   out_2347128049005111238[101] = 0;
   out_2347128049005111238[102] = 0;
   out_2347128049005111238[103] = 0;
   out_2347128049005111238[104] = dt;
   out_2347128049005111238[105] = 0;
   out_2347128049005111238[106] = 0;
   out_2347128049005111238[107] = 0;
   out_2347128049005111238[108] = 0;
   out_2347128049005111238[109] = 0;
   out_2347128049005111238[110] = 0;
   out_2347128049005111238[111] = 0;
   out_2347128049005111238[112] = 0;
   out_2347128049005111238[113] = 0;
   out_2347128049005111238[114] = 1;
   out_2347128049005111238[115] = 0;
   out_2347128049005111238[116] = 0;
   out_2347128049005111238[117] = 0;
   out_2347128049005111238[118] = 0;
   out_2347128049005111238[119] = 0;
   out_2347128049005111238[120] = 0;
   out_2347128049005111238[121] = 0;
   out_2347128049005111238[122] = 0;
   out_2347128049005111238[123] = 0;
   out_2347128049005111238[124] = 0;
   out_2347128049005111238[125] = 0;
   out_2347128049005111238[126] = 0;
   out_2347128049005111238[127] = 0;
   out_2347128049005111238[128] = 0;
   out_2347128049005111238[129] = 0;
   out_2347128049005111238[130] = 0;
   out_2347128049005111238[131] = 0;
   out_2347128049005111238[132] = 0;
   out_2347128049005111238[133] = 1;
   out_2347128049005111238[134] = 0;
   out_2347128049005111238[135] = 0;
   out_2347128049005111238[136] = 0;
   out_2347128049005111238[137] = 0;
   out_2347128049005111238[138] = 0;
   out_2347128049005111238[139] = 0;
   out_2347128049005111238[140] = 0;
   out_2347128049005111238[141] = 0;
   out_2347128049005111238[142] = 0;
   out_2347128049005111238[143] = 0;
   out_2347128049005111238[144] = 0;
   out_2347128049005111238[145] = 0;
   out_2347128049005111238[146] = 0;
   out_2347128049005111238[147] = 0;
   out_2347128049005111238[148] = 0;
   out_2347128049005111238[149] = 0;
   out_2347128049005111238[150] = 0;
   out_2347128049005111238[151] = 0;
   out_2347128049005111238[152] = 1;
   out_2347128049005111238[153] = 0;
   out_2347128049005111238[154] = 0;
   out_2347128049005111238[155] = 0;
   out_2347128049005111238[156] = 0;
   out_2347128049005111238[157] = 0;
   out_2347128049005111238[158] = 0;
   out_2347128049005111238[159] = 0;
   out_2347128049005111238[160] = 0;
   out_2347128049005111238[161] = 0;
   out_2347128049005111238[162] = 0;
   out_2347128049005111238[163] = 0;
   out_2347128049005111238[164] = 0;
   out_2347128049005111238[165] = 0;
   out_2347128049005111238[166] = 0;
   out_2347128049005111238[167] = 0;
   out_2347128049005111238[168] = 0;
   out_2347128049005111238[169] = 0;
   out_2347128049005111238[170] = 0;
   out_2347128049005111238[171] = 1;
   out_2347128049005111238[172] = 0;
   out_2347128049005111238[173] = 0;
   out_2347128049005111238[174] = 0;
   out_2347128049005111238[175] = 0;
   out_2347128049005111238[176] = 0;
   out_2347128049005111238[177] = 0;
   out_2347128049005111238[178] = 0;
   out_2347128049005111238[179] = 0;
   out_2347128049005111238[180] = 0;
   out_2347128049005111238[181] = 0;
   out_2347128049005111238[182] = 0;
   out_2347128049005111238[183] = 0;
   out_2347128049005111238[184] = 0;
   out_2347128049005111238[185] = 0;
   out_2347128049005111238[186] = 0;
   out_2347128049005111238[187] = 0;
   out_2347128049005111238[188] = 0;
   out_2347128049005111238[189] = 0;
   out_2347128049005111238[190] = 1;
   out_2347128049005111238[191] = 0;
   out_2347128049005111238[192] = 0;
   out_2347128049005111238[193] = 0;
   out_2347128049005111238[194] = 0;
   out_2347128049005111238[195] = 0;
   out_2347128049005111238[196] = 0;
   out_2347128049005111238[197] = 0;
   out_2347128049005111238[198] = 0;
   out_2347128049005111238[199] = 0;
   out_2347128049005111238[200] = 0;
   out_2347128049005111238[201] = 0;
   out_2347128049005111238[202] = 0;
   out_2347128049005111238[203] = 0;
   out_2347128049005111238[204] = 0;
   out_2347128049005111238[205] = 0;
   out_2347128049005111238[206] = 0;
   out_2347128049005111238[207] = 0;
   out_2347128049005111238[208] = 0;
   out_2347128049005111238[209] = 1;
   out_2347128049005111238[210] = 0;
   out_2347128049005111238[211] = 0;
   out_2347128049005111238[212] = 0;
   out_2347128049005111238[213] = 0;
   out_2347128049005111238[214] = 0;
   out_2347128049005111238[215] = 0;
   out_2347128049005111238[216] = 0;
   out_2347128049005111238[217] = 0;
   out_2347128049005111238[218] = 0;
   out_2347128049005111238[219] = 0;
   out_2347128049005111238[220] = 0;
   out_2347128049005111238[221] = 0;
   out_2347128049005111238[222] = 0;
   out_2347128049005111238[223] = 0;
   out_2347128049005111238[224] = 0;
   out_2347128049005111238[225] = 0;
   out_2347128049005111238[226] = 0;
   out_2347128049005111238[227] = 0;
   out_2347128049005111238[228] = 1;
   out_2347128049005111238[229] = 0;
   out_2347128049005111238[230] = 0;
   out_2347128049005111238[231] = 0;
   out_2347128049005111238[232] = 0;
   out_2347128049005111238[233] = 0;
   out_2347128049005111238[234] = 0;
   out_2347128049005111238[235] = 0;
   out_2347128049005111238[236] = 0;
   out_2347128049005111238[237] = 0;
   out_2347128049005111238[238] = 0;
   out_2347128049005111238[239] = 0;
   out_2347128049005111238[240] = 0;
   out_2347128049005111238[241] = 0;
   out_2347128049005111238[242] = 0;
   out_2347128049005111238[243] = 0;
   out_2347128049005111238[244] = 0;
   out_2347128049005111238[245] = 0;
   out_2347128049005111238[246] = 0;
   out_2347128049005111238[247] = 1;
   out_2347128049005111238[248] = 0;
   out_2347128049005111238[249] = 0;
   out_2347128049005111238[250] = 0;
   out_2347128049005111238[251] = 0;
   out_2347128049005111238[252] = 0;
   out_2347128049005111238[253] = 0;
   out_2347128049005111238[254] = 0;
   out_2347128049005111238[255] = 0;
   out_2347128049005111238[256] = 0;
   out_2347128049005111238[257] = 0;
   out_2347128049005111238[258] = 0;
   out_2347128049005111238[259] = 0;
   out_2347128049005111238[260] = 0;
   out_2347128049005111238[261] = 0;
   out_2347128049005111238[262] = 0;
   out_2347128049005111238[263] = 0;
   out_2347128049005111238[264] = 0;
   out_2347128049005111238[265] = 0;
   out_2347128049005111238[266] = 1;
   out_2347128049005111238[267] = 0;
   out_2347128049005111238[268] = 0;
   out_2347128049005111238[269] = 0;
   out_2347128049005111238[270] = 0;
   out_2347128049005111238[271] = 0;
   out_2347128049005111238[272] = 0;
   out_2347128049005111238[273] = 0;
   out_2347128049005111238[274] = 0;
   out_2347128049005111238[275] = 0;
   out_2347128049005111238[276] = 0;
   out_2347128049005111238[277] = 0;
   out_2347128049005111238[278] = 0;
   out_2347128049005111238[279] = 0;
   out_2347128049005111238[280] = 0;
   out_2347128049005111238[281] = 0;
   out_2347128049005111238[282] = 0;
   out_2347128049005111238[283] = 0;
   out_2347128049005111238[284] = 0;
   out_2347128049005111238[285] = 1;
   out_2347128049005111238[286] = 0;
   out_2347128049005111238[287] = 0;
   out_2347128049005111238[288] = 0;
   out_2347128049005111238[289] = 0;
   out_2347128049005111238[290] = 0;
   out_2347128049005111238[291] = 0;
   out_2347128049005111238[292] = 0;
   out_2347128049005111238[293] = 0;
   out_2347128049005111238[294] = 0;
   out_2347128049005111238[295] = 0;
   out_2347128049005111238[296] = 0;
   out_2347128049005111238[297] = 0;
   out_2347128049005111238[298] = 0;
   out_2347128049005111238[299] = 0;
   out_2347128049005111238[300] = 0;
   out_2347128049005111238[301] = 0;
   out_2347128049005111238[302] = 0;
   out_2347128049005111238[303] = 0;
   out_2347128049005111238[304] = 1;
   out_2347128049005111238[305] = 0;
   out_2347128049005111238[306] = 0;
   out_2347128049005111238[307] = 0;
   out_2347128049005111238[308] = 0;
   out_2347128049005111238[309] = 0;
   out_2347128049005111238[310] = 0;
   out_2347128049005111238[311] = 0;
   out_2347128049005111238[312] = 0;
   out_2347128049005111238[313] = 0;
   out_2347128049005111238[314] = 0;
   out_2347128049005111238[315] = 0;
   out_2347128049005111238[316] = 0;
   out_2347128049005111238[317] = 0;
   out_2347128049005111238[318] = 0;
   out_2347128049005111238[319] = 0;
   out_2347128049005111238[320] = 0;
   out_2347128049005111238[321] = 0;
   out_2347128049005111238[322] = 0;
   out_2347128049005111238[323] = 1;
}
void h_4(double *state, double *unused, double *out_5113645833906790079) {
   out_5113645833906790079[0] = state[6] + state[9];
   out_5113645833906790079[1] = state[7] + state[10];
   out_5113645833906790079[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2671746953210162242) {
   out_2671746953210162242[0] = 0;
   out_2671746953210162242[1] = 0;
   out_2671746953210162242[2] = 0;
   out_2671746953210162242[3] = 0;
   out_2671746953210162242[4] = 0;
   out_2671746953210162242[5] = 0;
   out_2671746953210162242[6] = 1;
   out_2671746953210162242[7] = 0;
   out_2671746953210162242[8] = 0;
   out_2671746953210162242[9] = 1;
   out_2671746953210162242[10] = 0;
   out_2671746953210162242[11] = 0;
   out_2671746953210162242[12] = 0;
   out_2671746953210162242[13] = 0;
   out_2671746953210162242[14] = 0;
   out_2671746953210162242[15] = 0;
   out_2671746953210162242[16] = 0;
   out_2671746953210162242[17] = 0;
   out_2671746953210162242[18] = 0;
   out_2671746953210162242[19] = 0;
   out_2671746953210162242[20] = 0;
   out_2671746953210162242[21] = 0;
   out_2671746953210162242[22] = 0;
   out_2671746953210162242[23] = 0;
   out_2671746953210162242[24] = 0;
   out_2671746953210162242[25] = 1;
   out_2671746953210162242[26] = 0;
   out_2671746953210162242[27] = 0;
   out_2671746953210162242[28] = 1;
   out_2671746953210162242[29] = 0;
   out_2671746953210162242[30] = 0;
   out_2671746953210162242[31] = 0;
   out_2671746953210162242[32] = 0;
   out_2671746953210162242[33] = 0;
   out_2671746953210162242[34] = 0;
   out_2671746953210162242[35] = 0;
   out_2671746953210162242[36] = 0;
   out_2671746953210162242[37] = 0;
   out_2671746953210162242[38] = 0;
   out_2671746953210162242[39] = 0;
   out_2671746953210162242[40] = 0;
   out_2671746953210162242[41] = 0;
   out_2671746953210162242[42] = 0;
   out_2671746953210162242[43] = 0;
   out_2671746953210162242[44] = 1;
   out_2671746953210162242[45] = 0;
   out_2671746953210162242[46] = 0;
   out_2671746953210162242[47] = 1;
   out_2671746953210162242[48] = 0;
   out_2671746953210162242[49] = 0;
   out_2671746953210162242[50] = 0;
   out_2671746953210162242[51] = 0;
   out_2671746953210162242[52] = 0;
   out_2671746953210162242[53] = 0;
}
void h_10(double *state, double *unused, double *out_5157045351269012104) {
   out_5157045351269012104[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5157045351269012104[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5157045351269012104[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_6967721430408486920) {
   out_6967721430408486920[0] = 0;
   out_6967721430408486920[1] = 9.8100000000000005*cos(state[1]);
   out_6967721430408486920[2] = 0;
   out_6967721430408486920[3] = 0;
   out_6967721430408486920[4] = -state[8];
   out_6967721430408486920[5] = state[7];
   out_6967721430408486920[6] = 0;
   out_6967721430408486920[7] = state[5];
   out_6967721430408486920[8] = -state[4];
   out_6967721430408486920[9] = 0;
   out_6967721430408486920[10] = 0;
   out_6967721430408486920[11] = 0;
   out_6967721430408486920[12] = 1;
   out_6967721430408486920[13] = 0;
   out_6967721430408486920[14] = 0;
   out_6967721430408486920[15] = 1;
   out_6967721430408486920[16] = 0;
   out_6967721430408486920[17] = 0;
   out_6967721430408486920[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_6967721430408486920[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_6967721430408486920[20] = 0;
   out_6967721430408486920[21] = state[8];
   out_6967721430408486920[22] = 0;
   out_6967721430408486920[23] = -state[6];
   out_6967721430408486920[24] = -state[5];
   out_6967721430408486920[25] = 0;
   out_6967721430408486920[26] = state[3];
   out_6967721430408486920[27] = 0;
   out_6967721430408486920[28] = 0;
   out_6967721430408486920[29] = 0;
   out_6967721430408486920[30] = 0;
   out_6967721430408486920[31] = 1;
   out_6967721430408486920[32] = 0;
   out_6967721430408486920[33] = 0;
   out_6967721430408486920[34] = 1;
   out_6967721430408486920[35] = 0;
   out_6967721430408486920[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_6967721430408486920[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_6967721430408486920[38] = 0;
   out_6967721430408486920[39] = -state[7];
   out_6967721430408486920[40] = state[6];
   out_6967721430408486920[41] = 0;
   out_6967721430408486920[42] = state[4];
   out_6967721430408486920[43] = -state[3];
   out_6967721430408486920[44] = 0;
   out_6967721430408486920[45] = 0;
   out_6967721430408486920[46] = 0;
   out_6967721430408486920[47] = 0;
   out_6967721430408486920[48] = 0;
   out_6967721430408486920[49] = 0;
   out_6967721430408486920[50] = 1;
   out_6967721430408486920[51] = 0;
   out_6967721430408486920[52] = 0;
   out_6967721430408486920[53] = 1;
}
void h_13(double *state, double *unused, double *out_488268826700853092) {
   out_488268826700853092[0] = state[3];
   out_488268826700853092[1] = state[4];
   out_488268826700853092[2] = state[5];
}
void H_13(double *state, double *unused, double *out_8164365912182688445) {
   out_8164365912182688445[0] = 0;
   out_8164365912182688445[1] = 0;
   out_8164365912182688445[2] = 0;
   out_8164365912182688445[3] = 1;
   out_8164365912182688445[4] = 0;
   out_8164365912182688445[5] = 0;
   out_8164365912182688445[6] = 0;
   out_8164365912182688445[7] = 0;
   out_8164365912182688445[8] = 0;
   out_8164365912182688445[9] = 0;
   out_8164365912182688445[10] = 0;
   out_8164365912182688445[11] = 0;
   out_8164365912182688445[12] = 0;
   out_8164365912182688445[13] = 0;
   out_8164365912182688445[14] = 0;
   out_8164365912182688445[15] = 0;
   out_8164365912182688445[16] = 0;
   out_8164365912182688445[17] = 0;
   out_8164365912182688445[18] = 0;
   out_8164365912182688445[19] = 0;
   out_8164365912182688445[20] = 0;
   out_8164365912182688445[21] = 0;
   out_8164365912182688445[22] = 1;
   out_8164365912182688445[23] = 0;
   out_8164365912182688445[24] = 0;
   out_8164365912182688445[25] = 0;
   out_8164365912182688445[26] = 0;
   out_8164365912182688445[27] = 0;
   out_8164365912182688445[28] = 0;
   out_8164365912182688445[29] = 0;
   out_8164365912182688445[30] = 0;
   out_8164365912182688445[31] = 0;
   out_8164365912182688445[32] = 0;
   out_8164365912182688445[33] = 0;
   out_8164365912182688445[34] = 0;
   out_8164365912182688445[35] = 0;
   out_8164365912182688445[36] = 0;
   out_8164365912182688445[37] = 0;
   out_8164365912182688445[38] = 0;
   out_8164365912182688445[39] = 0;
   out_8164365912182688445[40] = 0;
   out_8164365912182688445[41] = 1;
   out_8164365912182688445[42] = 0;
   out_8164365912182688445[43] = 0;
   out_8164365912182688445[44] = 0;
   out_8164365912182688445[45] = 0;
   out_8164365912182688445[46] = 0;
   out_8164365912182688445[47] = 0;
   out_8164365912182688445[48] = 0;
   out_8164365912182688445[49] = 0;
   out_8164365912182688445[50] = 0;
   out_8164365912182688445[51] = 0;
   out_8164365912182688445[52] = 0;
   out_8164365912182688445[53] = 0;
}
void h_14(double *state, double *unused, double *out_2240330411018770776) {
   out_2240330411018770776[0] = state[6];
   out_2240330411018770776[1] = state[7];
   out_2240330411018770776[2] = state[8];
}
void H_14(double *state, double *unused, double *out_411041479085210054) {
   out_411041479085210054[0] = 0;
   out_411041479085210054[1] = 0;
   out_411041479085210054[2] = 0;
   out_411041479085210054[3] = 0;
   out_411041479085210054[4] = 0;
   out_411041479085210054[5] = 0;
   out_411041479085210054[6] = 1;
   out_411041479085210054[7] = 0;
   out_411041479085210054[8] = 0;
   out_411041479085210054[9] = 0;
   out_411041479085210054[10] = 0;
   out_411041479085210054[11] = 0;
   out_411041479085210054[12] = 0;
   out_411041479085210054[13] = 0;
   out_411041479085210054[14] = 0;
   out_411041479085210054[15] = 0;
   out_411041479085210054[16] = 0;
   out_411041479085210054[17] = 0;
   out_411041479085210054[18] = 0;
   out_411041479085210054[19] = 0;
   out_411041479085210054[20] = 0;
   out_411041479085210054[21] = 0;
   out_411041479085210054[22] = 0;
   out_411041479085210054[23] = 0;
   out_411041479085210054[24] = 0;
   out_411041479085210054[25] = 1;
   out_411041479085210054[26] = 0;
   out_411041479085210054[27] = 0;
   out_411041479085210054[28] = 0;
   out_411041479085210054[29] = 0;
   out_411041479085210054[30] = 0;
   out_411041479085210054[31] = 0;
   out_411041479085210054[32] = 0;
   out_411041479085210054[33] = 0;
   out_411041479085210054[34] = 0;
   out_411041479085210054[35] = 0;
   out_411041479085210054[36] = 0;
   out_411041479085210054[37] = 0;
   out_411041479085210054[38] = 0;
   out_411041479085210054[39] = 0;
   out_411041479085210054[40] = 0;
   out_411041479085210054[41] = 0;
   out_411041479085210054[42] = 0;
   out_411041479085210054[43] = 0;
   out_411041479085210054[44] = 1;
   out_411041479085210054[45] = 0;
   out_411041479085210054[46] = 0;
   out_411041479085210054[47] = 0;
   out_411041479085210054[48] = 0;
   out_411041479085210054[49] = 0;
   out_411041479085210054[50] = 0;
   out_411041479085210054[51] = 0;
   out_411041479085210054[52] = 0;
   out_411041479085210054[53] = 0;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_6902255537984290412) {
  err_fun(nom_x, delta_x, out_6902255537984290412);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_971548101061258492) {
  inv_err_fun(nom_x, true_x, out_971548101061258492);
}
void pose_H_mod_fun(double *state, double *out_5872156854311632266) {
  H_mod_fun(state, out_5872156854311632266);
}
void pose_f_fun(double *state, double dt, double *out_7853950092549063317) {
  f_fun(state,  dt, out_7853950092549063317);
}
void pose_F_fun(double *state, double dt, double *out_2347128049005111238) {
  F_fun(state,  dt, out_2347128049005111238);
}
void pose_h_4(double *state, double *unused, double *out_5113645833906790079) {
  h_4(state, unused, out_5113645833906790079);
}
void pose_H_4(double *state, double *unused, double *out_2671746953210162242) {
  H_4(state, unused, out_2671746953210162242);
}
void pose_h_10(double *state, double *unused, double *out_5157045351269012104) {
  h_10(state, unused, out_5157045351269012104);
}
void pose_H_10(double *state, double *unused, double *out_6967721430408486920) {
  H_10(state, unused, out_6967721430408486920);
}
void pose_h_13(double *state, double *unused, double *out_488268826700853092) {
  h_13(state, unused, out_488268826700853092);
}
void pose_H_13(double *state, double *unused, double *out_8164365912182688445) {
  H_13(state, unused, out_8164365912182688445);
}
void pose_h_14(double *state, double *unused, double *out_2240330411018770776) {
  h_14(state, unused, out_2240330411018770776);
}
void pose_H_14(double *state, double *unused, double *out_411041479085210054) {
  H_14(state, unused, out_411041479085210054);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
