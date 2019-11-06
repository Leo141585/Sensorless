#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "LowVoltage.h"

void LVD_Init (void)
{
	//AUX &= ~(0x80);
	AUX |= 0x80;
}