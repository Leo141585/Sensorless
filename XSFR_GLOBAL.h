#ifndef _XSFR_GLOBAL_H
#define _XSFR_GLOBAL_H
#include <absacc.h>
//---------- XRAM SFR -------------
//------ 0x1018 ~ 0x101F -------------
#define CRC_CTRL XBYTE[0x1018]
#define CRC_DIN XBYTE[0x1019]
#define CRC_DOUT_L XBYTE[0x101A]
#define CRC_DOUT_H XBYTE[0x101B]
#define CRC_STR_BANK XBYTE[0x101C]
#define CRC_END_BANK XBYTE[0x101D]
//------ 0x1020 ~ 0x1027 -------------
//#define IPWM_CYC *(volatile unsigned int xdata *)0x1021
#define IPWM_CYC_L XBYTE[0x1020]
#define IPWM_CYC_H XBYTE[0x1021]
//#define IPWM_DUTY *(volatile unsigned int xdata *)0x1023
#define IPWM_DUTY_L XBYTE[0x1022]
#define IPWM_DUTY_H XBYTE[0x1023]
#define IPWM_CTRL XBYTE[0x1024]
#define FG_CTRL XBYTE[0x1027]
//------ 0x1028 ~ 0x102F -------------
#define SOFT_RST_KEY XBYTE[0x1028]
#define SOFT_RST_EN XBYTE[0x1029]
//------ 0x1030 ~ 0x1037 -------------
#define MD_MODE XBYTE[0x1030]
#define MD_CONT XBYTE[0x1031]
#define MD0 XBYTE[0x1032]
#define MD1 XBYTE[0x1033]
#define MD2 XBYTE[0x1034]
#define MD3 XBYTE[0x1035]
#define MD4 XBYTE[0x1036]
#define MD5 XBYTE[0x1037]
//------ 0x1038 ~ 0x103F -------------
#define IR_DEC_SET XBYTE[0x1038]
#define IR_DEC_CTRL XBYTE[0x1039]
#define IR_HEADER_Z1_L XBYTE[0x103A]
#define IR_HEADER_Z1_H XBYTE[0x103B]
#define IR_HEADER_Z2_L XBYTE[0x103C]
#define IR_HEADER_Z2_H XBYTE[0x103D]
#define IR_STOP_Z_L XBYTE[0x103E]
#define IR_STOP_Z_H XBYTE[0x103F]
//------ 0x1040 ~ 0x1047 -------------
#define IR_DOUT0 XBYTE[0x1040]
#define IR_DOUT1 XBYTE[0x1041]
#define IR_DOUT2 XBYTE[0x1042]
#define IR_DOUT3 XBYTE[0x1043]
#define IR_DOUT4 XBYTE[0x1044]
#define IR_DOUT5 XBYTE[0x1045]
//------ 0x1058 ~ 0x105F -------------
//#define dFB_Ib *(volatile unsigned int xdata *)0x1059
#define dFB_Ib_DRH XBYTE[0x1059]
#define dFB_Ib_DRL XBYTE[0x1058]
//#define dFB_Ibeta *(volatile unsigned int xdata *)0x105B
#define dFB_Ibeta_DRH XBYTE[0x105B]
#define dFB_Ibeta_DRL XBYTE[0x105A]
//#define dVbeta *(volatile unsigned int xdata *)0x105D
#define dVbeta_DRH XBYTE[0x105D]
#define dVbeta_DRL XBYTE[0x105C]
//#define dVb *(volatile unsigned int xdata *)0x105F
#define dVb_DRH XBYTE[0x105F]
#define dVb_DRL XBYTE[0x105E]
//------ 0x1060 ~ 0x1067 -------------
#define dFB_Ia *(volatile unsigned int xdata *)0x1061
#define dFB_Ia_DRH XBYTE[0x1061]
#define dFB_Ia_DRL XBYTE[0x1060]
//#define dFB_Ialpha *(volatile unsigned int xdata *)0x1063
#define dFB_Ialpha_DRH XBYTE[0x1063]
#define dFB_Ialpha_DRL XBYTE[0x1062]
//#define dValpha *(volatile unsigned int xdata *)0x1065
#define dValpha_DRH XBYTE[0x1065]
#define dValpha_DRL XBYTE[0x1064]
//#define dVa *(volatile unsigned int xdata *)0x1067
#define dVa_DRH XBYTE[0x1067]
#define dVa_DRL XBYTE[0x1066]
//------ 0x1068 ~ 0x106F -------------
//#define dZbeta *(volatile unsigned int xdata *)0x1069
#define dZbeta_DRH XBYTE[0x1069]
#define dZbeta_DRL XBYTE[0x1068]
//#define dES_Ibeta *(volatile unsigned int xdata *)0x106B
#define dES_Ibeta_DRH XBYTE[0x106B]
#define dES_Ibeta_DRL XBYTE[0x106A]
//#define dES_Ebeta *(volatile unsigned int xdata *)0x106D
#define dES_Ebeta_DRH XBYTE[0x106D]
#define dES_Ebeta_DRL XBYTE[0x106C]
//------ 0x1070 ~ 0x1077 -------------
//#define dZalpha *(volatile unsigned int xdata *)0x1071
#define dZalpha_DRH XBYTE[0x1071]
#define dZalpha_DRL XBYTE[0x1070]
//#define dES_Ialpha *(volatile unsigned int xdata *)0x1073
#define dES_Ialpha_DRH XBYTE[0x1073]
#define dES_Ialpha_DRL XBYTE[0x1072]
//#define dES_Ealpha *(volatile unsigned int xdata *)0x1074
#define dES_Ealpha_DRH XBYTE[0x1075]
#define dES_Ealpha_DRL XBYTE[0x1074]

//==================================

extern void DAC_FUNCTION(unsigned int DAC_CHA,unsigned int DAC_CHB,unsigned int DAC_CHC,unsigned int DAC_CHD);

/*
Function :DAC setting and Register
*/

//extern xdata unsigned char DAC_CTRL;
//extern xdata unsigned char DAC_CHA_L;
//extern xdata unsigned char DAC_CHA_H;
//extern xdata unsigned char DAC_CHB_L;
//extern xdata unsigned char DAC_CHB_H;
//extern xdata unsigned char DAC_CHC_L;
//extern xdata unsigned char DAC_CHC_H;
//extern xdata unsigned char DAC_CHD_L;
//extern xdata unsigned char DAC_CHD_H;

/*
Function :FG setting and Register
*/
//extern xdata unsigned char FG_CTRL;

/*
Function :IPWM setting and Register
*/
//extern xdata unsigned char IPWM_CTRL;
//extern xdata unsigned char IPWM_DUTY_H;
//extern xdata unsigned char IPWM_DUTY_L;
//extern xdata unsigned char IPWM_CYC_H;
//extern xdata unsigned char IPWM_CYC_L;

#endif
