#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Eeprom.h"

#define EEPDefaultStatus 0x0
#define EEPWriteStatus 0x1
#define EEPRaedStatus 0x2
#define EEPEraseStatus 0x3

unsigned char EEPBuf;
//unsigned char EEPBuf[8];
unsigned char EEPBusyFlag;
unsigned char EEPStatus;
unsigned char EEPStep;

void EEP_Init (unsigned char Add)
{
//	unsigned char i;//,EEPCheckSum;//,EEPBuf[8];
//		
//	EEP_Erase();
//	for(i = 0; i < 8; i++)
//	{
//		EEPBuf[i] = i;
//		Add += i;
//		EEP_Write(Add,EEPBuf[i]);
//	}
//	
#if 1
	if(EEP_Read(Add) == 0xFF)
	{
		EEP_Erase();
		EEP_Write(Add,10);
	}
	else if(EEP_Read(Add) == 0x00)
	{
		EEP_Erase();
		EEP_Write(Add,100);
	}
	else
	{
		EEPBuf = EEP_Read(Add);
		EEPBuf += 5;
		EEPBuf &= 0xFF;
		EEP_Erase();
		EEP_Write(Add,EEPBuf);
	}
#endif
}

void EEP_Erase (void)
{
	unsigned char Temp;
	
	SFR_PAGE = 2;
	EEPROM = EepErase;
	
	do
	{
		SFR_PAGE = 2;
		Temp = EEPROM;
	}
	while((Temp & EepEraseFlag) == EepEraseFlag);
}

void EEP_Write (unsigned char Add,unsigned char Data)
{
	unsigned char Temp;
	
	SFR_PAGE = 0;
	EEPROM = Add;
	SFR_PAGE = 1;
	EEPROM = Data;
	SFR_PAGE = 2;
	EEPROM = EepWrite;
	
	do
	{
		SFR_PAGE = 2;
		Temp = EEPROM;
	}
	while((Temp & EepWriteFlag) == EepWriteFlag);
}

unsigned char EEP_Read (unsigned char Add)
{
	unsigned char Temp;
	
	SFR_PAGE = 0;
	EEPROM = Add;
	SFR_PAGE = 2;
	EEPROM = EepRead;
	
	do
	{
		SFR_PAGE = 2;
		Temp = EEPROM;
	}
	while((Temp & EepReadFlag) == EepReadFlag);
	
	SFR_PAGE = 1;
	Temp = EEPROM;
	return Temp;
}