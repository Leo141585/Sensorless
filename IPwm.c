#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "IPwm.h"

#define IPWM_MODE 0x06
#define IPWM_EN 0x01

void IPWM_Init (void)
{
	IPWM_CTRL = IPWM_MODE | IPWM_EN;
	
	IPWM_CYC_H = (IPWM_Counter & 0xFF00)>>8;//0x04;
	IPWM_CYC_L = (IPWM_Counter & 0x00FF);//0xB0;
	SYNC = 0x55;
}

#if 0
void IPWM_Duty (unsigned short PwmDuty)
{
	IPWM_DUTY_H = (PwmDuty & 0xFF00)>>8;//0x02;
	IPWM_DUTY_L = (PwmDuty & 0x00FF);//0x58;
	SYNC = 0x55;
}
#endif