#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Capture.h"

#define E_CAPCKS 0x07
#define EOK_CLR 0x08
#define CAPPINSEL 0x00
#define E_CAP_OK 0x40
#define E_CAP_EN 0x80

unsigned short CaptureTotal;
unsigned short CaptureTotalTemp;

void EXCAP_Init (void)
{
	SFR_PAGE = 0; // External 
	CAPCONT = CAPCONT_REGS;
	//CAPCONT = E_CAP_EN | CAPPINSEL | 0x03;
	//CAPCONT = E_CAP_EN | CAPPINSEL | EOK_CLR | E_CAPCKS;
}
#if 1
void CAP_Init (void)
{
	SFR_PAGE = 1; // Internal 
	CAPCONT = CAPCONT_REGS;
	//CAPCONT = E_CAP_EN | CAPPINSEL | 0x07;
	//CAPCONT = E_CAP_EN | CAPPINSEL | EOK_CLR | E_CAPCKS;
}
#endif