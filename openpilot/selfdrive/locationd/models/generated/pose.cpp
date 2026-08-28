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
void err_fun(double *nom_x, double *delta_x, double *out_8980418967492834945) {
   out_8980418967492834945[0] = delta_x[0] + nom_x[0];
   out_8980418967492834945[1] = delta_x[1] + nom_x[1];
   out_8980418967492834945[2] = delta_x[2] + nom_x[2];
   out_8980418967492834945[3] = delta_x[3] + nom_x[3];
   out_8980418967492834945[4] = delta_x[4] + nom_x[4];
   out_8980418967492834945[5] = delta_x[5] + nom_x[5];
   out_8980418967492834945[6] = delta_x[6] + nom_x[6];
   out_8980418967492834945[7] = delta_x[7] + nom_x[7];
   out_8980418967492834945[8] = delta_x[8] + nom_x[8];
   out_8980418967492834945[9] = delta_x[9] + nom_x[9];
   out_8980418967492834945[10] = delta_x[10] + nom_x[10];
   out_8980418967492834945[11] = delta_x[11] + nom_x[11];
   out_8980418967492834945[12] = delta_x[12] + nom_x[12];
   out_8980418967492834945[13] = delta_x[13] + nom_x[13];
   out_8980418967492834945[14] = delta_x[14] + nom_x[14];
   out_8980418967492834945[15] = delta_x[15] + nom_x[15];
   out_8980418967492834945[16] = delta_x[16] + nom_x[16];
   out_8980418967492834945[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5853420954923124138) {
   out_5853420954923124138[0] = -nom_x[0] + true_x[0];
   out_5853420954923124138[1] = -nom_x[1] + true_x[1];
   out_5853420954923124138[2] = -nom_x[2] + true_x[2];
   out_5853420954923124138[3] = -nom_x[3] + true_x[3];
   out_5853420954923124138[4] = -nom_x[4] + true_x[4];
   out_5853420954923124138[5] = -nom_x[5] + true_x[5];
   out_5853420954923124138[6] = -nom_x[6] + true_x[6];
   out_5853420954923124138[7] = -nom_x[7] + true_x[7];
   out_5853420954923124138[8] = -nom_x[8] + true_x[8];
   out_5853420954923124138[9] = -nom_x[9] + true_x[9];
   out_5853420954923124138[10] = -nom_x[10] + true_x[10];
   out_5853420954923124138[11] = -nom_x[11] + true_x[11];
   out_5853420954923124138[12] = -nom_x[12] + true_x[12];
   out_5853420954923124138[13] = -nom_x[13] + true_x[13];
   out_5853420954923124138[14] = -nom_x[14] + true_x[14];
   out_5853420954923124138[15] = -nom_x[15] + true_x[15];
   out_5853420954923124138[16] = -nom_x[16] + true_x[16];
   out_5853420954923124138[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_4198711007403848478) {
   out_4198711007403848478[0] = 1.0;
   out_4198711007403848478[1] = 0.0;
   out_4198711007403848478[2] = 0.0;
   out_4198711007403848478[3] = 0.0;
   out_4198711007403848478[4] = 0.0;
   out_4198711007403848478[5] = 0.0;
   out_4198711007403848478[6] = 0.0;
   out_4198711007403848478[7] = 0.0;
   out_4198711007403848478[8] = 0.0;
   out_4198711007403848478[9] = 0.0;
   out_4198711007403848478[10] = 0.0;
   out_4198711007403848478[11] = 0.0;
   out_4198711007403848478[12] = 0.0;
   out_4198711007403848478[13] = 0.0;
   out_4198711007403848478[14] = 0.0;
   out_4198711007403848478[15] = 0.0;
   out_4198711007403848478[16] = 0.0;
   out_4198711007403848478[17] = 0.0;
   out_4198711007403848478[18] = 0.0;
   out_4198711007403848478[19] = 1.0;
   out_4198711007403848478[20] = 0.0;
   out_4198711007403848478[21] = 0.0;
   out_4198711007403848478[22] = 0.0;
   out_4198711007403848478[23] = 0.0;
   out_4198711007403848478[24] = 0.0;
   out_4198711007403848478[25] = 0.0;
   out_4198711007403848478[26] = 0.0;
   out_4198711007403848478[27] = 0.0;
   out_4198711007403848478[28] = 0.0;
   out_4198711007403848478[29] = 0.0;
   out_4198711007403848478[30] = 0.0;
   out_4198711007403848478[31] = 0.0;
   out_4198711007403848478[32] = 0.0;
   out_4198711007403848478[33] = 0.0;
   out_4198711007403848478[34] = 0.0;
   out_4198711007403848478[35] = 0.0;
   out_4198711007403848478[36] = 0.0;
   out_4198711007403848478[37] = 0.0;
   out_4198711007403848478[38] = 1.0;
   out_4198711007403848478[39] = 0.0;
   out_4198711007403848478[40] = 0.0;
   out_4198711007403848478[41] = 0.0;
   out_4198711007403848478[42] = 0.0;
   out_4198711007403848478[43] = 0.0;
   out_4198711007403848478[44] = 0.0;
   out_4198711007403848478[45] = 0.0;
   out_4198711007403848478[46] = 0.0;
   out_4198711007403848478[47] = 0.0;
   out_4198711007403848478[48] = 0.0;
   out_4198711007403848478[49] = 0.0;
   out_4198711007403848478[50] = 0.0;
   out_4198711007403848478[51] = 0.0;
   out_4198711007403848478[52] = 0.0;
   out_4198711007403848478[53] = 0.0;
   out_4198711007403848478[54] = 0.0;
   out_4198711007403848478[55] = 0.0;
   out_4198711007403848478[56] = 0.0;
   out_4198711007403848478[57] = 1.0;
   out_4198711007403848478[58] = 0.0;
   out_4198711007403848478[59] = 0.0;
   out_4198711007403848478[60] = 0.0;
   out_4198711007403848478[61] = 0.0;
   out_4198711007403848478[62] = 0.0;
   out_4198711007403848478[63] = 0.0;
   out_4198711007403848478[64] = 0.0;
   out_4198711007403848478[65] = 0.0;
   out_4198711007403848478[66] = 0.0;
   out_4198711007403848478[67] = 0.0;
   out_4198711007403848478[68] = 0.0;
   out_4198711007403848478[69] = 0.0;
   out_4198711007403848478[70] = 0.0;
   out_4198711007403848478[71] = 0.0;
   out_4198711007403848478[72] = 0.0;
   out_4198711007403848478[73] = 0.0;
   out_4198711007403848478[74] = 0.0;
   out_4198711007403848478[75] = 0.0;
   out_4198711007403848478[76] = 1.0;
   out_4198711007403848478[77] = 0.0;
   out_4198711007403848478[78] = 0.0;
   out_4198711007403848478[79] = 0.0;
   out_4198711007403848478[80] = 0.0;
   out_4198711007403848478[81] = 0.0;
   out_4198711007403848478[82] = 0.0;
   out_4198711007403848478[83] = 0.0;
   out_4198711007403848478[84] = 0.0;
   out_4198711007403848478[85] = 0.0;
   out_4198711007403848478[86] = 0.0;
   out_4198711007403848478[87] = 0.0;
   out_4198711007403848478[88] = 0.0;
   out_4198711007403848478[89] = 0.0;
   out_4198711007403848478[90] = 0.0;
   out_4198711007403848478[91] = 0.0;
   out_4198711007403848478[92] = 0.0;
   out_4198711007403848478[93] = 0.0;
   out_4198711007403848478[94] = 0.0;
   out_4198711007403848478[95] = 1.0;
   out_4198711007403848478[96] = 0.0;
   out_4198711007403848478[97] = 0.0;
   out_4198711007403848478[98] = 0.0;
   out_4198711007403848478[99] = 0.0;
   out_4198711007403848478[100] = 0.0;
   out_4198711007403848478[101] = 0.0;
   out_4198711007403848478[102] = 0.0;
   out_4198711007403848478[103] = 0.0;
   out_4198711007403848478[104] = 0.0;
   out_4198711007403848478[105] = 0.0;
   out_4198711007403848478[106] = 0.0;
   out_4198711007403848478[107] = 0.0;
   out_4198711007403848478[108] = 0.0;
   out_4198711007403848478[109] = 0.0;
   out_4198711007403848478[110] = 0.0;
   out_4198711007403848478[111] = 0.0;
   out_4198711007403848478[112] = 0.0;
   out_4198711007403848478[113] = 0.0;
   out_4198711007403848478[114] = 1.0;
   out_4198711007403848478[115] = 0.0;
   out_4198711007403848478[116] = 0.0;
   out_4198711007403848478[117] = 0.0;
   out_4198711007403848478[118] = 0.0;
   out_4198711007403848478[119] = 0.0;
   out_4198711007403848478[120] = 0.0;
   out_4198711007403848478[121] = 0.0;
   out_4198711007403848478[122] = 0.0;
   out_4198711007403848478[123] = 0.0;
   out_4198711007403848478[124] = 0.0;
   out_4198711007403848478[125] = 0.0;
   out_4198711007403848478[126] = 0.0;
   out_4198711007403848478[127] = 0.0;
   out_4198711007403848478[128] = 0.0;
   out_4198711007403848478[129] = 0.0;
   out_4198711007403848478[130] = 0.0;
   out_4198711007403848478[131] = 0.0;
   out_4198711007403848478[132] = 0.0;
   out_4198711007403848478[133] = 1.0;
   out_4198711007403848478[134] = 0.0;
   out_4198711007403848478[135] = 0.0;
   out_4198711007403848478[136] = 0.0;
   out_4198711007403848478[137] = 0.0;
   out_4198711007403848478[138] = 0.0;
   out_4198711007403848478[139] = 0.0;
   out_4198711007403848478[140] = 0.0;
   out_4198711007403848478[141] = 0.0;
   out_4198711007403848478[142] = 0.0;
   out_4198711007403848478[143] = 0.0;
   out_4198711007403848478[144] = 0.0;
   out_4198711007403848478[145] = 0.0;
   out_4198711007403848478[146] = 0.0;
   out_4198711007403848478[147] = 0.0;
   out_4198711007403848478[148] = 0.0;
   out_4198711007403848478[149] = 0.0;
   out_4198711007403848478[150] = 0.0;
   out_4198711007403848478[151] = 0.0;
   out_4198711007403848478[152] = 1.0;
   out_4198711007403848478[153] = 0.0;
   out_4198711007403848478[154] = 0.0;
   out_4198711007403848478[155] = 0.0;
   out_4198711007403848478[156] = 0.0;
   out_4198711007403848478[157] = 0.0;
   out_4198711007403848478[158] = 0.0;
   out_4198711007403848478[159] = 0.0;
   out_4198711007403848478[160] = 0.0;
   out_4198711007403848478[161] = 0.0;
   out_4198711007403848478[162] = 0.0;
   out_4198711007403848478[163] = 0.0;
   out_4198711007403848478[164] = 0.0;
   out_4198711007403848478[165] = 0.0;
   out_4198711007403848478[166] = 0.0;
   out_4198711007403848478[167] = 0.0;
   out_4198711007403848478[168] = 0.0;
   out_4198711007403848478[169] = 0.0;
   out_4198711007403848478[170] = 0.0;
   out_4198711007403848478[171] = 1.0;
   out_4198711007403848478[172] = 0.0;
   out_4198711007403848478[173] = 0.0;
   out_4198711007403848478[174] = 0.0;
   out_4198711007403848478[175] = 0.0;
   out_4198711007403848478[176] = 0.0;
   out_4198711007403848478[177] = 0.0;
   out_4198711007403848478[178] = 0.0;
   out_4198711007403848478[179] = 0.0;
   out_4198711007403848478[180] = 0.0;
   out_4198711007403848478[181] = 0.0;
   out_4198711007403848478[182] = 0.0;
   out_4198711007403848478[183] = 0.0;
   out_4198711007403848478[184] = 0.0;
   out_4198711007403848478[185] = 0.0;
   out_4198711007403848478[186] = 0.0;
   out_4198711007403848478[187] = 0.0;
   out_4198711007403848478[188] = 0.0;
   out_4198711007403848478[189] = 0.0;
   out_4198711007403848478[190] = 1.0;
   out_4198711007403848478[191] = 0.0;
   out_4198711007403848478[192] = 0.0;
   out_4198711007403848478[193] = 0.0;
   out_4198711007403848478[194] = 0.0;
   out_4198711007403848478[195] = 0.0;
   out_4198711007403848478[196] = 0.0;
   out_4198711007403848478[197] = 0.0;
   out_4198711007403848478[198] = 0.0;
   out_4198711007403848478[199] = 0.0;
   out_4198711007403848478[200] = 0.0;
   out_4198711007403848478[201] = 0.0;
   out_4198711007403848478[202] = 0.0;
   out_4198711007403848478[203] = 0.0;
   out_4198711007403848478[204] = 0.0;
   out_4198711007403848478[205] = 0.0;
   out_4198711007403848478[206] = 0.0;
   out_4198711007403848478[207] = 0.0;
   out_4198711007403848478[208] = 0.0;
   out_4198711007403848478[209] = 1.0;
   out_4198711007403848478[210] = 0.0;
   out_4198711007403848478[211] = 0.0;
   out_4198711007403848478[212] = 0.0;
   out_4198711007403848478[213] = 0.0;
   out_4198711007403848478[214] = 0.0;
   out_4198711007403848478[215] = 0.0;
   out_4198711007403848478[216] = 0.0;
   out_4198711007403848478[217] = 0.0;
   out_4198711007403848478[218] = 0.0;
   out_4198711007403848478[219] = 0.0;
   out_4198711007403848478[220] = 0.0;
   out_4198711007403848478[221] = 0.0;
   out_4198711007403848478[222] = 0.0;
   out_4198711007403848478[223] = 0.0;
   out_4198711007403848478[224] = 0.0;
   out_4198711007403848478[225] = 0.0;
   out_4198711007403848478[226] = 0.0;
   out_4198711007403848478[227] = 0.0;
   out_4198711007403848478[228] = 1.0;
   out_4198711007403848478[229] = 0.0;
   out_4198711007403848478[230] = 0.0;
   out_4198711007403848478[231] = 0.0;
   out_4198711007403848478[232] = 0.0;
   out_4198711007403848478[233] = 0.0;
   out_4198711007403848478[234] = 0.0;
   out_4198711007403848478[235] = 0.0;
   out_4198711007403848478[236] = 0.0;
   out_4198711007403848478[237] = 0.0;
   out_4198711007403848478[238] = 0.0;
   out_4198711007403848478[239] = 0.0;
   out_4198711007403848478[240] = 0.0;
   out_4198711007403848478[241] = 0.0;
   out_4198711007403848478[242] = 0.0;
   out_4198711007403848478[243] = 0.0;
   out_4198711007403848478[244] = 0.0;
   out_4198711007403848478[245] = 0.0;
   out_4198711007403848478[246] = 0.0;
   out_4198711007403848478[247] = 1.0;
   out_4198711007403848478[248] = 0.0;
   out_4198711007403848478[249] = 0.0;
   out_4198711007403848478[250] = 0.0;
   out_4198711007403848478[251] = 0.0;
   out_4198711007403848478[252] = 0.0;
   out_4198711007403848478[253] = 0.0;
   out_4198711007403848478[254] = 0.0;
   out_4198711007403848478[255] = 0.0;
   out_4198711007403848478[256] = 0.0;
   out_4198711007403848478[257] = 0.0;
   out_4198711007403848478[258] = 0.0;
   out_4198711007403848478[259] = 0.0;
   out_4198711007403848478[260] = 0.0;
   out_4198711007403848478[261] = 0.0;
   out_4198711007403848478[262] = 0.0;
   out_4198711007403848478[263] = 0.0;
   out_4198711007403848478[264] = 0.0;
   out_4198711007403848478[265] = 0.0;
   out_4198711007403848478[266] = 1.0;
   out_4198711007403848478[267] = 0.0;
   out_4198711007403848478[268] = 0.0;
   out_4198711007403848478[269] = 0.0;
   out_4198711007403848478[270] = 0.0;
   out_4198711007403848478[271] = 0.0;
   out_4198711007403848478[272] = 0.0;
   out_4198711007403848478[273] = 0.0;
   out_4198711007403848478[274] = 0.0;
   out_4198711007403848478[275] = 0.0;
   out_4198711007403848478[276] = 0.0;
   out_4198711007403848478[277] = 0.0;
   out_4198711007403848478[278] = 0.0;
   out_4198711007403848478[279] = 0.0;
   out_4198711007403848478[280] = 0.0;
   out_4198711007403848478[281] = 0.0;
   out_4198711007403848478[282] = 0.0;
   out_4198711007403848478[283] = 0.0;
   out_4198711007403848478[284] = 0.0;
   out_4198711007403848478[285] = 1.0;
   out_4198711007403848478[286] = 0.0;
   out_4198711007403848478[287] = 0.0;
   out_4198711007403848478[288] = 0.0;
   out_4198711007403848478[289] = 0.0;
   out_4198711007403848478[290] = 0.0;
   out_4198711007403848478[291] = 0.0;
   out_4198711007403848478[292] = 0.0;
   out_4198711007403848478[293] = 0.0;
   out_4198711007403848478[294] = 0.0;
   out_4198711007403848478[295] = 0.0;
   out_4198711007403848478[296] = 0.0;
   out_4198711007403848478[297] = 0.0;
   out_4198711007403848478[298] = 0.0;
   out_4198711007403848478[299] = 0.0;
   out_4198711007403848478[300] = 0.0;
   out_4198711007403848478[301] = 0.0;
   out_4198711007403848478[302] = 0.0;
   out_4198711007403848478[303] = 0.0;
   out_4198711007403848478[304] = 1.0;
   out_4198711007403848478[305] = 0.0;
   out_4198711007403848478[306] = 0.0;
   out_4198711007403848478[307] = 0.0;
   out_4198711007403848478[308] = 0.0;
   out_4198711007403848478[309] = 0.0;
   out_4198711007403848478[310] = 0.0;
   out_4198711007403848478[311] = 0.0;
   out_4198711007403848478[312] = 0.0;
   out_4198711007403848478[313] = 0.0;
   out_4198711007403848478[314] = 0.0;
   out_4198711007403848478[315] = 0.0;
   out_4198711007403848478[316] = 0.0;
   out_4198711007403848478[317] = 0.0;
   out_4198711007403848478[318] = 0.0;
   out_4198711007403848478[319] = 0.0;
   out_4198711007403848478[320] = 0.0;
   out_4198711007403848478[321] = 0.0;
   out_4198711007403848478[322] = 0.0;
   out_4198711007403848478[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_5112212409069008544) {
   out_5112212409069008544[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_5112212409069008544[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_5112212409069008544[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_5112212409069008544[3] = dt*state[12] + state[3];
   out_5112212409069008544[4] = dt*state[13] + state[4];
   out_5112212409069008544[5] = dt*state[14] + state[5];
   out_5112212409069008544[6] = state[6];
   out_5112212409069008544[7] = state[7];
   out_5112212409069008544[8] = state[8];
   out_5112212409069008544[9] = state[9];
   out_5112212409069008544[10] = state[10];
   out_5112212409069008544[11] = state[11];
   out_5112212409069008544[12] = state[12];
   out_5112212409069008544[13] = state[13];
   out_5112212409069008544[14] = state[14];
   out_5112212409069008544[15] = state[15];
   out_5112212409069008544[16] = state[16];
   out_5112212409069008544[17] = state[17];
}
void F_fun(double *state, double dt, double *out_8268211090171322777) {
   out_8268211090171322777[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8268211090171322777[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8268211090171322777[2] = 0;
   out_8268211090171322777[3] = 0;
   out_8268211090171322777[4] = 0;
   out_8268211090171322777[5] = 0;
   out_8268211090171322777[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8268211090171322777[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8268211090171322777[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_8268211090171322777[9] = 0;
   out_8268211090171322777[10] = 0;
   out_8268211090171322777[11] = 0;
   out_8268211090171322777[12] = 0;
   out_8268211090171322777[13] = 0;
   out_8268211090171322777[14] = 0;
   out_8268211090171322777[15] = 0;
   out_8268211090171322777[16] = 0;
   out_8268211090171322777[17] = 0;
   out_8268211090171322777[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8268211090171322777[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8268211090171322777[20] = 0;
   out_8268211090171322777[21] = 0;
   out_8268211090171322777[22] = 0;
   out_8268211090171322777[23] = 0;
   out_8268211090171322777[24] = 0;
   out_8268211090171322777[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8268211090171322777[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_8268211090171322777[27] = 0;
   out_8268211090171322777[28] = 0;
   out_8268211090171322777[29] = 0;
   out_8268211090171322777[30] = 0;
   out_8268211090171322777[31] = 0;
   out_8268211090171322777[32] = 0;
   out_8268211090171322777[33] = 0;
   out_8268211090171322777[34] = 0;
   out_8268211090171322777[35] = 0;
   out_8268211090171322777[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8268211090171322777[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8268211090171322777[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8268211090171322777[39] = 0;
   out_8268211090171322777[40] = 0;
   out_8268211090171322777[41] = 0;
   out_8268211090171322777[42] = 0;
   out_8268211090171322777[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8268211090171322777[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_8268211090171322777[45] = 0;
   out_8268211090171322777[46] = 0;
   out_8268211090171322777[47] = 0;
   out_8268211090171322777[48] = 0;
   out_8268211090171322777[49] = 0;
   out_8268211090171322777[50] = 0;
   out_8268211090171322777[51] = 0;
   out_8268211090171322777[52] = 0;
   out_8268211090171322777[53] = 0;
   out_8268211090171322777[54] = 0;
   out_8268211090171322777[55] = 0;
   out_8268211090171322777[56] = 0;
   out_8268211090171322777[57] = 1;
   out_8268211090171322777[58] = 0;
   out_8268211090171322777[59] = 0;
   out_8268211090171322777[60] = 0;
   out_8268211090171322777[61] = 0;
   out_8268211090171322777[62] = 0;
   out_8268211090171322777[63] = 0;
   out_8268211090171322777[64] = 0;
   out_8268211090171322777[65] = 0;
   out_8268211090171322777[66] = dt;
   out_8268211090171322777[67] = 0;
   out_8268211090171322777[68] = 0;
   out_8268211090171322777[69] = 0;
   out_8268211090171322777[70] = 0;
   out_8268211090171322777[71] = 0;
   out_8268211090171322777[72] = 0;
   out_8268211090171322777[73] = 0;
   out_8268211090171322777[74] = 0;
   out_8268211090171322777[75] = 0;
   out_8268211090171322777[76] = 1;
   out_8268211090171322777[77] = 0;
   out_8268211090171322777[78] = 0;
   out_8268211090171322777[79] = 0;
   out_8268211090171322777[80] = 0;
   out_8268211090171322777[81] = 0;
   out_8268211090171322777[82] = 0;
   out_8268211090171322777[83] = 0;
   out_8268211090171322777[84] = 0;
   out_8268211090171322777[85] = dt;
   out_8268211090171322777[86] = 0;
   out_8268211090171322777[87] = 0;
   out_8268211090171322777[88] = 0;
   out_8268211090171322777[89] = 0;
   out_8268211090171322777[90] = 0;
   out_8268211090171322777[91] = 0;
   out_8268211090171322777[92] = 0;
   out_8268211090171322777[93] = 0;
   out_8268211090171322777[94] = 0;
   out_8268211090171322777[95] = 1;
   out_8268211090171322777[96] = 0;
   out_8268211090171322777[97] = 0;
   out_8268211090171322777[98] = 0;
   out_8268211090171322777[99] = 0;
   out_8268211090171322777[100] = 0;
   out_8268211090171322777[101] = 0;
   out_8268211090171322777[102] = 0;
   out_8268211090171322777[103] = 0;
   out_8268211090171322777[104] = dt;
   out_8268211090171322777[105] = 0;
   out_8268211090171322777[106] = 0;
   out_8268211090171322777[107] = 0;
   out_8268211090171322777[108] = 0;
   out_8268211090171322777[109] = 0;
   out_8268211090171322777[110] = 0;
   out_8268211090171322777[111] = 0;
   out_8268211090171322777[112] = 0;
   out_8268211090171322777[113] = 0;
   out_8268211090171322777[114] = 1;
   out_8268211090171322777[115] = 0;
   out_8268211090171322777[116] = 0;
   out_8268211090171322777[117] = 0;
   out_8268211090171322777[118] = 0;
   out_8268211090171322777[119] = 0;
   out_8268211090171322777[120] = 0;
   out_8268211090171322777[121] = 0;
   out_8268211090171322777[122] = 0;
   out_8268211090171322777[123] = 0;
   out_8268211090171322777[124] = 0;
   out_8268211090171322777[125] = 0;
   out_8268211090171322777[126] = 0;
   out_8268211090171322777[127] = 0;
   out_8268211090171322777[128] = 0;
   out_8268211090171322777[129] = 0;
   out_8268211090171322777[130] = 0;
   out_8268211090171322777[131] = 0;
   out_8268211090171322777[132] = 0;
   out_8268211090171322777[133] = 1;
   out_8268211090171322777[134] = 0;
   out_8268211090171322777[135] = 0;
   out_8268211090171322777[136] = 0;
   out_8268211090171322777[137] = 0;
   out_8268211090171322777[138] = 0;
   out_8268211090171322777[139] = 0;
   out_8268211090171322777[140] = 0;
   out_8268211090171322777[141] = 0;
   out_8268211090171322777[142] = 0;
   out_8268211090171322777[143] = 0;
   out_8268211090171322777[144] = 0;
   out_8268211090171322777[145] = 0;
   out_8268211090171322777[146] = 0;
   out_8268211090171322777[147] = 0;
   out_8268211090171322777[148] = 0;
   out_8268211090171322777[149] = 0;
   out_8268211090171322777[150] = 0;
   out_8268211090171322777[151] = 0;
   out_8268211090171322777[152] = 1;
   out_8268211090171322777[153] = 0;
   out_8268211090171322777[154] = 0;
   out_8268211090171322777[155] = 0;
   out_8268211090171322777[156] = 0;
   out_8268211090171322777[157] = 0;
   out_8268211090171322777[158] = 0;
   out_8268211090171322777[159] = 0;
   out_8268211090171322777[160] = 0;
   out_8268211090171322777[161] = 0;
   out_8268211090171322777[162] = 0;
   out_8268211090171322777[163] = 0;
   out_8268211090171322777[164] = 0;
   out_8268211090171322777[165] = 0;
   out_8268211090171322777[166] = 0;
   out_8268211090171322777[167] = 0;
   out_8268211090171322777[168] = 0;
   out_8268211090171322777[169] = 0;
   out_8268211090171322777[170] = 0;
   out_8268211090171322777[171] = 1;
   out_8268211090171322777[172] = 0;
   out_8268211090171322777[173] = 0;
   out_8268211090171322777[174] = 0;
   out_8268211090171322777[175] = 0;
   out_8268211090171322777[176] = 0;
   out_8268211090171322777[177] = 0;
   out_8268211090171322777[178] = 0;
   out_8268211090171322777[179] = 0;
   out_8268211090171322777[180] = 0;
   out_8268211090171322777[181] = 0;
   out_8268211090171322777[182] = 0;
   out_8268211090171322777[183] = 0;
   out_8268211090171322777[184] = 0;
   out_8268211090171322777[185] = 0;
   out_8268211090171322777[186] = 0;
   out_8268211090171322777[187] = 0;
   out_8268211090171322777[188] = 0;
   out_8268211090171322777[189] = 0;
   out_8268211090171322777[190] = 1;
   out_8268211090171322777[191] = 0;
   out_8268211090171322777[192] = 0;
   out_8268211090171322777[193] = 0;
   out_8268211090171322777[194] = 0;
   out_8268211090171322777[195] = 0;
   out_8268211090171322777[196] = 0;
   out_8268211090171322777[197] = 0;
   out_8268211090171322777[198] = 0;
   out_8268211090171322777[199] = 0;
   out_8268211090171322777[200] = 0;
   out_8268211090171322777[201] = 0;
   out_8268211090171322777[202] = 0;
   out_8268211090171322777[203] = 0;
   out_8268211090171322777[204] = 0;
   out_8268211090171322777[205] = 0;
   out_8268211090171322777[206] = 0;
   out_8268211090171322777[207] = 0;
   out_8268211090171322777[208] = 0;
   out_8268211090171322777[209] = 1;
   out_8268211090171322777[210] = 0;
   out_8268211090171322777[211] = 0;
   out_8268211090171322777[212] = 0;
   out_8268211090171322777[213] = 0;
   out_8268211090171322777[214] = 0;
   out_8268211090171322777[215] = 0;
   out_8268211090171322777[216] = 0;
   out_8268211090171322777[217] = 0;
   out_8268211090171322777[218] = 0;
   out_8268211090171322777[219] = 0;
   out_8268211090171322777[220] = 0;
   out_8268211090171322777[221] = 0;
   out_8268211090171322777[222] = 0;
   out_8268211090171322777[223] = 0;
   out_8268211090171322777[224] = 0;
   out_8268211090171322777[225] = 0;
   out_8268211090171322777[226] = 0;
   out_8268211090171322777[227] = 0;
   out_8268211090171322777[228] = 1;
   out_8268211090171322777[229] = 0;
   out_8268211090171322777[230] = 0;
   out_8268211090171322777[231] = 0;
   out_8268211090171322777[232] = 0;
   out_8268211090171322777[233] = 0;
   out_8268211090171322777[234] = 0;
   out_8268211090171322777[235] = 0;
   out_8268211090171322777[236] = 0;
   out_8268211090171322777[237] = 0;
   out_8268211090171322777[238] = 0;
   out_8268211090171322777[239] = 0;
   out_8268211090171322777[240] = 0;
   out_8268211090171322777[241] = 0;
   out_8268211090171322777[242] = 0;
   out_8268211090171322777[243] = 0;
   out_8268211090171322777[244] = 0;
   out_8268211090171322777[245] = 0;
   out_8268211090171322777[246] = 0;
   out_8268211090171322777[247] = 1;
   out_8268211090171322777[248] = 0;
   out_8268211090171322777[249] = 0;
   out_8268211090171322777[250] = 0;
   out_8268211090171322777[251] = 0;
   out_8268211090171322777[252] = 0;
   out_8268211090171322777[253] = 0;
   out_8268211090171322777[254] = 0;
   out_8268211090171322777[255] = 0;
   out_8268211090171322777[256] = 0;
   out_8268211090171322777[257] = 0;
   out_8268211090171322777[258] = 0;
   out_8268211090171322777[259] = 0;
   out_8268211090171322777[260] = 0;
   out_8268211090171322777[261] = 0;
   out_8268211090171322777[262] = 0;
   out_8268211090171322777[263] = 0;
   out_8268211090171322777[264] = 0;
   out_8268211090171322777[265] = 0;
   out_8268211090171322777[266] = 1;
   out_8268211090171322777[267] = 0;
   out_8268211090171322777[268] = 0;
   out_8268211090171322777[269] = 0;
   out_8268211090171322777[270] = 0;
   out_8268211090171322777[271] = 0;
   out_8268211090171322777[272] = 0;
   out_8268211090171322777[273] = 0;
   out_8268211090171322777[274] = 0;
   out_8268211090171322777[275] = 0;
   out_8268211090171322777[276] = 0;
   out_8268211090171322777[277] = 0;
   out_8268211090171322777[278] = 0;
   out_8268211090171322777[279] = 0;
   out_8268211090171322777[280] = 0;
   out_8268211090171322777[281] = 0;
   out_8268211090171322777[282] = 0;
   out_8268211090171322777[283] = 0;
   out_8268211090171322777[284] = 0;
   out_8268211090171322777[285] = 1;
   out_8268211090171322777[286] = 0;
   out_8268211090171322777[287] = 0;
   out_8268211090171322777[288] = 0;
   out_8268211090171322777[289] = 0;
   out_8268211090171322777[290] = 0;
   out_8268211090171322777[291] = 0;
   out_8268211090171322777[292] = 0;
   out_8268211090171322777[293] = 0;
   out_8268211090171322777[294] = 0;
   out_8268211090171322777[295] = 0;
   out_8268211090171322777[296] = 0;
   out_8268211090171322777[297] = 0;
   out_8268211090171322777[298] = 0;
   out_8268211090171322777[299] = 0;
   out_8268211090171322777[300] = 0;
   out_8268211090171322777[301] = 0;
   out_8268211090171322777[302] = 0;
   out_8268211090171322777[303] = 0;
   out_8268211090171322777[304] = 1;
   out_8268211090171322777[305] = 0;
   out_8268211090171322777[306] = 0;
   out_8268211090171322777[307] = 0;
   out_8268211090171322777[308] = 0;
   out_8268211090171322777[309] = 0;
   out_8268211090171322777[310] = 0;
   out_8268211090171322777[311] = 0;
   out_8268211090171322777[312] = 0;
   out_8268211090171322777[313] = 0;
   out_8268211090171322777[314] = 0;
   out_8268211090171322777[315] = 0;
   out_8268211090171322777[316] = 0;
   out_8268211090171322777[317] = 0;
   out_8268211090171322777[318] = 0;
   out_8268211090171322777[319] = 0;
   out_8268211090171322777[320] = 0;
   out_8268211090171322777[321] = 0;
   out_8268211090171322777[322] = 0;
   out_8268211090171322777[323] = 1;
}
void h_4(double *state, double *unused, double *out_2358914934415246195) {
   out_2358914934415246195[0] = state[6] + state[9];
   out_2358914934415246195[1] = state[7] + state[10];
   out_2358914934415246195[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_7472933086194797731) {
   out_7472933086194797731[0] = 0;
   out_7472933086194797731[1] = 0;
   out_7472933086194797731[2] = 0;
   out_7472933086194797731[3] = 0;
   out_7472933086194797731[4] = 0;
   out_7472933086194797731[5] = 0;
   out_7472933086194797731[6] = 1;
   out_7472933086194797731[7] = 0;
   out_7472933086194797731[8] = 0;
   out_7472933086194797731[9] = 1;
   out_7472933086194797731[10] = 0;
   out_7472933086194797731[11] = 0;
   out_7472933086194797731[12] = 0;
   out_7472933086194797731[13] = 0;
   out_7472933086194797731[14] = 0;
   out_7472933086194797731[15] = 0;
   out_7472933086194797731[16] = 0;
   out_7472933086194797731[17] = 0;
   out_7472933086194797731[18] = 0;
   out_7472933086194797731[19] = 0;
   out_7472933086194797731[20] = 0;
   out_7472933086194797731[21] = 0;
   out_7472933086194797731[22] = 0;
   out_7472933086194797731[23] = 0;
   out_7472933086194797731[24] = 0;
   out_7472933086194797731[25] = 1;
   out_7472933086194797731[26] = 0;
   out_7472933086194797731[27] = 0;
   out_7472933086194797731[28] = 1;
   out_7472933086194797731[29] = 0;
   out_7472933086194797731[30] = 0;
   out_7472933086194797731[31] = 0;
   out_7472933086194797731[32] = 0;
   out_7472933086194797731[33] = 0;
   out_7472933086194797731[34] = 0;
   out_7472933086194797731[35] = 0;
   out_7472933086194797731[36] = 0;
   out_7472933086194797731[37] = 0;
   out_7472933086194797731[38] = 0;
   out_7472933086194797731[39] = 0;
   out_7472933086194797731[40] = 0;
   out_7472933086194797731[41] = 0;
   out_7472933086194797731[42] = 0;
   out_7472933086194797731[43] = 0;
   out_7472933086194797731[44] = 1;
   out_7472933086194797731[45] = 0;
   out_7472933086194797731[46] = 0;
   out_7472933086194797731[47] = 1;
   out_7472933086194797731[48] = 0;
   out_7472933086194797731[49] = 0;
   out_7472933086194797731[50] = 0;
   out_7472933086194797731[51] = 0;
   out_7472933086194797731[52] = 0;
   out_7472933086194797731[53] = 0;
}
void h_10(double *state, double *unused, double *out_2019013223460407147) {
   out_2019013223460407147[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2019013223460407147[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2019013223460407147[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_8707321851081198616) {
   out_8707321851081198616[0] = 0;
   out_8707321851081198616[1] = 9.8100000000000005*cos(state[1]);
   out_8707321851081198616[2] = 0;
   out_8707321851081198616[3] = 0;
   out_8707321851081198616[4] = -state[8];
   out_8707321851081198616[5] = state[7];
   out_8707321851081198616[6] = 0;
   out_8707321851081198616[7] = state[5];
   out_8707321851081198616[8] = -state[4];
   out_8707321851081198616[9] = 0;
   out_8707321851081198616[10] = 0;
   out_8707321851081198616[11] = 0;
   out_8707321851081198616[12] = 1;
   out_8707321851081198616[13] = 0;
   out_8707321851081198616[14] = 0;
   out_8707321851081198616[15] = 1;
   out_8707321851081198616[16] = 0;
   out_8707321851081198616[17] = 0;
   out_8707321851081198616[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_8707321851081198616[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_8707321851081198616[20] = 0;
   out_8707321851081198616[21] = state[8];
   out_8707321851081198616[22] = 0;
   out_8707321851081198616[23] = -state[6];
   out_8707321851081198616[24] = -state[5];
   out_8707321851081198616[25] = 0;
   out_8707321851081198616[26] = state[3];
   out_8707321851081198616[27] = 0;
   out_8707321851081198616[28] = 0;
   out_8707321851081198616[29] = 0;
   out_8707321851081198616[30] = 0;
   out_8707321851081198616[31] = 1;
   out_8707321851081198616[32] = 0;
   out_8707321851081198616[33] = 0;
   out_8707321851081198616[34] = 1;
   out_8707321851081198616[35] = 0;
   out_8707321851081198616[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_8707321851081198616[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_8707321851081198616[38] = 0;
   out_8707321851081198616[39] = -state[7];
   out_8707321851081198616[40] = state[6];
   out_8707321851081198616[41] = 0;
   out_8707321851081198616[42] = state[4];
   out_8707321851081198616[43] = -state[3];
   out_8707321851081198616[44] = 0;
   out_8707321851081198616[45] = 0;
   out_8707321851081198616[46] = 0;
   out_8707321851081198616[47] = 0;
   out_8707321851081198616[48] = 0;
   out_8707321851081198616[49] = 0;
   out_8707321851081198616[50] = 1;
   out_8707321851081198616[51] = 0;
   out_8707321851081198616[52] = 0;
   out_8707321851081198616[53] = 1;
}
void h_13(double *state, double *unused, double *out_830035003939239818) {
   out_830035003939239818[0] = state[3];
   out_830035003939239818[1] = state[4];
   out_830035003939239818[2] = state[5];
}
void H_13(double *state, double *unused, double *out_4260659260862464930) {
   out_4260659260862464930[0] = 0;
   out_4260659260862464930[1] = 0;
   out_4260659260862464930[2] = 0;
   out_4260659260862464930[3] = 1;
   out_4260659260862464930[4] = 0;
   out_4260659260862464930[5] = 0;
   out_4260659260862464930[6] = 0;
   out_4260659260862464930[7] = 0;
   out_4260659260862464930[8] = 0;
   out_4260659260862464930[9] = 0;
   out_4260659260862464930[10] = 0;
   out_4260659260862464930[11] = 0;
   out_4260659260862464930[12] = 0;
   out_4260659260862464930[13] = 0;
   out_4260659260862464930[14] = 0;
   out_4260659260862464930[15] = 0;
   out_4260659260862464930[16] = 0;
   out_4260659260862464930[17] = 0;
   out_4260659260862464930[18] = 0;
   out_4260659260862464930[19] = 0;
   out_4260659260862464930[20] = 0;
   out_4260659260862464930[21] = 0;
   out_4260659260862464930[22] = 1;
   out_4260659260862464930[23] = 0;
   out_4260659260862464930[24] = 0;
   out_4260659260862464930[25] = 0;
   out_4260659260862464930[26] = 0;
   out_4260659260862464930[27] = 0;
   out_4260659260862464930[28] = 0;
   out_4260659260862464930[29] = 0;
   out_4260659260862464930[30] = 0;
   out_4260659260862464930[31] = 0;
   out_4260659260862464930[32] = 0;
   out_4260659260862464930[33] = 0;
   out_4260659260862464930[34] = 0;
   out_4260659260862464930[35] = 0;
   out_4260659260862464930[36] = 0;
   out_4260659260862464930[37] = 0;
   out_4260659260862464930[38] = 0;
   out_4260659260862464930[39] = 0;
   out_4260659260862464930[40] = 0;
   out_4260659260862464930[41] = 1;
   out_4260659260862464930[42] = 0;
   out_4260659260862464930[43] = 0;
   out_4260659260862464930[44] = 0;
   out_4260659260862464930[45] = 0;
   out_4260659260862464930[46] = 0;
   out_4260659260862464930[47] = 0;
   out_4260659260862464930[48] = 0;
   out_4260659260862464930[49] = 0;
   out_4260659260862464930[50] = 0;
   out_4260659260862464930[51] = 0;
   out_4260659260862464930[52] = 0;
   out_4260659260862464930[53] = 0;
}
void h_14(double *state, double *unused, double *out_6071747091570746666) {
   out_6071747091570746666[0] = state[6];
   out_6071747091570746666[1] = state[7];
   out_6071747091570746666[2] = state[8];
}
void H_14(double *state, double *unused, double *out_3509692229855313202) {
   out_3509692229855313202[0] = 0;
   out_3509692229855313202[1] = 0;
   out_3509692229855313202[2] = 0;
   out_3509692229855313202[3] = 0;
   out_3509692229855313202[4] = 0;
   out_3509692229855313202[5] = 0;
   out_3509692229855313202[6] = 1;
   out_3509692229855313202[7] = 0;
   out_3509692229855313202[8] = 0;
   out_3509692229855313202[9] = 0;
   out_3509692229855313202[10] = 0;
   out_3509692229855313202[11] = 0;
   out_3509692229855313202[12] = 0;
   out_3509692229855313202[13] = 0;
   out_3509692229855313202[14] = 0;
   out_3509692229855313202[15] = 0;
   out_3509692229855313202[16] = 0;
   out_3509692229855313202[17] = 0;
   out_3509692229855313202[18] = 0;
   out_3509692229855313202[19] = 0;
   out_3509692229855313202[20] = 0;
   out_3509692229855313202[21] = 0;
   out_3509692229855313202[22] = 0;
   out_3509692229855313202[23] = 0;
   out_3509692229855313202[24] = 0;
   out_3509692229855313202[25] = 1;
   out_3509692229855313202[26] = 0;
   out_3509692229855313202[27] = 0;
   out_3509692229855313202[28] = 0;
   out_3509692229855313202[29] = 0;
   out_3509692229855313202[30] = 0;
   out_3509692229855313202[31] = 0;
   out_3509692229855313202[32] = 0;
   out_3509692229855313202[33] = 0;
   out_3509692229855313202[34] = 0;
   out_3509692229855313202[35] = 0;
   out_3509692229855313202[36] = 0;
   out_3509692229855313202[37] = 0;
   out_3509692229855313202[38] = 0;
   out_3509692229855313202[39] = 0;
   out_3509692229855313202[40] = 0;
   out_3509692229855313202[41] = 0;
   out_3509692229855313202[42] = 0;
   out_3509692229855313202[43] = 0;
   out_3509692229855313202[44] = 1;
   out_3509692229855313202[45] = 0;
   out_3509692229855313202[46] = 0;
   out_3509692229855313202[47] = 0;
   out_3509692229855313202[48] = 0;
   out_3509692229855313202[49] = 0;
   out_3509692229855313202[50] = 0;
   out_3509692229855313202[51] = 0;
   out_3509692229855313202[52] = 0;
   out_3509692229855313202[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8980418967492834945) {
  err_fun(nom_x, delta_x, out_8980418967492834945);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5853420954923124138) {
  inv_err_fun(nom_x, true_x, out_5853420954923124138);
}
void pose_H_mod_fun(double *state, double *out_4198711007403848478) {
  H_mod_fun(state, out_4198711007403848478);
}
void pose_f_fun(double *state, double dt, double *out_5112212409069008544) {
  f_fun(state,  dt, out_5112212409069008544);
}
void pose_F_fun(double *state, double dt, double *out_8268211090171322777) {
  F_fun(state,  dt, out_8268211090171322777);
}
void pose_h_4(double *state, double *unused, double *out_2358914934415246195) {
  h_4(state, unused, out_2358914934415246195);
}
void pose_H_4(double *state, double *unused, double *out_7472933086194797731) {
  H_4(state, unused, out_7472933086194797731);
}
void pose_h_10(double *state, double *unused, double *out_2019013223460407147) {
  h_10(state, unused, out_2019013223460407147);
}
void pose_H_10(double *state, double *unused, double *out_8707321851081198616) {
  H_10(state, unused, out_8707321851081198616);
}
void pose_h_13(double *state, double *unused, double *out_830035003939239818) {
  h_13(state, unused, out_830035003939239818);
}
void pose_H_13(double *state, double *unused, double *out_4260659260862464930) {
  H_13(state, unused, out_4260659260862464930);
}
void pose_h_14(double *state, double *unused, double *out_6071747091570746666) {
  h_14(state, unused, out_6071747091570746666);
}
void pose_H_14(double *state, double *unused, double *out_3509692229855313202) {
  H_14(state, unused, out_3509692229855313202);
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
