#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
//#include "ET1202_INIT.h"
//#include <intrins.h>
//#include <math.h>
//#include <stdio.h>
/*
Function :DAC setting and Register
*/

//#define HIBYTE(const) ((unsigned char)((const)>>8))
//#define LOBYTE(const) ((unsigned char)((const)))
//	
//xdata unsigned char DAC_CTRL		_at_ 0x101F;
//xdata unsigned char DAC_CHA_L		_at_ 0x1010;
//xdata unsigned char DAC_CHA_H		_at_ 0x1011;
//xdata unsigned char DAC_CHB_L		_at_ 0x1012;
//xdata unsigned char DAC_CHB_H		_at_ 0x1013;
//xdata unsigned char DAC_CHC_L		_at_ 0x1014;
//xdata unsigned char DAC_CHC_H		_at_ 0x1015;
//xdata unsigned char DAC_CHD_L		_at_ 0x1016;
//xdata unsigned char DAC_CHD_H		_at_ 0x1017;

/*
Function :FG setting and Register
*/
//xdata unsigned char FG_CTRL			_at_ 0x1027;


/*
Function :IPWM setting and Register
*/
//xdata unsigned char IPWM_CTRL			_at_ 0x1024;
//xdata unsigned char IPWM_DUTY_H		_at_ 0x1023;
//xdata unsigned char IPWM_DUTY_L		_at_ 0x1022;
//xdata unsigned char IPWM_CYC_H		_at_ 0x1021;
//xdata unsigned char IPWM_CYC_L		_at_ 0x1020;

/*
DAC_CH1	FPGA	M6
DAC_CH2 FPGA	P6
DAC_CH3 FPGA	N6
DAC_CH4 FPGA	P5
*/
//void DAC_FUNCTION(unsigned int DAC_CHA,unsigned int DAC_CHB,unsigned int DAC_CHC,unsigned int DAC_CHD)
//{
//	DAC_CHA_L=HIBYTE(DAC_CHA);
//	DAC_CHA_H=LOBYTE(DAC_CHA);
//	
//	DAC_CHB_L=HIBYTE(DAC_CHB);
//	DAC_CHB_H=LOBYTE(DAC_CHB);
//	
//	DAC_CHC_L=HIBYTE(DAC_CHC);
//	DAC_CHC_H=LOBYTE(DAC_CHC);
//	
//	DAC_CHD_L=HIBYTE(DAC_CHD);
//	DAC_CHD_H=LOBYTE(DAC_CHD);
//}