#include "ET1202_SFR.h"
#include "UartSystem.h"
#include "Main.h"
#include "Motor.h"
#include "PWM.h"

xint16_t Debug_A;
xint16_t Debug_B;
xint16_t Debug_C;
xint16_t Debug_D;
xint16_t Debug_E;
xint16_t Debug_F;

xuint8_t TxFinish;
xuint8_t RxState;
xuint8_t Debug_TxCounter;
xuint8_t Debug_RxCounter;
xuint8_t Debug_PreRxCounter;
xuint8_t Debug_TxBuffer[20];
xuint8_t Debug_RxBuffer[20];
xuint8_t Debug_TxDataLen;

//unsigned short CheckSUM;
unsigned char CheckSUMFlag;
unsigned char RxTimeOutCount = 0;

//======================================================================
/*
 * Name         : UART_definition 
 *
 * Synopsis     : void UART_DEFINITION(void)
 */
 
// 公式 : (1024 - ( (24Mhz/8) / Baudrate_115200)
#define Baudrate_115200 0x03E5

// 公式 : (1024 - ( (24Mhz/8) / Baudrate_9600)
#define Baudrate_9600 0x02C7

// 公式 : (1024 - ( (24Mhz/64) / Baudrate_9600)
//#define Baudrate_9600 0x03D9

void Uart_Definition(void)
{
	PINSET6 |= 0x0A;	//TX, RX PULL UP; MOCS = 1
	PINCONG6 |= 0x01;	//RX Quasi-bidirectional
										//TX Push-pull output
	AUX = 0x04;
	PFCON = 0x30;		//[5:4] SRELPS 00:F_PER/64, 01:F_PER/32, 10:F_PER/16, 11:F_PER/8
									//[3:2] T1PS   00:F_PER/12, 01:F_PER,    10:F_PER/96
									//[1:0] T0PS   00:F_PER/12, 01:F_PER,    10:F_PER/96
	SCON = 0x52;		//8 bit UART, enable Serial reception
									
	SRELH = Baudrate_115200 >> 8;
	SRELL = Baudrate_115200 & 0xFF;

}
//======================================================================
/*
 * Name         : UART_PACKAGE_BUILD 
 *
 * Synopsis     : void UART_PACKAGE_BUILD(void)
 *
 * Description  : Build slave package to response.
 * 
 */

void Uart_Package_Build(void)
{
	unsigned short CheckSum = 0;
	unsigned short i = 0;

	if(TxFinish == TX_FINISH)
	{
		Debug_TxBuffer[0] = 0x28;// Start Byte : 0x28
		Debug_TxBuffer[1] = 0x41;// MCU Cmd Byte : 0x41
		Debug_TxBuffer[2] = 0x16;// DataType : 
		Debug_TxBuffer[3] = 0x0C;// DataLength : 0x0C (12 Byte DataLength)
		Debug_TxBuffer[4] = ((Debug_A) & 0xFF );
		Debug_TxBuffer[5] = ((Debug_A>>8) & 0xFF );
		Debug_TxBuffer[6] = ((Debug_B) & 0xFF );
		Debug_TxBuffer[7] = ((Debug_B>>8) & 0xFF );
		Debug_TxBuffer[8] = ((Debug_C) & 0xFF );
		Debug_TxBuffer[9] = ((Debug_C>>8) & 0xFF );
		Debug_TxBuffer[10] = ((Debug_D) & 0xFF );
		Debug_TxBuffer[11] = ((Debug_D>>8) & 0xFF );
		Debug_TxBuffer[12] = ((Debug_E) & 0xFF );
		Debug_TxBuffer[13] = ((Debug_E>>8) & 0xFF );
		Debug_TxBuffer[14] = ((Debug_F) & 0xFF );
		Debug_TxBuffer[15] = ((Debug_F>>8) & 0xFF );
		
		CheckSum = 0;
		for(i = 0;i < 16; i++)
		{
			CheckSum += Debug_TxBuffer[i];
		}

		Debug_TxBuffer[16] = ((CheckSum) & 0xFF );
		Debug_TxBuffer[17] = ((CheckSum>>8) & 0xFF );

		TxFinish = TX_SEND;
		Debug_TxDataLen = 18;

		Debug_TxCounter = 0;
		SBUF = Debug_TxBuffer[Debug_TxCounter];
		Debug_TxCounter++;
	}
}

