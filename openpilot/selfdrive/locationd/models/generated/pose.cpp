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
void err_fun(double *nom_x, double *delta_x, double *out_8549168006619746742) {
   out_8549168006619746742[0] = delta_x[0] + nom_x[0];
   out_8549168006619746742[1] = delta_x[1] + nom_x[1];
   out_8549168006619746742[2] = delta_x[2] + nom_x[2];
   out_8549168006619746742[3] = delta_x[3] + nom_x[3];
   out_8549168006619746742[4] = delta_x[4] + nom_x[4];
   out_8549168006619746742[5] = delta_x[5] + nom_x[5];
   out_8549168006619746742[6] = delta_x[6] + nom_x[6];
   out_8549168006619746742[7] = delta_x[7] + nom_x[7];
   out_8549168006619746742[8] = delta_x[8] + nom_x[8];
   out_8549168006619746742[9] = delta_x[9] + nom_x[9];
   out_8549168006619746742[10] = delta_x[10] + nom_x[10];
   out_8549168006619746742[11] = delta_x[11] + nom_x[11];
   out_8549168006619746742[12] = delta_x[12] + nom_x[12];
   out_8549168006619746742[13] = delta_x[13] + nom_x[13];
   out_8549168006619746742[14] = delta_x[14] + nom_x[14];
   out_8549168006619746742[15] = delta_x[15] + nom_x[15];
   out_8549168006619746742[16] = delta_x[16] + nom_x[16];
   out_8549168006619746742[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_5584185916794207075) {
   out_5584185916794207075[0] = -nom_x[0] + true_x[0];
   out_5584185916794207075[1] = -nom_x[1] + true_x[1];
   out_5584185916794207075[2] = -nom_x[2] + true_x[2];
   out_5584185916794207075[3] = -nom_x[3] + true_x[3];
   out_5584185916794207075[4] = -nom_x[4] + true_x[4];
   out_5584185916794207075[5] = -nom_x[5] + true_x[5];
   out_5584185916794207075[6] = -nom_x[6] + true_x[6];
   out_5584185916794207075[7] = -nom_x[7] + true_x[7];
   out_5584185916794207075[8] = -nom_x[8] + true_x[8];
   out_5584185916794207075[9] = -nom_x[9] + true_x[9];
   out_5584185916794207075[10] = -nom_x[10] + true_x[10];
   out_5584185916794207075[11] = -nom_x[11] + true_x[11];
   out_5584185916794207075[12] = -nom_x[12] + true_x[12];
   out_5584185916794207075[13] = -nom_x[13] + true_x[13];
   out_5584185916794207075[14] = -nom_x[14] + true_x[14];
   out_5584185916794207075[15] = -nom_x[15] + true_x[15];
   out_5584185916794207075[16] = -nom_x[16] + true_x[16];
   out_5584185916794207075[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8554003597594456449) {
   out_8554003597594456449[0] = 1.0;
   out_8554003597594456449[1] = 0.0;
   out_8554003597594456449[2] = 0.0;
   out_8554003597594456449[3] = 0.0;
   out_8554003597594456449[4] = 0.0;
   out_8554003597594456449[5] = 0.0;
   out_8554003597594456449[6] = 0.0;
   out_8554003597594456449[7] = 0.0;
   out_8554003597594456449[8] = 0.0;
   out_8554003597594456449[9] = 0.0;
   out_8554003597594456449[10] = 0.0;
   out_8554003597594456449[11] = 0.0;
   out_8554003597594456449[12] = 0.0;
   out_8554003597594456449[13] = 0.0;
   out_8554003597594456449[14] = 0.0;
   out_8554003597594456449[15] = 0.0;
   out_8554003597594456449[16] = 0.0;
   out_8554003597594456449[17] = 0.0;
   out_8554003597594456449[18] = 0.0;
   out_8554003597594456449[19] = 1.0;
   out_8554003597594456449[20] = 0.0;
   out_8554003597594456449[21] = 0.0;
   out_8554003597594456449[22] = 0.0;
   out_8554003597594456449[23] = 0.0;
   out_8554003597594456449[24] = 0.0;
   out_8554003597594456449[25] = 0.0;
   out_8554003597594456449[26] = 0.0;
   out_8554003597594456449[27] = 0.0;
   out_8554003597594456449[28] = 0.0;
   out_8554003597594456449[29] = 0.0;
   out_8554003597594456449[30] = 0.0;
   out_8554003597594456449[31] = 0.0;
   out_8554003597594456449[32] = 0.0;
   out_8554003597594456449[33] = 0.0;
   out_8554003597594456449[34] = 0.0;
   out_8554003597594456449[35] = 0.0;
   out_8554003597594456449[36] = 0.0;
   out_8554003597594456449[37] = 0.0;
   out_8554003597594456449[38] = 1.0;
   out_8554003597594456449[39] = 0.0;
   out_8554003597594456449[40] = 0.0;
   out_8554003597594456449[41] = 0.0;
   out_8554003597594456449[42] = 0.0;
   out_8554003597594456449[43] = 0.0;
   out_8554003597594456449[44] = 0.0;
   out_8554003597594456449[45] = 0.0;
   out_8554003597594456449[46] = 0.0;
   out_8554003597594456449[47] = 0.0;
   out_8554003597594456449[48] = 0.0;
   out_8554003597594456449[49] = 0.0;
   out_8554003597594456449[50] = 0.0;
   out_8554003597594456449[51] = 0.0;
   out_8554003597594456449[52] = 0.0;
   out_8554003597594456449[53] = 0.0;
   out_8554003597594456449[54] = 0.0;
   out_8554003597594456449[55] = 0.0;
   out_8554003597594456449[56] = 0.0;
   out_8554003597594456449[57] = 1.0;
   out_8554003597594456449[58] = 0.0;
   out_8554003597594456449[59] = 0.0;
   out_8554003597594456449[60] = 0.0;
   out_8554003597594456449[61] = 0.0;
   out_8554003597594456449[62] = 0.0;
   out_8554003597594456449[63] = 0.0;
   out_8554003597594456449[64] = 0.0;
   out_8554003597594456449[65] = 0.0;
   out_8554003597594456449[66] = 0.0;
   out_8554003597594456449[67] = 0.0;
   out_8554003597594456449[68] = 0.0;
   out_8554003597594456449[69] = 0.0;
   out_8554003597594456449[70] = 0.0;
   out_8554003597594456449[71] = 0.0;
   out_8554003597594456449[72] = 0.0;
   out_8554003597594456449[73] = 0.0;
   out_8554003597594456449[74] = 0.0;
   out_8554003597594456449[75] = 0.0;
   out_8554003597594456449[76] = 1.0;
   out_8554003597594456449[77] = 0.0;
   out_8554003597594456449[78] = 0.0;
   out_8554003597594456449[79] = 0.0;
   out_8554003597594456449[80] = 0.0;
   out_8554003597594456449[81] = 0.0;
   out_8554003597594456449[82] = 0.0;
   out_8554003597594456449[83] = 0.0;
   out_8554003597594456449[84] = 0.0;
   out_8554003597594456449[85] = 0.0;
   out_8554003597594456449[86] = 0.0;
   out_8554003597594456449[87] = 0.0;
   out_8554003597594456449[88] = 0.0;
   out_8554003597594456449[89] = 0.0;
   out_8554003597594456449[90] = 0.0;
   out_8554003597594456449[91] = 0.0;
   out_8554003597594456449[92] = 0.0;
   out_8554003597594456449[93] = 0.0;
   out_8554003597594456449[94] = 0.0;
   out_8554003597594456449[95] = 1.0;
   out_8554003597594456449[96] = 0.0;
   out_8554003597594456449[97] = 0.0;
   out_8554003597594456449[98] = 0.0;
   out_8554003597594456449[99] = 0.0;
   out_8554003597594456449[100] = 0.0;
   out_8554003597594456449[101] = 0.0;
   out_8554003597594456449[102] = 0.0;
   out_8554003597594456449[103] = 0.0;
   out_8554003597594456449[104] = 0.0;
   out_8554003597594456449[105] = 0.0;
   out_8554003597594456449[106] = 0.0;
   out_8554003597594456449[107] = 0.0;
   out_8554003597594456449[108] = 0.0;
   out_8554003597594456449[109] = 0.0;
   out_8554003597594456449[110] = 0.0;
   out_8554003597594456449[111] = 0.0;
   out_8554003597594456449[112] = 0.0;
   out_8554003597594456449[113] = 0.0;
   out_8554003597594456449[114] = 1.0;
   out_8554003597594456449[115] = 0.0;
   out_8554003597594456449[116] = 0.0;
   out_8554003597594456449[117] = 0.0;
   out_8554003597594456449[118] = 0.0;
   out_8554003597594456449[119] = 0.0;
   out_8554003597594456449[120] = 0.0;
   out_8554003597594456449[121] = 0.0;
   out_8554003597594456449[122] = 0.0;
   out_8554003597594456449[123] = 0.0;
   out_8554003597594456449[124] = 0.0;
   out_8554003597594456449[125] = 0.0;
   out_8554003597594456449[126] = 0.0;
   out_8554003597594456449[127] = 0.0;
   out_8554003597594456449[128] = 0.0;
   out_8554003597594456449[129] = 0.0;
   out_8554003597594456449[130] = 0.0;
   out_8554003597594456449[131] = 0.0;
   out_8554003597594456449[132] = 0.0;
   out_8554003597594456449[133] = 1.0;
   out_8554003597594456449[134] = 0.0;
   out_8554003597594456449[135] = 0.0;
   out_8554003597594456449[136] = 0.0;
   out_8554003597594456449[137] = 0.0;
   out_8554003597594456449[138] = 0.0;
   out_8554003597594456449[139] = 0.0;
   out_8554003597594456449[140] = 0.0;
   out_8554003597594456449[141] = 0.0;
   out_8554003597594456449[142] = 0.0;
   out_8554003597594456449[143] = 0.0;
   out_8554003597594456449[144] = 0.0;
   out_8554003597594456449[145] = 0.0;
   out_8554003597594456449[146] = 0.0;
   out_8554003597594456449[147] = 0.0;
   out_8554003597594456449[148] = 0.0;
   out_8554003597594456449[149] = 0.0;
   out_8554003597594456449[150] = 0.0;
   out_8554003597594456449[151] = 0.0;
   out_8554003597594456449[152] = 1.0;
   out_8554003597594456449[153] = 0.0;
   out_8554003597594456449[154] = 0.0;
   out_8554003597594456449[155] = 0.0;
   out_8554003597594456449[156] = 0.0;
   out_8554003597594456449[157] = 0.0;
   out_8554003597594456449[158] = 0.0;
   out_8554003597594456449[159] = 0.0;
   out_8554003597594456449[160] = 0.0;
   out_8554003597594456449[161] = 0.0;
   out_8554003597594456449[162] = 0.0;
   out_8554003597594456449[163] = 0.0;
   out_8554003597594456449[164] = 0.0;
   out_8554003597594456449[165] = 0.0;
   out_8554003597594456449[166] = 0.0;
   out_8554003597594456449[167] = 0.0;
   out_8554003597594456449[168] = 0.0;
   out_8554003597594456449[169] = 0.0;
   out_8554003597594456449[170] = 0.0;
   out_8554003597594456449[171] = 1.0;
   out_8554003597594456449[172] = 0.0;
   out_8554003597594456449[173] = 0.0;
   out_8554003597594456449[174] = 0.0;
   out_8554003597594456449[175] = 0.0;
   out_8554003597594456449[176] = 0.0;
   out_8554003597594456449[177] = 0.0;
   out_8554003597594456449[178] = 0.0;
   out_8554003597594456449[179] = 0.0;
   out_8554003597594456449[180] = 0.0;
   out_8554003597594456449[181] = 0.0;
   out_8554003597594456449[182] = 0.0;
   out_8554003597594456449[183] = 0.0;
   out_8554003597594456449[184] = 0.0;
   out_8554003597594456449[185] = 0.0;
   out_8554003597594456449[186] = 0.0;
   out_8554003597594456449[187] = 0.0;
   out_8554003597594456449[188] = 0.0;
   out_8554003597594456449[189] = 0.0;
   out_8554003597594456449[190] = 1.0;
   out_8554003597594456449[191] = 0.0;
   out_8554003597594456449[192] = 0.0;
   out_8554003597594456449[193] = 0.0;
   out_8554003597594456449[194] = 0.0;
   out_8554003597594456449[195] = 0.0;
   out_8554003597594456449[196] = 0.0;
   out_8554003597594456449[197] = 0.0;
   out_8554003597594456449[198] = 0.0;
   out_8554003597594456449[199] = 0.0;
   out_8554003597594456449[200] = 0.0;
   out_8554003597594456449[201] = 0.0;
   out_8554003597594456449[202] = 0.0;
   out_8554003597594456449[203] = 0.0;
   out_8554003597594456449[204] = 0.0;
   out_8554003597594456449[205] = 0.0;
   out_8554003597594456449[206] = 0.0;
   out_8554003597594456449[207] = 0.0;
   out_8554003597594456449[208] = 0.0;
   out_8554003597594456449[209] = 1.0;
   out_8554003597594456449[210] = 0.0;
   out_8554003597594456449[211] = 0.0;
   out_8554003597594456449[212] = 0.0;
   out_8554003597594456449[213] = 0.0;
   out_8554003597594456449[214] = 0.0;
   out_8554003597594456449[215] = 0.0;
   out_8554003597594456449[216] = 0.0;
   out_8554003597594456449[217] = 0.0;
   out_8554003597594456449[218] = 0.0;
   out_8554003597594456449[219] = 0.0;
   out_8554003597594456449[220] = 0.0;
   out_8554003597594456449[221] = 0.0;
   out_8554003597594456449[222] = 0.0;
   out_8554003597594456449[223] = 0.0;
   out_8554003597594456449[224] = 0.0;
   out_8554003597594456449[225] = 0.0;
   out_8554003597594456449[226] = 0.0;
   out_8554003597594456449[227] = 0.0;
   out_8554003597594456449[228] = 1.0;
   out_8554003597594456449[229] = 0.0;
   out_8554003597594456449[230] = 0.0;
   out_8554003597594456449[231] = 0.0;
   out_8554003597594456449[232] = 0.0;
   out_8554003597594456449[233] = 0.0;
   out_8554003597594456449[234] = 0.0;
   out_8554003597594456449[235] = 0.0;
   out_8554003597594456449[236] = 0.0;
   out_8554003597594456449[237] = 0.0;
   out_8554003597594456449[238] = 0.0;
   out_8554003597594456449[239] = 0.0;
   out_8554003597594456449[240] = 0.0;
   out_8554003597594456449[241] = 0.0;
   out_8554003597594456449[242] = 0.0;
   out_8554003597594456449[243] = 0.0;
   out_8554003597594456449[244] = 0.0;
   out_8554003597594456449[245] = 0.0;
   out_8554003597594456449[246] = 0.0;
   out_8554003597594456449[247] = 1.0;
   out_8554003597594456449[248] = 0.0;
   out_8554003597594456449[249] = 0.0;
   out_8554003597594456449[250] = 0.0;
   out_8554003597594456449[251] = 0.0;
   out_8554003597594456449[252] = 0.0;
   out_8554003597594456449[253] = 0.0;
   out_8554003597594456449[254] = 0.0;
   out_8554003597594456449[255] = 0.0;
   out_8554003597594456449[256] = 0.0;
   out_8554003597594456449[257] = 0.0;
   out_8554003597594456449[258] = 0.0;
   out_8554003597594456449[259] = 0.0;
   out_8554003597594456449[260] = 0.0;
   out_8554003597594456449[261] = 0.0;
   out_8554003597594456449[262] = 0.0;
   out_8554003597594456449[263] = 0.0;
   out_8554003597594456449[264] = 0.0;
   out_8554003597594456449[265] = 0.0;
   out_8554003597594456449[266] = 1.0;
   out_8554003597594456449[267] = 0.0;
   out_8554003597594456449[268] = 0.0;
   out_8554003597594456449[269] = 0.0;
   out_8554003597594456449[270] = 0.0;
   out_8554003597594456449[271] = 0.0;
   out_8554003597594456449[272] = 0.0;
   out_8554003597594456449[273] = 0.0;
   out_8554003597594456449[274] = 0.0;
   out_8554003597594456449[275] = 0.0;
   out_8554003597594456449[276] = 0.0;
   out_8554003597594456449[277] = 0.0;
   out_8554003597594456449[278] = 0.0;
   out_8554003597594456449[279] = 0.0;
   out_8554003597594456449[280] = 0.0;
   out_8554003597594456449[281] = 0.0;
   out_8554003597594456449[282] = 0.0;
   out_8554003597594456449[283] = 0.0;
   out_8554003597594456449[284] = 0.0;
   out_8554003597594456449[285] = 1.0;
   out_8554003597594456449[286] = 0.0;
   out_8554003597594456449[287] = 0.0;
   out_8554003597594456449[288] = 0.0;
   out_8554003597594456449[289] = 0.0;
   out_8554003597594456449[290] = 0.0;
   out_8554003597594456449[291] = 0.0;
   out_8554003597594456449[292] = 0.0;
   out_8554003597594456449[293] = 0.0;
   out_8554003597594456449[294] = 0.0;
   out_8554003597594456449[295] = 0.0;
   out_8554003597594456449[296] = 0.0;
   out_8554003597594456449[297] = 0.0;
   out_8554003597594456449[298] = 0.0;
   out_8554003597594456449[299] = 0.0;
   out_8554003597594456449[300] = 0.0;
   out_8554003597594456449[301] = 0.0;
   out_8554003597594456449[302] = 0.0;
   out_8554003597594456449[303] = 0.0;
   out_8554003597594456449[304] = 1.0;
   out_8554003597594456449[305] = 0.0;
   out_8554003597594456449[306] = 0.0;
   out_8554003597594456449[307] = 0.0;
   out_8554003597594456449[308] = 0.0;
   out_8554003597594456449[309] = 0.0;
   out_8554003597594456449[310] = 0.0;
   out_8554003597594456449[311] = 0.0;
   out_8554003597594456449[312] = 0.0;
   out_8554003597594456449[313] = 0.0;
   out_8554003597594456449[314] = 0.0;
   out_8554003597594456449[315] = 0.0;
   out_8554003597594456449[316] = 0.0;
   out_8554003597594456449[317] = 0.0;
   out_8554003597594456449[318] = 0.0;
   out_8554003597594456449[319] = 0.0;
   out_8554003597594456449[320] = 0.0;
   out_8554003597594456449[321] = 0.0;
   out_8554003597594456449[322] = 0.0;
   out_8554003597594456449[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_783971397319598562) {
   out_783971397319598562[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_783971397319598562[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_783971397319598562[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_783971397319598562[3] = dt*state[12] + state[3];
   out_783971397319598562[4] = dt*state[13] + state[4];
   out_783971397319598562[5] = dt*state[14] + state[5];
   out_783971397319598562[6] = state[6];
   out_783971397319598562[7] = state[7];
   out_783971397319598562[8] = state[8];
   out_783971397319598562[9] = state[9];
   out_783971397319598562[10] = state[10];
   out_783971397319598562[11] = state[11];
   out_783971397319598562[12] = state[12];
   out_783971397319598562[13] = state[13];
   out_783971397319598562[14] = state[14];
   out_783971397319598562[15] = state[15];
   out_783971397319598562[16] = state[16];
   out_783971397319598562[17] = state[17];
}
void F_fun(double *state, double dt, double *out_804064847722436257) {
   out_804064847722436257[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_804064847722436257[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_804064847722436257[2] = 0;
   out_804064847722436257[3] = 0;
   out_804064847722436257[4] = 0;
   out_804064847722436257[5] = 0;
   out_804064847722436257[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_804064847722436257[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_804064847722436257[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_804064847722436257[9] = 0;
   out_804064847722436257[10] = 0;
   out_804064847722436257[11] = 0;
   out_804064847722436257[12] = 0;
   out_804064847722436257[13] = 0;
   out_804064847722436257[14] = 0;
   out_804064847722436257[15] = 0;
   out_804064847722436257[16] = 0;
   out_804064847722436257[17] = 0;
   out_804064847722436257[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_804064847722436257[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_804064847722436257[20] = 0;
   out_804064847722436257[21] = 0;
   out_804064847722436257[22] = 0;
   out_804064847722436257[23] = 0;
   out_804064847722436257[24] = 0;
   out_804064847722436257[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_804064847722436257[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_804064847722436257[27] = 0;
   out_804064847722436257[28] = 0;
   out_804064847722436257[29] = 0;
   out_804064847722436257[30] = 0;
   out_804064847722436257[31] = 0;
   out_804064847722436257[32] = 0;
   out_804064847722436257[33] = 0;
   out_804064847722436257[34] = 0;
   out_804064847722436257[35] = 0;
   out_804064847722436257[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_804064847722436257[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_804064847722436257[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_804064847722436257[39] = 0;
   out_804064847722436257[40] = 0;
   out_804064847722436257[41] = 0;
   out_804064847722436257[42] = 0;
   out_804064847722436257[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_804064847722436257[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_804064847722436257[45] = 0;
   out_804064847722436257[46] = 0;
   out_804064847722436257[47] = 0;
   out_804064847722436257[48] = 0;
   out_804064847722436257[49] = 0;
   out_804064847722436257[50] = 0;
   out_804064847722436257[51] = 0;
   out_804064847722436257[52] = 0;
   out_804064847722436257[53] = 0;
   out_804064847722436257[54] = 0;
   out_804064847722436257[55] = 0;
   out_804064847722436257[56] = 0;
   out_804064847722436257[57] = 1;
   out_804064847722436257[58] = 0;
   out_804064847722436257[59] = 0;
   out_804064847722436257[60] = 0;
   out_804064847722436257[61] = 0;
   out_804064847722436257[62] = 0;
   out_804064847722436257[63] = 0;
   out_804064847722436257[64] = 0;
   out_804064847722436257[65] = 0;
   out_804064847722436257[66] = dt;
   out_804064847722436257[67] = 0;
   out_804064847722436257[68] = 0;
   out_804064847722436257[69] = 0;
   out_804064847722436257[70] = 0;
   out_804064847722436257[71] = 0;
   out_804064847722436257[72] = 0;
   out_804064847722436257[73] = 0;
   out_804064847722436257[74] = 0;
   out_804064847722436257[75] = 0;
   out_804064847722436257[76] = 1;
   out_804064847722436257[77] = 0;
   out_804064847722436257[78] = 0;
   out_804064847722436257[79] = 0;
   out_804064847722436257[80] = 0;
   out_804064847722436257[81] = 0;
   out_804064847722436257[82] = 0;
   out_804064847722436257[83] = 0;
   out_804064847722436257[84] = 0;
   out_804064847722436257[85] = dt;
   out_804064847722436257[86] = 0;
   out_804064847722436257[87] = 0;
   out_804064847722436257[88] = 0;
   out_804064847722436257[89] = 0;
   out_804064847722436257[90] = 0;
   out_804064847722436257[91] = 0;
   out_804064847722436257[92] = 0;
   out_804064847722436257[93] = 0;
   out_804064847722436257[94] = 0;
   out_804064847722436257[95] = 1;
   out_804064847722436257[96] = 0;
   out_804064847722436257[97] = 0;
   out_804064847722436257[98] = 0;
   out_804064847722436257[99] = 0;
   out_804064847722436257[100] = 0;
   out_804064847722436257[101] = 0;
   out_804064847722436257[102] = 0;
   out_804064847722436257[103] = 0;
   out_804064847722436257[104] = dt;
   out_804064847722436257[105] = 0;
   out_804064847722436257[106] = 0;
   out_804064847722436257[107] = 0;
   out_804064847722436257[108] = 0;
   out_804064847722436257[109] = 0;
   out_804064847722436257[110] = 0;
   out_804064847722436257[111] = 0;
   out_804064847722436257[112] = 0;
   out_804064847722436257[113] = 0;
   out_804064847722436257[114] = 1;
   out_804064847722436257[115] = 0;
   out_804064847722436257[116] = 0;
   out_804064847722436257[117] = 0;
   out_804064847722436257[118] = 0;
   out_804064847722436257[119] = 0;
   out_804064847722436257[120] = 0;
   out_804064847722436257[121] = 0;
   out_804064847722436257[122] = 0;
   out_804064847722436257[123] = 0;
   out_804064847722436257[124] = 0;
   out_804064847722436257[125] = 0;
   out_804064847722436257[126] = 0;
   out_804064847722436257[127] = 0;
   out_804064847722436257[128] = 0;
   out_804064847722436257[129] = 0;
   out_804064847722436257[130] = 0;
   out_804064847722436257[131] = 0;
   out_804064847722436257[132] = 0;
   out_804064847722436257[133] = 1;
   out_804064847722436257[134] = 0;
   out_804064847722436257[135] = 0;
   out_804064847722436257[136] = 0;
   out_804064847722436257[137] = 0;
   out_804064847722436257[138] = 0;
   out_804064847722436257[139] = 0;
   out_804064847722436257[140] = 0;
   out_804064847722436257[141] = 0;
   out_804064847722436257[142] = 0;
   out_804064847722436257[143] = 0;
   out_804064847722436257[144] = 0;
   out_804064847722436257[145] = 0;
   out_804064847722436257[146] = 0;
   out_804064847722436257[147] = 0;
   out_804064847722436257[148] = 0;
   out_804064847722436257[149] = 0;
   out_804064847722436257[150] = 0;
   out_804064847722436257[151] = 0;
   out_804064847722436257[152] = 1;
   out_804064847722436257[153] = 0;
   out_804064847722436257[154] = 0;
   out_804064847722436257[155] = 0;
   out_804064847722436257[156] = 0;
   out_804064847722436257[157] = 0;
   out_804064847722436257[158] = 0;
   out_804064847722436257[159] = 0;
   out_804064847722436257[160] = 0;
   out_804064847722436257[161] = 0;
   out_804064847722436257[162] = 0;
   out_804064847722436257[163] = 0;
   out_804064847722436257[164] = 0;
   out_804064847722436257[165] = 0;
   out_804064847722436257[166] = 0;
   out_804064847722436257[167] = 0;
   out_804064847722436257[168] = 0;
   out_804064847722436257[169] = 0;
   out_804064847722436257[170] = 0;
   out_804064847722436257[171] = 1;
   out_804064847722436257[172] = 0;
   out_804064847722436257[173] = 0;
   out_804064847722436257[174] = 0;
   out_804064847722436257[175] = 0;
   out_804064847722436257[176] = 0;
   out_804064847722436257[177] = 0;
   out_804064847722436257[178] = 0;
   out_804064847722436257[179] = 0;
   out_804064847722436257[180] = 0;
   out_804064847722436257[181] = 0;
   out_804064847722436257[182] = 0;
   out_804064847722436257[183] = 0;
   out_804064847722436257[184] = 0;
   out_804064847722436257[185] = 0;
   out_804064847722436257[186] = 0;
   out_804064847722436257[187] = 0;
   out_804064847722436257[188] = 0;
   out_804064847722436257[189] = 0;
   out_804064847722436257[190] = 1;
   out_804064847722436257[191] = 0;
   out_804064847722436257[192] = 0;
   out_804064847722436257[193] = 0;
   out_804064847722436257[194] = 0;
   out_804064847722436257[195] = 0;
   out_804064847722436257[196] = 0;
   out_804064847722436257[197] = 0;
   out_804064847722436257[198] = 0;
   out_804064847722436257[199] = 0;
   out_804064847722436257[200] = 0;
   out_804064847722436257[201] = 0;
   out_804064847722436257[202] = 0;
   out_804064847722436257[203] = 0;
   out_804064847722436257[204] = 0;
   out_804064847722436257[205] = 0;
   out_804064847722436257[206] = 0;
   out_804064847722436257[207] = 0;
   out_804064847722436257[208] = 0;
   out_804064847722436257[209] = 1;
   out_804064847722436257[210] = 0;
   out_804064847722436257[211] = 0;
   out_804064847722436257[212] = 0;
   out_804064847722436257[213] = 0;
   out_804064847722436257[214] = 0;
   out_804064847722436257[215] = 0;
   out_804064847722436257[216] = 0;
   out_804064847722436257[217] = 0;
   out_804064847722436257[218] = 0;
   out_804064847722436257[219] = 0;
   out_804064847722436257[220] = 0;
   out_804064847722436257[221] = 0;
   out_804064847722436257[222] = 0;
   out_804064847722436257[223] = 0;
   out_804064847722436257[224] = 0;
   out_804064847722436257[225] = 0;
   out_804064847722436257[226] = 0;
   out_804064847722436257[227] = 0;
   out_804064847722436257[228] = 1;
   out_804064847722436257[229] = 0;
   out_804064847722436257[230] = 0;
   out_804064847722436257[231] = 0;
   out_804064847722436257[232] = 0;
   out_804064847722436257[233] = 0;
   out_804064847722436257[234] = 0;
   out_804064847722436257[235] = 0;
   out_804064847722436257[236] = 0;
   out_804064847722436257[237] = 0;
   out_804064847722436257[238] = 0;
   out_804064847722436257[239] = 0;
   out_804064847722436257[240] = 0;
   out_804064847722436257[241] = 0;
   out_804064847722436257[242] = 0;
   out_804064847722436257[243] = 0;
   out_804064847722436257[244] = 0;
   out_804064847722436257[245] = 0;
   out_804064847722436257[246] = 0;
   out_804064847722436257[247] = 1;
   out_804064847722436257[248] = 0;
   out_804064847722436257[249] = 0;
   out_804064847722436257[250] = 0;
   out_804064847722436257[251] = 0;
   out_804064847722436257[252] = 0;
   out_804064847722436257[253] = 0;
   out_804064847722436257[254] = 0;
   out_804064847722436257[255] = 0;
   out_804064847722436257[256] = 0;
   out_804064847722436257[257] = 0;
   out_804064847722436257[258] = 0;
   out_804064847722436257[259] = 0;
   out_804064847722436257[260] = 0;
   out_804064847722436257[261] = 0;
   out_804064847722436257[262] = 0;
   out_804064847722436257[263] = 0;
   out_804064847722436257[264] = 0;
   out_804064847722436257[265] = 0;
   out_804064847722436257[266] = 1;
   out_804064847722436257[267] = 0;
   out_804064847722436257[268] = 0;
   out_804064847722436257[269] = 0;
   out_804064847722436257[270] = 0;
   out_804064847722436257[271] = 0;
   out_804064847722436257[272] = 0;
   out_804064847722436257[273] = 0;
   out_804064847722436257[274] = 0;
   out_804064847722436257[275] = 0;
   out_804064847722436257[276] = 0;
   out_804064847722436257[277] = 0;
   out_804064847722436257[278] = 0;
   out_804064847722436257[279] = 0;
   out_804064847722436257[280] = 0;
   out_804064847722436257[281] = 0;
   out_804064847722436257[282] = 0;
   out_804064847722436257[283] = 0;
   out_804064847722436257[284] = 0;
   out_804064847722436257[285] = 1;
   out_804064847722436257[286] = 0;
   out_804064847722436257[287] = 0;
   out_804064847722436257[288] = 0;
   out_804064847722436257[289] = 0;
   out_804064847722436257[290] = 0;
   out_804064847722436257[291] = 0;
   out_804064847722436257[292] = 0;
   out_804064847722436257[293] = 0;
   out_804064847722436257[294] = 0;
   out_804064847722436257[295] = 0;
   out_804064847722436257[296] = 0;
   out_804064847722436257[297] = 0;
   out_804064847722436257[298] = 0;
   out_804064847722436257[299] = 0;
   out_804064847722436257[300] = 0;
   out_804064847722436257[301] = 0;
   out_804064847722436257[302] = 0;
   out_804064847722436257[303] = 0;
   out_804064847722436257[304] = 1;
   out_804064847722436257[305] = 0;
   out_804064847722436257[306] = 0;
   out_804064847722436257[307] = 0;
   out_804064847722436257[308] = 0;
   out_804064847722436257[309] = 0;
   out_804064847722436257[310] = 0;
   out_804064847722436257[311] = 0;
   out_804064847722436257[312] = 0;
   out_804064847722436257[313] = 0;
   out_804064847722436257[314] = 0;
   out_804064847722436257[315] = 0;
   out_804064847722436257[316] = 0;
   out_804064847722436257[317] = 0;
   out_804064847722436257[318] = 0;
   out_804064847722436257[319] = 0;
   out_804064847722436257[320] = 0;
   out_804064847722436257[321] = 0;
   out_804064847722436257[322] = 0;
   out_804064847722436257[323] = 1;
}
void h_4(double *state, double *unused, double *out_4471711811158414269) {
   out_4471711811158414269[0] = state[6] + state[9];
   out_4471711811158414269[1] = state[7] + state[10];
   out_4471711811158414269[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_184192634488532689) {
   out_184192634488532689[0] = 0;
   out_184192634488532689[1] = 0;
   out_184192634488532689[2] = 0;
   out_184192634488532689[3] = 0;
   out_184192634488532689[4] = 0;
   out_184192634488532689[5] = 0;
   out_184192634488532689[6] = 1;
   out_184192634488532689[7] = 0;
   out_184192634488532689[8] = 0;
   out_184192634488532689[9] = 1;
   out_184192634488532689[10] = 0;
   out_184192634488532689[11] = 0;
   out_184192634488532689[12] = 0;
   out_184192634488532689[13] = 0;
   out_184192634488532689[14] = 0;
   out_184192634488532689[15] = 0;
   out_184192634488532689[16] = 0;
   out_184192634488532689[17] = 0;
   out_184192634488532689[18] = 0;
   out_184192634488532689[19] = 0;
   out_184192634488532689[20] = 0;
   out_184192634488532689[21] = 0;
   out_184192634488532689[22] = 0;
   out_184192634488532689[23] = 0;
   out_184192634488532689[24] = 0;
   out_184192634488532689[25] = 1;
   out_184192634488532689[26] = 0;
   out_184192634488532689[27] = 0;
   out_184192634488532689[28] = 1;
   out_184192634488532689[29] = 0;
   out_184192634488532689[30] = 0;
   out_184192634488532689[31] = 0;
   out_184192634488532689[32] = 0;
   out_184192634488532689[33] = 0;
   out_184192634488532689[34] = 0;
   out_184192634488532689[35] = 0;
   out_184192634488532689[36] = 0;
   out_184192634488532689[37] = 0;
   out_184192634488532689[38] = 0;
   out_184192634488532689[39] = 0;
   out_184192634488532689[40] = 0;
   out_184192634488532689[41] = 0;
   out_184192634488532689[42] = 0;
   out_184192634488532689[43] = 0;
   out_184192634488532689[44] = 1;
   out_184192634488532689[45] = 0;
   out_184192634488532689[46] = 0;
   out_184192634488532689[47] = 1;
   out_184192634488532689[48] = 0;
   out_184192634488532689[49] = 0;
   out_184192634488532689[50] = 0;
   out_184192634488532689[51] = 0;
   out_184192634488532689[52] = 0;
   out_184192634488532689[53] = 0;
}
void h_10(double *state, double *unused, double *out_2904425780055764226) {
   out_2904425780055764226[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_2904425780055764226[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_2904425780055764226[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_4999916326087304913) {
   out_4999916326087304913[0] = 0;
   out_4999916326087304913[1] = 9.8100000000000005*cos(state[1]);
   out_4999916326087304913[2] = 0;
   out_4999916326087304913[3] = 0;
   out_4999916326087304913[4] = -state[8];
   out_4999916326087304913[5] = state[7];
   out_4999916326087304913[6] = 0;
   out_4999916326087304913[7] = state[5];
   out_4999916326087304913[8] = -state[4];
   out_4999916326087304913[9] = 0;
   out_4999916326087304913[10] = 0;
   out_4999916326087304913[11] = 0;
   out_4999916326087304913[12] = 1;
   out_4999916326087304913[13] = 0;
   out_4999916326087304913[14] = 0;
   out_4999916326087304913[15] = 1;
   out_4999916326087304913[16] = 0;
   out_4999916326087304913[17] = 0;
   out_4999916326087304913[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_4999916326087304913[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_4999916326087304913[20] = 0;
   out_4999916326087304913[21] = state[8];
   out_4999916326087304913[22] = 0;
   out_4999916326087304913[23] = -state[6];
   out_4999916326087304913[24] = -state[5];
   out_4999916326087304913[25] = 0;
   out_4999916326087304913[26] = state[3];
   out_4999916326087304913[27] = 0;
   out_4999916326087304913[28] = 0;
   out_4999916326087304913[29] = 0;
   out_4999916326087304913[30] = 0;
   out_4999916326087304913[31] = 1;
   out_4999916326087304913[32] = 0;
   out_4999916326087304913[33] = 0;
   out_4999916326087304913[34] = 1;
   out_4999916326087304913[35] = 0;
   out_4999916326087304913[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_4999916326087304913[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_4999916326087304913[38] = 0;
   out_4999916326087304913[39] = -state[7];
   out_4999916326087304913[40] = state[6];
   out_4999916326087304913[41] = 0;
   out_4999916326087304913[42] = state[4];
   out_4999916326087304913[43] = -state[3];
   out_4999916326087304913[44] = 0;
   out_4999916326087304913[45] = 0;
   out_4999916326087304913[46] = 0;
   out_4999916326087304913[47] = 0;
   out_4999916326087304913[48] = 0;
   out_4999916326087304913[49] = 0;
   out_4999916326087304913[50] = 1;
   out_4999916326087304913[51] = 0;
   out_4999916326087304913[52] = 0;
   out_4999916326087304913[53] = 1;
}
void h_13(double *state, double *unused, double *out_985319506289674804) {
   out_985319506289674804[0] = state[3];
   out_985319506289674804[1] = state[4];
   out_985319506289674804[2] = state[5];
}
void H_13(double *state, double *unused, double *out_3649562828813991335) {
   out_3649562828813991335[0] = 0;
   out_3649562828813991335[1] = 0;
   out_3649562828813991335[2] = 0;
   out_3649562828813991335[3] = 1;
   out_3649562828813991335[4] = 0;
   out_3649562828813991335[5] = 0;
   out_3649562828813991335[6] = 0;
   out_3649562828813991335[7] = 0;
   out_3649562828813991335[8] = 0;
   out_3649562828813991335[9] = 0;
   out_3649562828813991335[10] = 0;
   out_3649562828813991335[11] = 0;
   out_3649562828813991335[12] = 0;
   out_3649562828813991335[13] = 0;
   out_3649562828813991335[14] = 0;
   out_3649562828813991335[15] = 0;
   out_3649562828813991335[16] = 0;
   out_3649562828813991335[17] = 0;
   out_3649562828813991335[18] = 0;
   out_3649562828813991335[19] = 0;
   out_3649562828813991335[20] = 0;
   out_3649562828813991335[21] = 0;
   out_3649562828813991335[22] = 1;
   out_3649562828813991335[23] = 0;
   out_3649562828813991335[24] = 0;
   out_3649562828813991335[25] = 0;
   out_3649562828813991335[26] = 0;
   out_3649562828813991335[27] = 0;
   out_3649562828813991335[28] = 0;
   out_3649562828813991335[29] = 0;
   out_3649562828813991335[30] = 0;
   out_3649562828813991335[31] = 0;
   out_3649562828813991335[32] = 0;
   out_3649562828813991335[33] = 0;
   out_3649562828813991335[34] = 0;
   out_3649562828813991335[35] = 0;
   out_3649562828813991335[36] = 0;
   out_3649562828813991335[37] = 0;
   out_3649562828813991335[38] = 0;
   out_3649562828813991335[39] = 0;
   out_3649562828813991335[40] = 0;
   out_3649562828813991335[41] = 1;
   out_3649562828813991335[42] = 0;
   out_3649562828813991335[43] = 0;
   out_3649562828813991335[44] = 0;
   out_3649562828813991335[45] = 0;
   out_3649562828813991335[46] = 0;
   out_3649562828813991335[47] = 0;
   out_3649562828813991335[48] = 0;
   out_3649562828813991335[49] = 0;
   out_3649562828813991335[50] = 0;
   out_3649562828813991335[51] = 0;
   out_3649562828813991335[52] = 0;
   out_3649562828813991335[53] = 0;
}
void h_14(double *state, double *unused, double *out_8223911283974111787) {
   out_8223911283974111787[0] = state[6];
   out_8223911283974111787[1] = state[7];
   out_8223911283974111787[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2898595797806839607) {
   out_2898595797806839607[0] = 0;
   out_2898595797806839607[1] = 0;
   out_2898595797806839607[2] = 0;
   out_2898595797806839607[3] = 0;
   out_2898595797806839607[4] = 0;
   out_2898595797806839607[5] = 0;
   out_2898595797806839607[6] = 1;
   out_2898595797806839607[7] = 0;
   out_2898595797806839607[8] = 0;
   out_2898595797806839607[9] = 0;
   out_2898595797806839607[10] = 0;
   out_2898595797806839607[11] = 0;
   out_2898595797806839607[12] = 0;
   out_2898595797806839607[13] = 0;
   out_2898595797806839607[14] = 0;
   out_2898595797806839607[15] = 0;
   out_2898595797806839607[16] = 0;
   out_2898595797806839607[17] = 0;
   out_2898595797806839607[18] = 0;
   out_2898595797806839607[19] = 0;
   out_2898595797806839607[20] = 0;
   out_2898595797806839607[21] = 0;
   out_2898595797806839607[22] = 0;
   out_2898595797806839607[23] = 0;
   out_2898595797806839607[24] = 0;
   out_2898595797806839607[25] = 1;
   out_2898595797806839607[26] = 0;
   out_2898595797806839607[27] = 0;
   out_2898595797806839607[28] = 0;
   out_2898595797806839607[29] = 0;
   out_2898595797806839607[30] = 0;
   out_2898595797806839607[31] = 0;
   out_2898595797806839607[32] = 0;
   out_2898595797806839607[33] = 0;
   out_2898595797806839607[34] = 0;
   out_2898595797806839607[35] = 0;
   out_2898595797806839607[36] = 0;
   out_2898595797806839607[37] = 0;
   out_2898595797806839607[38] = 0;
   out_2898595797806839607[39] = 0;
   out_2898595797806839607[40] = 0;
   out_2898595797806839607[41] = 0;
   out_2898595797806839607[42] = 0;
   out_2898595797806839607[43] = 0;
   out_2898595797806839607[44] = 1;
   out_2898595797806839607[45] = 0;
   out_2898595797806839607[46] = 0;
   out_2898595797806839607[47] = 0;
   out_2898595797806839607[48] = 0;
   out_2898595797806839607[49] = 0;
   out_2898595797806839607[50] = 0;
   out_2898595797806839607[51] = 0;
   out_2898595797806839607[52] = 0;
   out_2898595797806839607[53] = 0;
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
void pose_err_fun(double *nom_x, double *delta_x, double *out_8549168006619746742) {
  err_fun(nom_x, delta_x, out_8549168006619746742);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5584185916794207075) {
  inv_err_fun(nom_x, true_x, out_5584185916794207075);
}
void pose_H_mod_fun(double *state, double *out_8554003597594456449) {
  H_mod_fun(state, out_8554003597594456449);
}
void pose_f_fun(double *state, double dt, double *out_783971397319598562) {
  f_fun(state,  dt, out_783971397319598562);
}
void pose_F_fun(double *state, double dt, double *out_804064847722436257) {
  F_fun(state,  dt, out_804064847722436257);
}
void pose_h_4(double *state, double *unused, double *out_4471711811158414269) {
  h_4(state, unused, out_4471711811158414269);
}
void pose_H_4(double *state, double *unused, double *out_184192634488532689) {
  H_4(state, unused, out_184192634488532689);
}
void pose_h_10(double *state, double *unused, double *out_2904425780055764226) {
  h_10(state, unused, out_2904425780055764226);
}
void pose_H_10(double *state, double *unused, double *out_4999916326087304913) {
  H_10(state, unused, out_4999916326087304913);
}
void pose_h_13(double *state, double *unused, double *out_985319506289674804) {
  h_13(state, unused, out_985319506289674804);
}
void pose_H_13(double *state, double *unused, double *out_3649562828813991335) {
  H_13(state, unused, out_3649562828813991335);
}
void pose_h_14(double *state, double *unused, double *out_8223911283974111787) {
  h_14(state, unused, out_8223911283974111787);
}
void pose_H_14(double *state, double *unused, double *out_2898595797806839607) {
  H_14(state, unused, out_2898595797806839607);
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
