#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Timer.h"

void Timer_Init (void)
{
	PFCON = PFCON_REGS;
	TMOD = TMOD_REGS;
	TH0 = TIMER0_TH;
	TL0 = TIMER0_TL;
	TR0 = TIMER0_ENABLE;
	
	TH1 = TIMER1_TH;
	TL1 = TIMER1_TL;
	TR1 = TIMER1_ENABLE;
	
	T2CON = T2CON_REGS;
	TH2 = TIMER2_TH;
	TL2 = TIMER2_TL;
	TR2 = TIMER2_ENABLE;
}