void Uart_ReadPackage_Build(void)
{
	short Temp;
	
	// Check Sum Fun 目前有機率漏封包 需加 CheckSum 機制
	Uart_CheckSum_Fun();

	if(CheckSUMFlag == 1)
	{
		CheckSUMFlag = 0;
		switch(Debug_RxBuffer[2])
		{
			case 0:
				switch(Debug_RxBuffer[4])
				{
					case 1:
						SystemState ^= 0x01;
						break;
					case 2:
						SystemState ^= 0x02;
						break;
					case 3:
						if(MotorState == M_STOP)
							MotorState = M_RUN;
						else
							SystemState ^= 0x04;
						break;
					case 4:
						MotorState = M_STOP;
						break;
					case 5:
						IPDFlag ^= 1;
						break;
					case 6:
						CloseLoopFlag ^= 1;
						break;
					case 7:
						CCWFlag ^= 1;
						break;
					case 8:
						MotorState = M_REVERSE;
						break;
					default:
						MotorErrorState = 0;
						MotorState = 0;
						SystemState = 0;
						IPDFlag = 0;
						CloseLoopFlag = 0;
						break;
				}
				break;
			case 1:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				SpeedCmd = Temp;
				SPEED_CMD_MACRO((Temp * PLOE_PAIRS) >> AngleBaseShift);
				break;
			case 2:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				GS_MACRO(Temp);
				break;
			case 3:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				FS_MACRO(Temp);
				break;
			case 4:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				SMO_KSLIDE_MACRO(Temp);
				break;
			case 5:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				SMO_FILTER_MACRO(Temp);
				break;
			case 6:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				IQ_KP_MACRO(Temp);
				break;
			case 7:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				ID_KP_MACRO(Temp);
				break;
			case 8:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				SPEED_KP_MACRO(Temp);
				break;
			case 9:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				PLL_KP_MACRO(Temp);
				break;
			case 10:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				IQ_KI_MACRO(Temp);
				break;
			case 11:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				ID_KI_MACRO(Temp);
				break;
			case 12:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				SPEED_KI_MACRO(Temp);
				break;
			case 13:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				PLL_KI_MACRO(Temp);
				break;
			case 14:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				ID_CMD_MACRO(Temp);
				break;
			case 15:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				AS_MACRO(Temp);
				break;
			case 16:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				CurrentCmd = Temp;
				IQ_CMD_MACRO(CurrentCmd);
				break;
			case 17:
				Temp = Debug_RxBuffer[5]<<8;
				Temp |= Debug_RxBuffer[4];
				VD_OFFSET_MACRO(Temp);
				break;
			default:
				MotorErrorState = 0;
				MotorState = 0;
				SystemState = 0;
				IPDFlag = 0;
				CloseLoopFlag = 0;
				break;
		}
	}

	Debug_RxCounter = 0;
	RxState = RX_INT;
}

void Uart_Read_Handle(void)
{
	if(RxState == RX_HANDLE)
	{
		if(Debug_PreRxCounter == Debug_RxCounter)
		{
			RxTimeOutCount += Timer1ms;
			if(RxTimeOutCount >= PeriodTime10ms)
			{
				RxState = RX_FINISH;// Debug_RxCounter 未變化經過 10ms, 代表 Uart Rx 結束
			}
		}
		else
		{
			RxTimeOutCount = 0;
		}
		Debug_PreRxCounter = Debug_RxCounter;
	}
}

void Uart_Send_Buffer(void)
{
	if(TxFinish <= TX_SEND)
	{
		SBUF = Debug_TxBuffer[Debug_TxCounter];
		Debug_TxCounter++;
	}
	
	if(Debug_TxCounter >= Debug_TxDataLen)
	{
		TxFinish = TX_FINISH;
		UartFlag = 0;
	}
}

void Uart_Read_Buffer(void)
{
	RxState = RX_HANDLE;// Rx Start Flag
	Debug_RxBuffer[Debug_RxCounter] = SBUF;
	Debug_RxCounter++;// DataLen
}

void Uart_CheckSum_Fun(void)
{
	unsigned char i;
	unsigned char RxDataLen = Debug_RxBuffer[3]+4;
	unsigned short CheckSUM;
	
	CheckSUMFlag = 0;
	CheckSUM = 0;
	for(i = 0; i < RxDataLen; i++)
	{
		CheckSUM += Debug_RxBuffer[i];
	}
	
	if((Debug_RxBuffer[RxDataLen] == (CheckSUM & 0x00FF))
		&& (Debug_RxBuffer[RxDataLen+1] == (CheckSUM & 0xFF00)>>8))
	{
		CheckSUMFlag = 1;
	}
}