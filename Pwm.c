#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Pwm.h"
#include "Motor.h"

#if (BREAK_FUNCTION == 1)
void PWM_SetBreak (void)
{
	MPWMCONT1 = SWAP | ActiveLow<<2 | ForceLow;
	SYNC = 0x55;
	MPWMCONT2 = ActiveLow<<6 | ForceLow<<4 | ActiveLow<<2 | ForceLow;
	SYNC = 0x55;
}
#else
void PWM_SetBreakForce (void)
{
	MPWMCONT1 = SWAP | ForceHi<<2 | ForceLow;
	SYNC = 0x55;
	MPWMCONT2 = ForceHi<<6 | ForceLow<<4 | ForceHi<<2 | ForceLow;
	SYNC = 0x55;
}
#endif

void PWM_Duty (unsigned short PwmDuty)
{
	SFR_PAGE = 1;// PWM Duty U
	MPWMDATA = PwmDuty;
	SYNC = 0x55;
	SFR_PAGE = 2;// PWM Duty V
	MPWMDATA = PwmDuty;
	SYNC = 0x55;
	SFR_PAGE = 3;// PWM Duty W
	MPWMDATA = PwmDuty;
	SYNC = 0x55;
}

void PWM_SetAllOff (void)
{
	MPWMCONT1 = SWAP | 0x00;
	SYNC = 0x55;
	MPWMCONT2 = 0x00;
	SYNC = 0x55;
}

void PWM_SetActive (void)
{
	MPWMCONT1 = SWAP | ActiveLow<<2 | ActiveHi;
	SYNC = 0x55;
	MPWMCONT2 = ActiveLow<<6 | ActiveHi<<4 | ActiveLow<<2 | ActiveHi;
	SYNC = 0x55;
}

void PWM_Init (void)
{
	SFR_PAGE = 0;
	MPWMDATA = MPWMDATA_REGS;
	SYNC = 0x55;
	MPWMINV = MPWMINV_REGS;
	SYNC = 0x55;
	MPWMDB = MPWMDB_REGS;
	SYNC = 0x55;
	PWM_SetAllOff();
}
