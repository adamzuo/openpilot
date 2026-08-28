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
void err_fun(double *nom_x, double *delta_x, double *out_3945029041185085555) {
   out_3945029041185085555[0] = delta_x[0] + nom_x[0];
   out_3945029041185085555[1] = delta_x[1] + nom_x[1];
   out_3945029041185085555[2] = delta_x[2] + nom_x[2];
   out_3945029041185085555[3] = delta_x[3] + nom_x[3];
   out_3945029041185085555[4] = delta_x[4] + nom_x[4];
   out_3945029041185085555[5] = delta_x[5] + nom_x[5];
   out_3945029041185085555[6] = delta_x[6] + nom_x[6];
   out_3945029041185085555[7] = delta_x[7] + nom_x[7];
   out_3945029041185085555[8] = delta_x[8] + nom_x[8];
   out_3945029041185085555[9] = delta_x[9] + nom_x[9];
   out_3945029041185085555[10] = delta_x[10] + nom_x[10];
   out_3945029041185085555[11] = delta_x[11] + nom_x[11];
   out_3945029041185085555[12] = delta_x[12] + nom_x[12];
   out_3945029041185085555[13] = delta_x[13] + nom_x[13];
   out_3945029041185085555[14] = delta_x[14] + nom_x[14];
   out_3945029041185085555[15] = delta_x[15] + nom_x[15];
   out_3945029041185085555[16] = delta_x[16] + nom_x[16];
   out_3945029041185085555[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_6546093222035050191) {
   out_6546093222035050191[0] = -nom_x[0] + true_x[0];
   out_6546093222035050191[1] = -nom_x[1] + true_x[1];
   out_6546093222035050191[2] = -nom_x[2] + true_x[2];
   out_6546093222035050191[3] = -nom_x[3] + true_x[3];
   out_6546093222035050191[4] = -nom_x[4] + true_x[4];
   out_6546093222035050191[5] = -nom_x[5] + true_x[5];
   out_6546093222035050191[6] = -nom_x[6] + true_x[6];
   out_6546093222035050191[7] = -nom_x[7] + true_x[7];
   out_6546093222035050191[8] = -nom_x[8] + true_x[8];
   out_6546093222035050191[9] = -nom_x[9] + true_x[9];
   out_6546093222035050191[10] = -nom_x[10] + true_x[10];
   out_6546093222035050191[11] = -nom_x[11] + true_x[11];
   out_6546093222035050191[12] = -nom_x[12] + true_x[12];
   out_6546093222035050191[13] = -nom_x[13] + true_x[13];
   out_6546093222035050191[14] = -nom_x[14] + true_x[14];
   out_6546093222035050191[15] = -nom_x[15] + true_x[15];
   out_6546093222035050191[16] = -nom_x[16] + true_x[16];
   out_6546093222035050191[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_6602467143110751234) {
   out_6602467143110751234[0] = 1.0;
   out_6602467143110751234[1] = 0.0;
   out_6602467143110751234[2] = 0.0;
   out_6602467143110751234[3] = 0.0;
   out_6602467143110751234[4] = 0.0;
   out_6602467143110751234[5] = 0.0;
   out_6602467143110751234[6] = 0.0;
   out_6602467143110751234[7] = 0.0;
   out_6602467143110751234[8] = 0.0;
   out_6602467143110751234[9] = 0.0;
   out_6602467143110751234[10] = 0.0;
   out_6602467143110751234[11] = 0.0;
   out_6602467143110751234[12] = 0.0;
   out_6602467143110751234[13] = 0.0;
   out_6602467143110751234[14] = 0.0;
   out_6602467143110751234[15] = 0.0;
   out_6602467143110751234[16] = 0.0;
   out_6602467143110751234[17] = 0.0;
   out_6602467143110751234[18] = 0.0;
   out_6602467143110751234[19] = 1.0;
   out_6602467143110751234[20] = 0.0;
   out_6602467143110751234[21] = 0.0;
   out_6602467143110751234[22] = 0.0;
   out_6602467143110751234[23] = 0.0;
   out_6602467143110751234[24] = 0.0;
   out_6602467143110751234[25] = 0.0;
   out_6602467143110751234[26] = 0.0;
   out_6602467143110751234[27] = 0.0;
   out_6602467143110751234[28] = 0.0;
   out_6602467143110751234[29] = 0.0;
   out_6602467143110751234[30] = 0.0;
   out_6602467143110751234[31] = 0.0;
   out_6602467143110751234[32] = 0.0;
   out_6602467143110751234[33] = 0.0;
   out_6602467143110751234[34] = 0.0;
   out_6602467143110751234[35] = 0.0;
   out_6602467143110751234[36] = 0.0;
   out_6602467143110751234[37] = 0.0;
   out_6602467143110751234[38] = 1.0;
   out_6602467143110751234[39] = 0.0;
   out_6602467143110751234[40] = 0.0;
   out_6602467143110751234[41] = 0.0;
   out_6602467143110751234[42] = 0.0;
   out_6602467143110751234[43] = 0.0;
   out_6602467143110751234[44] = 0.0;
   out_6602467143110751234[45] = 0.0;
   out_6602467143110751234[46] = 0.0;
   out_6602467143110751234[47] = 0.0;
   out_6602467143110751234[48] = 0.0;
   out_6602467143110751234[49] = 0.0;
   out_6602467143110751234[50] = 0.0;
   out_6602467143110751234[51] = 0.0;
   out_6602467143110751234[52] = 0.0;
   out_6602467143110751234[53] = 0.0;
   out_6602467143110751234[54] = 0.0;
   out_6602467143110751234[55] = 0.0;
   out_6602467143110751234[56] = 0.0;
   out_6602467143110751234[57] = 1.0;
   out_6602467143110751234[58] = 0.0;
   out_6602467143110751234[59] = 0.0;
   out_6602467143110751234[60] = 0.0;
   out_6602467143110751234[61] = 0.0;
   out_6602467143110751234[62] = 0.0;
   out_6602467143110751234[63] = 0.0;
   out_6602467143110751234[64] = 0.0;
   out_6602467143110751234[65] = 0.0;
   out_6602467143110751234[66] = 0.0;
   out_6602467143110751234[67] = 0.0;
   out_6602467143110751234[68] = 0.0;
   out_6602467143110751234[69] = 0.0;
   out_6602467143110751234[70] = 0.0;
   out_6602467143110751234[71] = 0.0;
   out_6602467143110751234[72] = 0.0;
   out_6602467143110751234[73] = 0.0;
   out_6602467143110751234[74] = 0.0;
   out_6602467143110751234[75] = 0.0;
   out_6602467143110751234[76] = 1.0;
   out_6602467143110751234[77] = 0.0;
   out_6602467143110751234[78] = 0.0;
   out_6602467143110751234[79] = 0.0;
   out_6602467143110751234[80] = 0.0;
   out_6602467143110751234[81] = 0.0;
   out_6602467143110751234[82] = 0.0;
   out_6602467143110751234[83] = 0.0;
   out_6602467143110751234[84] = 0.0;
   out_6602467143110751234[85] = 0.0;
   out_6602467143110751234[86] = 0.0;
   out_6602467143110751234[87] = 0.0;
   out_6602467143110751234[88] = 0.0;
   out_6602467143110751234[89] = 0.0;
   out_6602467143110751234[90] = 0.0;
   out_6602467143110751234[91] = 0.0;
   out_6602467143110751234[92] = 0.0;
   out_6602467143110751234[93] = 0.0;
   out_6602467143110751234[94] = 0.0;
   out_6602467143110751234[95] = 1.0;
   out_6602467143110751234[96] = 0.0;
   out_6602467143110751234[97] = 0.0;
   out_6602467143110751234[98] = 0.0;
   out_6602467143110751234[99] = 0.0;
   out_6602467143110751234[100] = 0.0;
   out_6602467143110751234[101] = 0.0;
   out_6602467143110751234[102] = 0.0;
   out_6602467143110751234[103] = 0.0;
   out_6602467143110751234[104] = 0.0;
   out_6602467143110751234[105] = 0.0;
   out_6602467143110751234[106] = 0.0;
   out_6602467143110751234[107] = 0.0;
   out_6602467143110751234[108] = 0.0;
   out_6602467143110751234[109] = 0.0;
   out_6602467143110751234[110] = 0.0;
   out_6602467143110751234[111] = 0.0;
   out_6602467143110751234[112] = 0.0;
   out_6602467143110751234[113] = 0.0;
   out_6602467143110751234[114] = 1.0;
   out_6602467143110751234[115] = 0.0;
   out_6602467143110751234[116] = 0.0;
   out_6602467143110751234[117] = 0.0;
   out_6602467143110751234[118] = 0.0;
   out_6602467143110751234[119] = 0.0;
   out_6602467143110751234[120] = 0.0;
   out_6602467143110751234[121] = 0.0;
   out_6602467143110751234[122] = 0.0;
   out_6602467143110751234[123] = 0.0;
   out_6602467143110751234[124] = 0.0;
   out_6602467143110751234[125] = 0.0;
   out_6602467143110751234[126] = 0.0;
   out_6602467143110751234[127] = 0.0;
   out_6602467143110751234[128] = 0.0;
   out_6602467143110751234[129] = 0.0;
   out_6602467143110751234[130] = 0.0;
   out_6602467143110751234[131] = 0.0;
   out_6602467143110751234[132] = 0.0;
   out_6602467143110751234[133] = 1.0;
   out_6602467143110751234[134] = 0.0;
   out_6602467143110751234[135] = 0.0;
   out_6602467143110751234[136] = 0.0;
   out_6602467143110751234[137] = 0.0;
   out_6602467143110751234[138] = 0.0;
   out_6602467143110751234[139] = 0.0;
   out_6602467143110751234[140] = 0.0;
   out_6602467143110751234[141] = 0.0;
   out_6602467143110751234[142] = 0.0;
   out_6602467143110751234[143] = 0.0;
   out_6602467143110751234[144] = 0.0;
   out_6602467143110751234[145] = 0.0;
   out_6602467143110751234[146] = 0.0;
   out_6602467143110751234[147] = 0.0;
   out_6602467143110751234[148] = 0.0;
   out_6602467143110751234[149] = 0.0;
   out_6602467143110751234[150] = 0.0;
   out_6602467143110751234[151] = 0.0;
   out_6602467143110751234[152] = 1.0;
   out_6602467143110751234[153] = 0.0;
   out_6602467143110751234[154] = 0.0;
   out_6602467143110751234[155] = 0.0;
   out_6602467143110751234[156] = 0.0;
   out_6602467143110751234[157] = 0.0;
   out_6602467143110751234[158] = 0.0;
   out_6602467143110751234[159] = 0.0;
   out_6602467143110751234[160] = 0.0;
   out_6602467143110751234[161] = 0.0;
   out_6602467143110751234[162] = 0.0;
   out_6602467143110751234[163] = 0.0;
   out_6602467143110751234[164] = 0.0;
   out_6602467143110751234[165] = 0.0;
   out_6602467143110751234[166] = 0.0;
   out_6602467143110751234[167] = 0.0;
   out_6602467143110751234[168] = 0.0;
   out_6602467143110751234[169] = 0.0;
   out_6602467143110751234[170] = 0.0;
   out_6602467143110751234[171] = 1.0;
   out_6602467143110751234[172] = 0.0;
   out_6602467143110751234[173] = 0.0;
   out_6602467143110751234[174] = 0.0;
   out_6602467143110751234[175] = 0.0;
   out_6602467143110751234[176] = 0.0;
   out_6602467143110751234[177] = 0.0;
   out_6602467143110751234[178] = 0.0;
   out_6602467143110751234[179] = 0.0;
   out_6602467143110751234[180] = 0.0;
   out_6602467143110751234[181] = 0.0;
   out_6602467143110751234[182] = 0.0;
   out_6602467143110751234[183] = 0.0;
   out_6602467143110751234[184] = 0.0;
   out_6602467143110751234[185] = 0.0;
   out_6602467143110751234[186] = 0.0;
   out_6602467143110751234[187] = 0.0;
   out_6602467143110751234[188] = 0.0;
   out_6602467143110751234[189] = 0.0;
   out_6602467143110751234[190] = 1.0;
   out_6602467143110751234[191] = 0.0;
   out_6602467143110751234[192] = 0.0;
   out_6602467143110751234[193] = 0.0;
   out_6602467143110751234[194] = 0.0;
   out_6602467143110751234[195] = 0.0;
   out_6602467143110751234[196] = 0.0;
   out_6602467143110751234[197] = 0.0;
   out_6602467143110751234[198] = 0.0;
   out_6602467143110751234[199] = 0.0;
   out_6602467143110751234[200] = 0.0;
   out_6602467143110751234[201] = 0.0;
   out_6602467143110751234[202] = 0.0;
   out_6602467143110751234[203] = 0.0;
   out_6602467143110751234[204] = 0.0;
   out_6602467143110751234[205] = 0.0;
   out_6602467143110751234[206] = 0.0;
   out_6602467143110751234[207] = 0.0;
   out_6602467143110751234[208] = 0.0;
   out_6602467143110751234[209] = 1.0;
   out_6602467143110751234[210] = 0.0;
   out_6602467143110751234[211] = 0.0;
   out_6602467143110751234[212] = 0.0;
   out_6602467143110751234[213] = 0.0;
   out_6602467143110751234[214] = 0.0;
   out_6602467143110751234[215] = 0.0;
   out_6602467143110751234[216] = 0.0;
   out_6602467143110751234[217] = 0.0;
   out_6602467143110751234[218] = 0.0;
   out_6602467143110751234[219] = 0.0;
   out_6602467143110751234[220] = 0.0;
   out_6602467143110751234[221] = 0.0;
   out_6602467143110751234[222] = 0.0;
   out_6602467143110751234[223] = 0.0;
   out_6602467143110751234[224] = 0.0;
   out_6602467143110751234[225] = 0.0;
   out_6602467143110751234[226] = 0.0;
   out_6602467143110751234[227] = 0.0;
   out_6602467143110751234[228] = 1.0;
   out_6602467143110751234[229] = 0.0;
   out_6602467143110751234[230] = 0.0;
   out_6602467143110751234[231] = 0.0;
   out_6602467143110751234[232] = 0.0;
   out_6602467143110751234[233] = 0.0;
   out_6602467143110751234[234] = 0.0;
   out_6602467143110751234[235] = 0.0;
   out_6602467143110751234[236] = 0.0;
   out_6602467143110751234[237] = 0.0;
   out_6602467143110751234[238] = 0.0;
   out_6602467143110751234[239] = 0.0;
   out_6602467143110751234[240] = 0.0;
   out_6602467143110751234[241] = 0.0;
   out_6602467143110751234[242] = 0.0;
   out_6602467143110751234[243] = 0.0;
   out_6602467143110751234[244] = 0.0;
   out_6602467143110751234[245] = 0.0;
   out_6602467143110751234[246] = 0.0;
   out_6602467143110751234[247] = 1.0;
   out_6602467143110751234[248] = 0.0;
   out_6602467143110751234[249] = 0.0;
   out_6602467143110751234[250] = 0.0;
   out_6602467143110751234[251] = 0.0;
   out_6602467143110751234[252] = 0.0;
   out_6602467143110751234[253] = 0.0;
   out_6602467143110751234[254] = 0.0;
   out_6602467143110751234[255] = 0.0;
   out_6602467143110751234[256] = 0.0;
   out_6602467143110751234[257] = 0.0;
   out_6602467143110751234[258] = 0.0;
   out_6602467143110751234[259] = 0.0;
   out_6602467143110751234[260] = 0.0;
   out_6602467143110751234[261] = 0.0;
   out_6602467143110751234[262] = 0.0;
   out_6602467143110751234[263] = 0.0;
   out_6602467143110751234[264] = 0.0;
   out_6602467143110751234[265] = 0.0;
   out_6602467143110751234[266] = 1.0;
   out_6602467143110751234[267] = 0.0;
   out_6602467143110751234[268] = 0.0;
   out_6602467143110751234[269] = 0.0;
   out_6602467143110751234[270] = 0.0;
   out_6602467143110751234[271] = 0.0;
   out_6602467143110751234[272] = 0.0;
   out_6602467143110751234[273] = 0.0;
   out_6602467143110751234[274] = 0.0;
   out_6602467143110751234[275] = 0.0;
   out_6602467143110751234[276] = 0.0;
   out_6602467143110751234[277] = 0.0;
   out_6602467143110751234[278] = 0.0;
   out_6602467143110751234[279] = 0.0;
   out_6602467143110751234[280] = 0.0;
   out_6602467143110751234[281] = 0.0;
   out_6602467143110751234[282] = 0.0;
   out_6602467143110751234[283] = 0.0;
   out_6602467143110751234[284] = 0.0;
   out_6602467143110751234[285] = 1.0;
   out_6602467143110751234[286] = 0.0;
   out_6602467143110751234[287] = 0.0;
   out_6602467143110751234[288] = 0.0;
   out_6602467143110751234[289] = 0.0;
   out_6602467143110751234[290] = 0.0;
   out_6602467143110751234[291] = 0.0;
   out_6602467143110751234[292] = 0.0;
   out_6602467143110751234[293] = 0.0;
   out_6602467143110751234[294] = 0.0;
   out_6602467143110751234[295] = 0.0;
   out_6602467143110751234[296] = 0.0;
   out_6602467143110751234[297] = 0.0;
   out_6602467143110751234[298] = 0.0;
   out_6602467143110751234[299] = 0.0;
   out_6602467143110751234[300] = 0.0;
   out_6602467143110751234[301] = 0.0;
   out_6602467143110751234[302] = 0.0;
   out_6602467143110751234[303] = 0.0;
   out_6602467143110751234[304] = 1.0;
   out_6602467143110751234[305] = 0.0;
   out_6602467143110751234[306] = 0.0;
   out_6602467143110751234[307] = 0.0;
   out_6602467143110751234[308] = 0.0;
   out_6602467143110751234[309] = 0.0;
   out_6602467143110751234[310] = 0.0;
   out_6602467143110751234[311] = 0.0;
   out_6602467143110751234[312] = 0.0;
   out_6602467143110751234[313] = 0.0;
   out_6602467143110751234[314] = 0.0;
   out_6602467143110751234[315] = 0.0;
   out_6602467143110751234[316] = 0.0;
   out_6602467143110751234[317] = 0.0;
   out_6602467143110751234[318] = 0.0;
   out_6602467143110751234[319] = 0.0;
   out_6602467143110751234[320] = 0.0;
   out_6602467143110751234[321] = 0.0;
   out_6602467143110751234[322] = 0.0;
   out_6602467143110751234[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_4817465423454828778) {
   out_4817465423454828778[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_4817465423454828778[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_4817465423454828778[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_4817465423454828778[3] = dt*state[12] + state[3];
   out_4817465423454828778[4] = dt*state[13] + state[4];
   out_4817465423454828778[5] = dt*state[14] + state[5];
   out_4817465423454828778[6] = state[6];
   out_4817465423454828778[7] = state[7];
   out_4817465423454828778[8] = state[8];
   out_4817465423454828778[9] = state[9];
   out_4817465423454828778[10] = state[10];
   out_4817465423454828778[11] = state[11];
   out_4817465423454828778[12] = state[12];
   out_4817465423454828778[13] = state[13];
   out_4817465423454828778[14] = state[14];
   out_4817465423454828778[15] = state[15];
   out_4817465423454828778[16] = state[16];
   out_4817465423454828778[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2368631440291744086) {
   out_2368631440291744086[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2368631440291744086[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2368631440291744086[2] = 0;
   out_2368631440291744086[3] = 0;
   out_2368631440291744086[4] = 0;
   out_2368631440291744086[5] = 0;
   out_2368631440291744086[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2368631440291744086[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2368631440291744086[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2368631440291744086[9] = 0;
   out_2368631440291744086[10] = 0;
   out_2368631440291744086[11] = 0;
   out_2368631440291744086[12] = 0;
   out_2368631440291744086[13] = 0;
   out_2368631440291744086[14] = 0;
   out_2368631440291744086[15] = 0;
   out_2368631440291744086[16] = 0;
   out_2368631440291744086[17] = 0;
   out_2368631440291744086[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2368631440291744086[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2368631440291744086[20] = 0;
   out_2368631440291744086[21] = 0;
   out_2368631440291744086[22] = 0;
   out_2368631440291744086[23] = 0;
   out_2368631440291744086[24] = 0;
   out_2368631440291744086[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2368631440291744086[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2368631440291744086[27] = 0;
   out_2368631440291744086[28] = 0;
   out_2368631440291744086[29] = 0;
   out_2368631440291744086[30] = 0;
   out_2368631440291744086[31] = 0;
   out_2368631440291744086[32] = 0;
   out_2368631440291744086[33] = 0;
   out_2368631440291744086[34] = 0;
   out_2368631440291744086[35] = 0;
   out_2368631440291744086[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2368631440291744086[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2368631440291744086[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2368631440291744086[39] = 0;
   out_2368631440291744086[40] = 0;
   out_2368631440291744086[41] = 0;
   out_2368631440291744086[42] = 0;
   out_2368631440291744086[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2368631440291744086[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2368631440291744086[45] = 0;
   out_2368631440291744086[46] = 0;
   out_2368631440291744086[47] = 0;
   out_2368631440291744086[48] = 0;
   out_2368631440291744086[49] = 0;
   out_2368631440291744086[50] = 0;
   out_2368631440291744086[51] = 0;
   out_2368631440291744086[52] = 0;
   out_2368631440291744086[53] = 0;
   out_2368631440291744086[54] = 0;
   out_2368631440291744086[55] = 0;
   out_2368631440291744086[56] = 0;
   out_2368631440291744086[57] = 1;
   out_2368631440291744086[58] = 0;
   out_2368631440291744086[59] = 0;
   out_2368631440291744086[60] = 0;
   out_2368631440291744086[61] = 0;
   out_2368631440291744086[62] = 0;
   out_2368631440291744086[63] = 0;
   out_2368631440291744086[64] = 0;
   out_2368631440291744086[65] = 0;
   out_2368631440291744086[66] = dt;
   out_2368631440291744086[67] = 0;
   out_2368631440291744086[68] = 0;
   out_2368631440291744086[69] = 0;
   out_2368631440291744086[70] = 0;
   out_2368631440291744086[71] = 0;
   out_2368631440291744086[72] = 0;
   out_2368631440291744086[73] = 0;
   out_2368631440291744086[74] = 0;
   out_2368631440291744086[75] = 0;
   out_2368631440291744086[76] = 1;
   out_2368631440291744086[77] = 0;
   out_2368631440291744086[78] = 0;
   out_2368631440291744086[79] = 0;
   out_2368631440291744086[80] = 0;
   out_2368631440291744086[81] = 0;
   out_2368631440291744086[82] = 0;
   out_2368631440291744086[83] = 0;
   out_2368631440291744086[84] = 0;
   out_2368631440291744086[85] = dt;
   out_2368631440291744086[86] = 0;
   out_2368631440291744086[87] = 0;
   out_2368631440291744086[88] = 0;
   out_2368631440291744086[89] = 0;
   out_2368631440291744086[90] = 0;
   out_2368631440291744086[91] = 0;
   out_2368631440291744086[92] = 0;
   out_2368631440291744086[93] = 0;
   out_2368631440291744086[94] = 0;
   out_2368631440291744086[95] = 1;
   out_2368631440291744086[96] = 0;
   out_2368631440291744086[97] = 0;
   out_2368631440291744086[98] = 0;
   out_2368631440291744086[99] = 0;
   out_2368631440291744086[100] = 0;
   out_2368631440291744086[101] = 0;
   out_2368631440291744086[102] = 0;
   out_2368631440291744086[103] = 0;
   out_2368631440291744086[104] = dt;
   out_2368631440291744086[105] = 0;
   out_2368631440291744086[106] = 0;
   out_2368631440291744086[107] = 0;
   out_2368631440291744086[108] = 0;
   out_2368631440291744086[109] = 0;
   out_2368631440291744086[110] = 0;
   out_2368631440291744086[111] = 0;
   out_2368631440291744086[112] = 0;
   out_2368631440291744086[113] = 0;
   out_2368631440291744086[114] = 1;
   out_2368631440291744086[115] = 0;
   out_2368631440291744086[116] = 0;
   out_2368631440291744086[117] = 0;
   out_2368631440291744086[118] = 0;
   out_2368631440291744086[119] = 0;
   out_2368631440291744086[120] = 0;
   out_2368631440291744086[121] = 0;
   out_2368631440291744086[122] = 0;
   out_2368631440291744086[123] = 0;
   out_2368631440291744086[124] = 0;
   out_2368631440291744086[125] = 0;
   out_2368631440291744086[126] = 0;
   out_2368631440291744086[127] = 0;
   out_2368631440291744086[128] = 0;
   out_2368631440291744086[129] = 0;
   out_2368631440291744086[130] = 0;
   out_2368631440291744086[131] = 0;
   out_2368631440291744086[132] = 0;
   out_2368631440291744086[133] = 1;
   out_2368631440291744086[134] = 0;
   out_2368631440291744086[135] = 0;
   out_2368631440291744086[136] = 0;
   out_2368631440291744086[137] = 0;
   out_2368631440291744086[138] = 0;
   out_2368631440291744086[139] = 0;
   out_2368631440291744086[140] = 0;
   out_2368631440291744086[141] = 0;
   out_2368631440291744086[142] = 0;
   out_2368631440291744086[143] = 0;
   out_2368631440291744086[144] = 0;
   out_2368631440291744086[145] = 0;
   out_2368631440291744086[146] = 0;
   out_2368631440291744086[147] = 0;
   out_2368631440291744086[148] = 0;
   out_2368631440291744086[149] = 0;
   out_2368631440291744086[150] = 0;
   out_2368631440291744086[151] = 0;
   out_2368631440291744086[152] = 1;
   out_2368631440291744086[153] = 0;
   out_2368631440291744086[154] = 0;
   out_2368631440291744086[155] = 0;
   out_2368631440291744086[156] = 0;
   out_2368631440291744086[157] = 0;
   out_2368631440291744086[158] = 0;
   out_2368631440291744086[159] = 0;
   out_2368631440291744086[160] = 0;
   out_2368631440291744086[161] = 0;
   out_2368631440291744086[162] = 0;
   out_2368631440291744086[163] = 0;
   out_2368631440291744086[164] = 0;
   out_2368631440291744086[165] = 0;
   out_2368631440291744086[166] = 0;
   out_2368631440291744086[167] = 0;
   out_2368631440291744086[168] = 0;
   out_2368631440291744086[169] = 0;
   out_2368631440291744086[170] = 0;
   out_2368631440291744086[171] = 1;
   out_2368631440291744086[172] = 0;
   out_2368631440291744086[173] = 0;
   out_2368631440291744086[174] = 0;
   out_2368631440291744086[175] = 0;
   out_2368631440291744086[176] = 0;
   out_2368631440291744086[177] = 0;
   out_2368631440291744086[178] = 0;
   out_2368631440291744086[179] = 0;
   out_2368631440291744086[180] = 0;
   out_2368631440291744086[181] = 0;
   out_2368631440291744086[182] = 0;
   out_2368631440291744086[183] = 0;
   out_2368631440291744086[184] = 0;
   out_2368631440291744086[185] = 0;
   out_2368631440291744086[186] = 0;
   out_2368631440291744086[187] = 0;
   out_2368631440291744086[188] = 0;
   out_2368631440291744086[189] = 0;
   out_2368631440291744086[190] = 1;
   out_2368631440291744086[191] = 0;
   out_2368631440291744086[192] = 0;
   out_2368631440291744086[193] = 0;
   out_2368631440291744086[194] = 0;
   out_2368631440291744086[195] = 0;
   out_2368631440291744086[196] = 0;
   out_2368631440291744086[197] = 0;
   out_2368631440291744086[198] = 0;
   out_2368631440291744086[199] = 0;
   out_2368631440291744086[200] = 0;
   out_2368631440291744086[201] = 0;
   out_2368631440291744086[202] = 0;
   out_2368631440291744086[203] = 0;
   out_2368631440291744086[204] = 0;
   out_2368631440291744086[205] = 0;
   out_2368631440291744086[206] = 0;
   out_2368631440291744086[207] = 0;
   out_2368631440291744086[208] = 0;
   out_2368631440291744086[209] = 1;
   out_2368631440291744086[210] = 0;
   out_2368631440291744086[211] = 0;
   out_2368631440291744086[212] = 0;
   out_2368631440291744086[213] = 0;
   out_2368631440291744086[214] = 0;
   out_2368631440291744086[215] = 0;
   out_2368631440291744086[216] = 0;
   out_2368631440291744086[217] = 0;
   out_2368631440291744086[218] = 0;
   out_2368631440291744086[219] = 0;
   out_2368631440291744086[220] = 0;
   out_2368631440291744086[221] = 0;
   out_2368631440291744086[222] = 0;
   out_2368631440291744086[223] = 0;
   out_2368631440291744086[224] = 0;
   out_2368631440291744086[225] = 0;
   out_2368631440291744086[226] = 0;
   out_2368631440291744086[227] = 0;
   out_2368631440291744086[228] = 1;
   out_2368631440291744086[229] = 0;
   out_2368631440291744086[230] = 0;
   out_2368631440291744086[231] = 0;
   out_2368631440291744086[232] = 0;
   out_2368631440291744086[233] = 0;
   out_2368631440291744086[234] = 0;
   out_2368631440291744086[235] = 0;
   out_2368631440291744086[236] = 0;
   out_2368631440291744086[237] = 0;
   out_2368631440291744086[238] = 0;
   out_2368631440291744086[239] = 0;
   out_2368631440291744086[240] = 0;
   out_2368631440291744086[241] = 0;
   out_2368631440291744086[242] = 0;
   out_2368631440291744086[243] = 0;
   out_2368631440291744086[244] = 0;
   out_2368631440291744086[245] = 0;
   out_2368631440291744086[246] = 0;
   out_2368631440291744086[247] = 1;
   out_2368631440291744086[248] = 0;
   out_2368631440291744086[249] = 0;
   out_2368631440291744086[250] = 0;
   out_2368631440291744086[251] = 0;
   out_2368631440291744086[252] = 0;
   out_2368631440291744086[253] = 0;
   out_2368631440291744086[254] = 0;
   out_2368631440291744086[255] = 0;
   out_2368631440291744086[256] = 0;
   out_2368631440291744086[257] = 0;
   out_2368631440291744086[258] = 0;
   out_2368631440291744086[259] = 0;
   out_2368631440291744086[260] = 0;
   out_2368631440291744086[261] = 0;
   out_2368631440291744086[262] = 0;
   out_2368631440291744086[263] = 0;
   out_2368631440291744086[264] = 0;
   out_2368631440291744086[265] = 0;
   out_2368631440291744086[266] = 1;
   out_2368631440291744086[267] = 0;
   out_2368631440291744086[268] = 0;
   out_2368631440291744086[269] = 0;
   out_2368631440291744086[270] = 0;
   out_2368631440291744086[271] = 0;
   out_2368631440291744086[272] = 0;
   out_2368631440291744086[273] = 0;
   out_2368631440291744086[274] = 0;
   out_2368631440291744086[275] = 0;
   out_2368631440291744086[276] = 0;
   out_2368631440291744086[277] = 0;
   out_2368631440291744086[278] = 0;
   out_2368631440291744086[279] = 0;
   out_2368631440291744086[280] = 0;
   out_2368631440291744086[281] = 0;
   out_2368631440291744086[282] = 0;
   out_2368631440291744086[283] = 0;
   out_2368631440291744086[284] = 0;
   out_2368631440291744086[285] = 1;
   out_2368631440291744086[286] = 0;
   out_2368631440291744086[287] = 0;
   out_2368631440291744086[288] = 0;
   out_2368631440291744086[289] = 0;
   out_2368631440291744086[290] = 0;
   out_2368631440291744086[291] = 0;
   out_2368631440291744086[292] = 0;
   out_2368631440291744086[293] = 0;
   out_2368631440291744086[294] = 0;
   out_2368631440291744086[295] = 0;
   out_2368631440291744086[296] = 0;
   out_2368631440291744086[297] = 0;
   out_2368631440291744086[298] = 0;
   out_2368631440291744086[299] = 0;
   out_2368631440291744086[300] = 0;
   out_2368631440291744086[301] = 0;
   out_2368631440291744086[302] = 0;
   out_2368631440291744086[303] = 0;
   out_2368631440291744086[304] = 1;
   out_2368631440291744086[305] = 0;
   out_2368631440291744086[306] = 0;
   out_2368631440291744086[307] = 0;
   out_2368631440291744086[308] = 0;
   out_2368631440291744086[309] = 0;
   out_2368631440291744086[310] = 0;
   out_2368631440291744086[311] = 0;
   out_2368631440291744086[312] = 0;
   out_2368631440291744086[313] = 0;
   out_2368631440291744086[314] = 0;
   out_2368631440291744086[315] = 0;
   out_2368631440291744086[316] = 0;
   out_2368631440291744086[317] = 0;
   out_2368631440291744086[318] = 0;
   out_2368631440291744086[319] = 0;
   out_2368631440291744086[320] = 0;
   out_2368631440291744086[321] = 0;
   out_2368631440291744086[322] = 0;
   out_2368631440291744086[323] = 1;
}
void h_4(double *state, double *unused, double *out_6519370834537087993) {
   out_6519370834537087993[0] = state[6] + state[9];
   out_6519370834537087993[1] = state[7] + state[10];
   out_6519370834537087993[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_8570054851807851129) {
   out_8570054851807851129[0] = 0;
   out_8570054851807851129[1] = 0;
   out_8570054851807851129[2] = 0;
   out_8570054851807851129[3] = 0;
   out_8570054851807851129[4] = 0;
   out_8570054851807851129[5] = 0;
   out_8570054851807851129[6] = 1;
   out_8570054851807851129[7] = 0;
   out_8570054851807851129[8] = 0;
   out_8570054851807851129[9] = 1;
   out_8570054851807851129[10] = 0;
   out_8570054851807851129[11] = 0;
   out_8570054851807851129[12] = 0;
   out_8570054851807851129[13] = 0;
   out_8570054851807851129[14] = 0;
   out_8570054851807851129[15] = 0;
   out_8570054851807851129[16] = 0;
   out_8570054851807851129[17] = 0;
   out_8570054851807851129[18] = 0;
   out_8570054851807851129[19] = 0;
   out_8570054851807851129[20] = 0;
   out_8570054851807851129[21] = 0;
   out_8570054851807851129[22] = 0;
   out_8570054851807851129[23] = 0;
   out_8570054851807851129[24] = 0;
   out_8570054851807851129[25] = 1;
   out_8570054851807851129[26] = 0;
   out_8570054851807851129[27] = 0;
   out_8570054851807851129[28] = 1;
   out_8570054851807851129[29] = 0;
   out_8570054851807851129[30] = 0;
   out_8570054851807851129[31] = 0;
   out_8570054851807851129[32] = 0;
   out_8570054851807851129[33] = 0;
   out_8570054851807851129[34] = 0;
   out_8570054851807851129[35] = 0;
   out_8570054851807851129[36] = 0;
   out_8570054851807851129[37] = 0;
   out_8570054851807851129[38] = 0;
   out_8570054851807851129[39] = 0;
   out_8570054851807851129[40] = 0;
   out_8570054851807851129[41] = 0;
   out_8570054851807851129[42] = 0;
   out_8570054851807851129[43] = 0;
   out_8570054851807851129[44] = 1;
   out_8570054851807851129[45] = 0;
   out_8570054851807851129[46] = 0;
   out_8570054851807851129[47] = 1;
   out_8570054851807851129[48] = 0;
   out_8570054851807851129[49] = 0;
   out_8570054851807851129[50] = 0;
   out_8570054851807851129[51] = 0;
   out_8570054851807851129[52] = 0;
   out_8570054851807851129[53] = 0;
}
void h_10(double *state, double *unused, double *out_5225915248190564975) {
   out_5225915248190564975[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5225915248190564975[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5225915248190564975[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8776126540151652793) {
   out_8776126540151652793[0] = 0;
   out_8776126540151652793[1] = 9.8100000000000005*cos(state[1]);
   out_8776126540151652793[2] = 0;
   out_8776126540151652793[3] = 0;
   out_8776126540151652793[4] = -state[8];
   out_8776126540151652793[5] = state[7];
   out_8776126540151652793[6] = 0;
   out_8776126540151652793[7] = state[5];
   out_8776126540151652793[8] = -state[4];
   out_8776126540151652793[9] = 0;
   out_8776126540151652793[10] = 0;
   out_8776126540151652793[11] = 0;
   out_8776126540151652793[12] = 1;
   out_8776126540151652793[13] = 0;
   out_8776126540151652793[14] = 0;
   out_8776126540151652793[15] = 1;
   out_8776126540151652793[16] = 0;
   out_8776126540151652793[17] = 0;
   out_8776126540151652793[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8776126540151652793[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8776126540151652793[20] = 0;
   out_8776126540151652793[21] = state[8];
   out_8776126540151652793[22] = 0;
   out_8776126540151652793[23] = -state[6];
   out_8776126540151652793[24] = -state[5];
   out_8776126540151652793[25] = 0;
   out_8776126540151652793[26] = state[3];
   out_8776126540151652793[27] = 0;
   out_8776126540151652793[28] = 0;
   out_8776126540151652793[29] = 0;
   out_8776126540151652793[30] = 0;
   out_8776126540151652793[31] = 1;
   out_8776126540151652793[32] = 0;
   out_8776126540151652793[33] = 0;
   out_8776126540151652793[34] = 1;
   out_8776126540151652793[35] = 0;
   out_8776126540151652793[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8776126540151652793[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8776126540151652793[38] = 0;
   out_8776126540151652793[39] = -state[7];
   out_8776126540151652793[40] = state[6];
   out_8776126540151652793[41] = 0;
   out_8776126540151652793[42] = state[4];
   out_8776126540151652793[43] = -state[3];
   out_8776126540151652793[44] = 0;
   out_8776126540151652793[45] = 0;
   out_8776126540151652793[46] = 0;
   out_8776126540151652793[47] = 0;
   out_8776126540151652793[48] = 0;
   out_8776126540151652793[49] = 0;
   out_8776126540151652793[50] = 1;
   out_8776126540151652793[51] = 0;
   out_8776126540151652793[52] = 0;
   out_8776126540151652793[53] = 1;
}
void h_13(double *state, double *unused, double *out_5413857800886136922) {
   out_5413857800886136922[0] = state[3];
   out_5413857800886136922[1] = state[4];
   out_5413857800886136922[2] = state[5];
}
void H_13(double *state, double *unused, double *out_2266058013584999558) {
   out_2266058013584999558[0] = 0;
   out_2266058013584999558[1] = 0;
   out_2266058013584999558[2] = 0;
   out_2266058013584999558[3] = 1;
   out_2266058013584999558[4] = 0;
   out_2266058013584999558[5] = 0;
   out_2266058013584999558[6] = 0;
   out_2266058013584999558[7] = 0;
   out_2266058013584999558[8] = 0;
   out_2266058013584999558[9] = 0;
   out_2266058013584999558[10] = 0;
   out_2266058013584999558[11] = 0;
   out_2266058013584999558[12] = 0;
   out_2266058013584999558[13] = 0;
   out_2266058013584999558[14] = 0;
   out_2266058013584999558[15] = 0;
   out_2266058013584999558[16] = 0;
   out_2266058013584999558[17] = 0;
   out_2266058013584999558[18] = 0;
   out_2266058013584999558[19] = 0;
   out_2266058013584999558[20] = 0;
   out_2266058013584999558[21] = 0;
   out_2266058013584999558[22] = 1;
   out_2266058013584999558[23] = 0;
   out_2266058013584999558[24] = 0;
   out_2266058013584999558[25] = 0;
   out_2266058013584999558[26] = 0;
   out_2266058013584999558[27] = 0;
   out_2266058013584999558[28] = 0;
   out_2266058013584999558[29] = 0;
   out_2266058013584999558[30] = 0;
   out_2266058013584999558[31] = 0;
   out_2266058013584999558[32] = 0;
   out_2266058013584999558[33] = 0;
   out_2266058013584999558[34] = 0;
   out_2266058013584999558[35] = 0;
   out_2266058013584999558[36] = 0;
   out_2266058013584999558[37] = 0;
   out_2266058013584999558[38] = 0;
   out_2266058013584999558[39] = 0;
   out_2266058013584999558[40] = 0;
   out_2266058013584999558[41] = 1;
   out_2266058013584999558[42] = 0;
   out_2266058013584999558[43] = 0;
   out_2266058013584999558[44] = 0;
   out_2266058013584999558[45] = 0;
   out_2266058013584999558[46] = 0;
   out_2266058013584999558[47] = 0;
   out_2266058013584999558[48] = 0;
   out_2266058013584999558[49] = 0;
   out_2266058013584999558[50] = 0;
   out_2266058013584999558[51] = 0;
   out_2266058013584999558[52] = 0;
   out_2266058013584999558[53] = 0;
}
void h_14(double *state, double *unused, double *out_4123985795765896642) {
   out_4123985795765896642[0] = state[6];
   out_4123985795765896642[1] = state[7];
   out_4123985795765896642[2] = state[8];
}
void H_14(double *state, double *unused, double *out_5913448365562215958) {
   out_5913448365562215958[0] = 0;
   out_5913448365562215958[1] = 0;
   out_5913448365562215958[2] = 0;
   out_5913448365562215958[3] = 0;
   out_5913448365562215958[4] = 0;
   out_5913448365562215958[5] = 0;
   out_5913448365562215958[6] = 1;
   out_5913448365562215958[7] = 0;
   out_5913448365562215958[8] = 0;
   out_5913448365562215958[9] = 0;
   out_5913448365562215958[10] = 0;
   out_5913448365562215958[11] = 0;
   out_5913448365562215958[12] = 0;
   out_5913448365562215958[13] = 0;
   out_5913448365562215958[14] = 0;
   out_5913448365562215958[15] = 0;
   out_5913448365562215958[16] = 0;
   out_5913448365562215958[17] = 0;
   out_5913448365562215958[18] = 0;
   out_5913448365562215958[19] = 0;
   out_5913448365562215958[20] = 0;
   out_5913448365562215958[21] = 0;
   out_5913448365562215958[22] = 0;
   out_5913448365562215958[23] = 0;
   out_5913448365562215958[24] = 0;
   out_5913448365562215958[25] = 1;
   out_5913448365562215958[26] = 0;
   out_5913448365562215958[27] = 0;
   out_5913448365562215958[28] = 0;
   out_5913448365562215958[29] = 0;
   out_5913448365562215958[30] = 0;
   out_5913448365562215958[31] = 0;
   out_5913448365562215958[32] = 0;
   out_5913448365562215958[33] = 0;
   out_5913448365562215958[34] = 0;
   out_5913448365562215958[35] = 0;
   out_5913448365562215958[36] = 0;
   out_5913448365562215958[37] = 0;
   out_5913448365562215958[38] = 0;
   out_5913448365562215958[39] = 0;
   out_5913448365562215958[40] = 0;
   out_5913448365562215958[41] = 0;
   out_5913448365562215958[42] = 0;
   out_5913448365562215958[43] = 0;
   out_5913448365562215958[44] = 1;
   out_5913448365562215958[45] = 0;
   out_5913448365562215958[46] = 0;
   out_5913448365562215958[47] = 0;
   out_5913448365562215958[48] = 0;
   out_5913448365562215958[49] = 0;
   out_5913448365562215958[50] = 0;
   out_5913448365562215958[51] = 0;
   out_5913448365562215958[52] = 0;
   out_5913448365562215958[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_3945029041185085555) {
  err_fun(nom_x, delta_x, out_3945029041185085555);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_6546093222035050191) {
  inv_err_fun(nom_x, true_x, out_6546093222035050191);
}
void pose_H_mod_fun(double *state, double *out_6602467143110751234) {
  H_mod_fun(state, out_6602467143110751234);
}
void pose_f_fun(double *state, double dt, double *out_4817465423454828778) {
  f_fun(state,  dt, out_4817465423454828778);
}
void pose_F_fun(double *state, double dt, double *out_2368631440291744086) {
  F_fun(state,  dt, out_2368631440291744086);
}
void pose_h_4(double *state, double *unused, double *out_6519370834537087993) {
  h_4(state, unused, out_6519370834537087993);
}
void pose_H_4(double *state, double *unused, double *out_8570054851807851129) {
  H_4(state, unused, out_8570054851807851129);
}
void pose_h_10(double *state, double *unused, double *out_5225915248190564975) {
  h_10(state, unused, out_5225915248190564975);
}
void pose_H_10(double *state, double *unused, double *out_8776126540151652793) {
  H_10(state, unused, out_8776126540151652793);
}
void pose_h_13(double *state, double *unused, double *out_5413857800886136922) {
  h_13(state, unused, out_5413857800886136922);
}
void pose_H_13(double *state, double *unused, double *out_2266058013584999558) {
  H_13(state, unused, out_2266058013584999558);
}
void pose_h_14(double *state, double *unused, double *out_4123985795765896642) {
  h_14(state, unused, out_4123985795765896642);
}
void pose_H_14(double *state, double *unused, double *out_5913448365562215958) {
  H_14(state, unused, out_5913448365562215958);
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
