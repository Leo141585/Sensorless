#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Adc.h"
#include "Motor.h"

#define BUSY 0x08
#define START 0x01

void Adc_Init (void)
{
	ADCCONT = ADCCONT_REGS;
	ADCSTR = ADCSTR_REGS | OPA_GAIN_REGS;
	
	SFR_PAGE = 0;
	ADCOFST = 512;//ADCOFST Init : 512
	SFR_PAGE = 1;
	ADCOFST = 512;//ADCOFST Init : 512;
}
//#if ((POWER_CONTROL == 1) || (POWER_LIMIT == 1) || (VSP_TRI == 1) || (Vbus_Protect == 1) || (Temperture_Protect == 1))
unsigned short Adc_Cannel (unsigned char Channel)
{
	unsigned short Temp;
	
	ADCCONT = (ADCCONT | Channel);
	ADCSTR = (ADCSTR | START);
	while((ADCSTR & BUSY) == BUSY){}; //Check ADC Busy end
	Temp = ADCD1;
	Temp = Temp << 8;
	Temp |= ADCD2;
	ADCCONT = (ADCCONT & 0xF8);
	return Temp;
}
//#endif