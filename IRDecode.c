#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "IRDecode.h"

#define dHEADER_EN 0x80 // With Header
#define dDOUT_REV 0x40 // First MSB
#define dDIN_TYP 0x00 // Type 1
#define dDIN_DB 0x10 // 500ns
#define dCLK_DIV_SEL 0x02 // 8MHz

#define dIR_DEC_EN 0x80
#define dIRIN_SEL_CH3 0x00

#if 0
unsigned char IR_Byte[5];

void IRDecode_Init (void)
{
	IR_DEC_SET = dHEADER_EN | dDOUT_REV | dDIN_TYP | dDIN_DB | dCLK_DIV_SEL;
	IR_HEADER_Z1_H = 0x5D;//Set HEADER_Z1 3ms / 8MHz = 24000 = 0x5DC0
	IR_HEADER_Z1_L = 0xC0;//Set HEADER_Z2 2ms / 8MHz = 16000 = 0x3E80
	IR_HEADER_Z2_H = 0x3E;
	IR_HEADER_Z2_L = 0x80;
	IR_STOP_Z_H = 0x5D;//Set STOP_Z 3ms
	IR_STOP_Z_L = 0xC0;
	IR_DEC_CTRL = dIR_DEC_EN | dIRIN_SEL_CH3;
}

void IRDecode_Read (void)
{
		if((IR_DEC_CTRL & 0x10) == 0x10)
		{
			//P3_2 ^= 1;
			IR_DEC_CTRL |= 0x01;
			IR_Byte[0] = IR_DOUT0;
			IR_Byte[1] = IR_DOUT1;
			IR_Byte[2] = IR_DOUT2;
			IR_Byte[3] = IR_DOUT3;
			IR_Byte[4] = IR_DOUT4;
			IR_Byte[5] = IR_DOUT5;
		}
}
#endif