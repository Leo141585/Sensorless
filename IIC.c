#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "IIC.h"

#define IICEnable 0x80
#define IICMaster 0x40
#define IICSlave 0x00
#define IICBaudRate 0x03

#define IICSendData 0x40
#define IICSendAdd 0x80
#define IICStop 0xC0

#define IICAddWrite 0x00
#define IICAddRead 0x01

#define MCP4728DeviceAdd 0xC0

//unsigned char IICWorkFlag = 0;
//unsigned char IICStatus = 0;
unsigned char IICStep = 0;
unsigned char IICFlag = 0;
unsigned char IICSendFlag = 0;
unsigned short IICCount = 20;
unsigned short IICCount1 = 0;
unsigned char IICTxFinish = 1;
unsigned char IICTxCounter;
unsigned char IICTxCounter;
xdata unsigned char IICBuf[10];

void IIC_MCP4628_Package_Build (void)
{
	IICCount1 += 100;
	IICCount1 &= 0x0FFF;
	if(IICTxFinish >= 1)
	{
		//IICBuf[0] = 0x40;
		//IICBuf[0] = IICCount1 >> 8;
		//IICBuf[1] = IICCount1 & 0x00FF;
		IICBuf[2] = IICCount1 >> 8;
		IICBuf[3] = IICCount1 & 0x00FF;
		IICBuf[4] = IICCount1 >> 8;
		IICBuf[5] = IICCount1 & 0x00FF;
		IICBuf[6] = IICCount1 >> 8;
		IICBuf[7] = IICCount1 & 0x00FF;// Data 9th
		IICTxFinish = 0;
		IICTxCounter = 0;
		IICTL = IICEnable | IICMaster | IICBaudRate | 0x00 | 0x10 | 0x08;
		IICA1 = MCP4728DeviceAdd | IICAddWrite;// Data 1st
		IICEBT = IICSendAdd;
		//IICTxCounter++;
	}
}

void IIC_MCP4728_Send_Buffer (void)
{
	if(IICTxCounter >= 8)// DataLen
	{
		IICEBT = IICStop;
		IICTxFinish = 1;
		IICFlag = 0;
	}
	
	if(IICTxFinish <= 0)
	{
		IICRWD = IICBuf[IICTxCounter];
		IICEBT = IICSendData;
		IICTxCounter++;
	}
}
