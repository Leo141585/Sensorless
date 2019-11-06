#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "WatchDog.h"

#if 0
void WatchDog_Refresh (void)
{
	WDTK = 0x55;//Refresh WDT
}

void WatchDog_Init (void)
{
	//For example, enable the watchdog with a time-out reset period of 5.461ms
	// 5.461ms = (375kHz / 8) * 256
	TAKEY = 0x55;
	TAKEY = 0xAA;
	TAKEY = 0x5A;//WDTC write is available
	#if 1
	WDTC = 0x23;//WDTM[3:0]=0111b, time-out period is 5.461ms, enable the WDT Reset
	#else
	WDTC = 0x63;//WDTM[3:0]=0111b, time-out period is 5.461ms, enable WDT interrupt & WDT Reset
	#endif
	WDTK = 0x55;//Refresh WDT
}

void WatchDog_Disable (void)
{
	//For example, enable the watchdog with a time-out reset period of 5.461ms
	// 5.461ms = (375kHz / 8) * 256
	TAKEY = 0x55;
	TAKEY = 0xAA;
	TAKEY = 0x5A;//WDTC write is available
	#if 1
	WDTC = 0x03;//WDTM[3:0]=0111b, time-out period is 5.461ms, enable the WDT Reset
	#else
	WDTC = 0x63;//WDTM[3:0]=0111b, time-out period is 5.461ms, enable WDT interrupt
	#endif
	WDTK = 0x55;//Refresh WDT
}
#endif
