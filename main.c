// Header:
// File Name:
// Author:
// Date:
#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "UartSystem.h"
#include "Gpio.h"
#include "Pwm.h"
#include "IAE.h"
#include "Adc.h"
#include "Timer.h"
#include "OCP.h"
#include "Motor.h"
#include "LowVoltage.h"
#include "WatchDog.h"
#include "Capture.h"
#include "External.h"
//#include "MCP4728.h"
//#include "CORDIC.h"
#include "PI_CTRL.h"
#include "Eeprom.h"
#include "IPwm.h"
#include "IRDecode.h"
#include "IIC.h"
#include "CRC.h"
#include "SleepMode.h"
//#include <intrins.h>
//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <rtx51tny.h>
//#include "DataType.h"

//xdata MCU_COMPONENT Mcu_Component;
//xdata PMCU_COMPONENT pMcu_Component =& Mcu_Component;

//MCU_COMPONENT Mcu_Component;
//PMCU_COMPONENT pMcu_Component =& Mcu_Component;

unsigned short StartDelayCount = 0;
unsigned char StartDelayFlag = 1;

unsigned short AdcCount = 0;
unsigned char AdcFlag = 0;
unsigned short AdcAutoData = 0;

unsigned short IPDCount = 0;
unsigned char IPDFlag = 0;

unsigned short UartCount = 10;
unsigned char UartFlag = 0;

unsigned char TimerFlag = 0;
unsigned char TimerCount = 0;

unsigned char IPwmFlag = 0;
unsigned char PwmFlag = 0;
unsigned short PwmCount = 0;

unsigned char LPF_flag = 0;
unsigned char OCP_flag = 0;
unsigned char SD_flag = 0;
unsigned char SoftReset_flag = 0;

unsigned short LPF_Out = 0;
unsigned short TimeOut = 0;

//void SD_Init (unsigned char PllClk,unsigned char SDMode);
void ET1202_Init (void);
void User_LPF (void);
void SoftReset (void);
//void MDU_u32_Div_u16 (void);
//void MDU_u16_Mpy_u16 (void);

#if 0
void User_LPF (void)
{
	unsigned short Temp;

	SFR_PAGE = 1;// CH2
	Temp = (ADCAUTO_H<<8) + ADCAUTO_L;

	// LPF_Out = Last_Out + GEN_Gain x (LPF_In - Last_Out)
	SFR_PAGE = 0;
	USER_LPF = 3;
	SFR_PAGE = 1;
	USER_LPF = Temp;
	SFR_PAGE = 3;
	USER_LPF = LPF_Out;// Last Out
	// Eanble LPF compute one time when GEN_LPF_ACT = 1
	MOTOR_CONT1 |= 0x02;
	SFR_PAGE = 2;
	LPF_Out = USER_LPF;
}

void SD_Init (unsigned char PllClk,unsigned char SDMode)
{
	SFR_PAGE = 0;
	SD_MODE = 0xA0 | PllClk | SDMode;
}
#endif

void SoftReset (void)
{
	SOFT_RST_KEY = 0xAA;
	SOFT_RST_KEY = 0x55;
	SOFT_RST_KEY = 0xA5;
	SOFT_RST_EN = 0x80;
}

void ET12x2_Init (void)
{
	EX0 = 0;//External0_ISR interrupt 0
	ET0 = 1;//Timer0_ISR interrupt 1
	EX1 = 0;//External1_ISR interrupt 2
	ET1 = 0;//Timer1_ISR interrupt 3
	ESP = 1;//Uart_ISR interrupt 4
	ET2 = 0;//Timer2_ISR interrupt 5
	OCPSIE = 1;//OCP_ISR interrupt 8
	ADCIE = 1;//ADC_ISR interrupt 9
	MPWMMINIE = 1;//PmwMin_ISR interrupt 10
	MPWMMAXIE = 0;//PmwMax_ISR interrupt 11
	IICIE = 0;//IIC_ISR interrupt 12
	LVDIIE = 0;//LowVoltage_ISR interrupt 13
	WDTIE = 0;//WatchDoag_ISR interrupt 14
	CAPIE = 1;//Cap_ISR interrupt 15
	//IP0 = 0x0C;//Interrupt Priority
	//IP1 = 0x06;//Group 2 > 3 > 1 > 0
	EA = 1;//Allow interrupt
}

void main (void)
{
	//if(SDModeDisable) SD_Init(PLLDiv1,SDMode1);
	PWM_SetAllOff();
	GPIO_Init();
	PWM_Init();
	Adc_Init();
	Timer_Init();
	OCP_Init();
	MOC_Init();
	ET12x2_Init();
	Uart_Definition();
	LVD_Init();
	//WatchDog_Init();// (註 : 當 IPD 時 會觸發 WatchDog Reset)
	IPWM_Init();
	//IRDecode_Init();
	//VdcCorrection_Init();
	//User_PI_Control_Init();
	//EXCAP_Init();
	CAP_Init();
	#if (POWER_CONTROL == 1)
	InitPI_ALL();
	InitPI(&PIParm_Watt);
	#endif
	#if ((POWER_CONTROL == 1) || (POWER_LIMIT == 1))
	//VbusOffset = Adc_Cannel(7);
	IbusOffset = Adc_Cannel(2);
	#endif
	EEP_Init(0xF0);

	while (1)
	{
		//VdcCorrection();
		//IRDecode_Read();
		//WatchDog_Refresh();
		
		if(AdcFlag == 1)
		{
			AdcFlag = 0;
			#if (Vbus_Protect == 1)
			VbusTemp = Adc_Cannel(6);
			Vbus_avg = (Vbus_avg * 15 + VbusTemp) >> 4;
			V_BUS = (unsigned int)((unsigned long)Vbus_avg << 4);
			#endif
			#if ((POWER_CONTROL == 1) || (POWER_LIMIT == 1))
			IbusTemp = Adc_Cannel(2);
			IbusTemp = IbusTemp - IbusOffset;
			Ibus_avg = (Ibus_avg * 15 + IbusTemp) >> 4;
			I_BUS = (unsigned int)((unsigned long)Ibus_avg << 4);
			#endif
			#if (VSP_TRI == 1)
			VspTemp = Adc_Cannel(4);
			Vsp_avg = (Vsp_avg * 15 + VspTemp) >> 4;
			#endif
		}

		if(SoftReset_flag == 1)
		{
			SoftReset();
			SoftReset_flag = 0;
		}

		//IPDFlag = 1;
		if(IPDFlag == 1)
		{
			if(IPDCount >= PeriodTime100ms)
			{
				//WatchDog_Disable();// ( 註 : 需注意當 IPD 時 會觸發 WatchDog Reset , Disable WatchDog)
				IPD_Init();
				//WatchDog_Init();
				IPDCount = 0;
			}
		}

		if(PwmFlag == 1)
		{
			PwmFlag = 0;
		}
		
		#if 0
		if(IPwmFlag == 1)
		{
			PwmCount++;
			PwmCount &= 0x3FF;
			IPWM_Duty(PwmCount);
			IPwmFlag = 0;
		}
		#endif

		if(RxState == RX_FINISH)
		{
			Uart_ReadPackage_Build();
		}
		
		if(UartFlag == 1)
		{
			SFR_PAGE = 0; Debug_A = MotorDir;//MotorState;//IPDPattern;//MotorDir;//MotorState;
			SFR_PAGE = 4; Debug_B = SMO_D1;//SMO_D2;//MotorSpeed
			SFR_PAGE = 0; Debug_C = PI_OUT;//Iq
			SFR_PAGE = 1; Debug_D = PI_OUT;//Id
			SFR_PAGE = 3; Debug_E = PI_OUT;//PLL
			SFR_PAGE = 2; Debug_F = PI_OUT;//Speed
			//Debug_A = (dFB_Ialpha_DRH * 256 | dFB_Ialpha_DRL);
			//Debug_B = (dES_Ialpha_DRH * 256 | dES_Ialpha_DRL);
			//Debug_A = (dFB_Ibeta_DRH * 256 | dFB_Ibeta_DRL);
			//Debug_B = (dES_Ibeta_DRH * 256 | dES_Ibeta_DRL);
			Uart_Package_Build();
			UartFlag = 0;
		}

		if(IICFlag == 1)
		{
			if(IICSendFlag == 1)
			{
				SFR_PAGE = 2;
				IICBuf[0] = PI_OUT >> 8;
				IICBuf[1] = PI_OUT & 0xFF;
				IIC_MCP4628_Package_Build();
				IICSendFlag = 0;
			}
		}

		if(TimerFlag == 1)
		{
			TimerFlag = 0;
			#if (VSP_TRI == 1)
			Vsp_Fun();
			#endif
			#if (FG_FUNCTION == 1)
			FG_Fun();
			#endif
			#if (FaultLock_Protect == 1)
			FaultLock_Fun();
			#endif
			#if (Vbus_Protect == 1)
			Vbus_Protect_Fun(Vbus_avg);
			#endif
			#if (Temperture_Protect == 1)
			Temperture_Protect_Fun(0);
			#endif
			if((TimerCount & 0x0F) == 0)// 0.16.32...etc 16ms
			{
				#if (POCP_Protect == 1)
				Phase_OCP_Protect_Fun();
				#endif
			}
			else if((TimerCount & 0x1F) == 2)//2.34...etc 2 *16ms
			{
				Read_Iq();
			}
			else if((TimerCount & 0x1F) == 4)//4.36...etc 2 *16ms
			{
				Read_Vq();
			}
			else if((TimerCount & 0x1F) == 6)//6.38...etc 2 *16ms
			{
				Read_Speed();
			}
		}
	}
}

void External0_ISR (void) interrupt 0
{
	//P3_3 = 0;
	//EX0 = 0;
}

void Timer0_ISR (void) interrupt 1
{
	TH0 = TIMER0_TH;//1mS @ 2MHz 65536 - 2000 = 63536 = 0xF830
	TL0 = TIMER0_TL;
	
	Motor_Control();
	
	//SFR_PAGE = 1; CAPCONT ^= 0x10;
	
	SystemState |= 0x04;// Start Test 
	if(StartDelayCount > 100)
		SystemState &= ~(0x08);
	else
		StartDelayCount++;
	
	#if (POWER_LIMIT == 1)
	PowerLimit_Fun();
	#endif
	#if (POWER_CONTROL == 1)
	PowerControl_Fun();
	#endif
	
	IPDCount += Timer1ms;
	IPDCount &= 0xFFFF;

	//LPF_flag = 1;
	//SoftReset_flag = 1;
//	IPwmFlag ^= 1;
//	if(IPwmFlag == 1) {SFR_PAGE = 1; CAPCONT |= 0x10;}
//	else {SFR_PAGE = 1; CAPCONT &= ~(0x10);}

	TimerFlag = 1;
	TimerCount += Timer1ms;
	TimerCount &= 0xFF;

	IICCount += Timer1ms;
	if(IICCount >= PeriodTime10ms)
	{
		IICFlag = 1;
		IICSendFlag = 1;
		IICCount = 0;
	}
	
	UartCount += Timer1ms;
	if(UartCount >= PeriodTime10ms)
	{
		UartFlag = 1;
		UartCount = 0;
	}
	
	Uart_Read_Handle();
	
	TF0 = 0;
}

void External1_ISR (void) interrupt 2
{
	//P3_3 = 0;
	//EX1 = 0;
}

void Timer1_ISR (void) interrupt 3
{
	TH1 = TIMER1_TH;//10mS @ 2MHz
	TL1 = TIMER1_TL;

	TF1 = 0;
}

void Uart_ISR (void) interrupt 4
{
	if(TI)
	{
		Uart_Send_Buffer();
		TI = 0;
	}
	
	if(RI)
	{
		Uart_Read_Buffer();
		RI = 0;
	}
}

void Timer2_ISR (void) interrupt 5
{
	TH2 = TIMER2_TH;//10mS @ 2MHz
	TL2 = TIMER2_TL;
		
	TF2 = 0;
}

void OCP_ISR (void) interrupt 8
{
	AOCP_Protect_Fun();
}

void ADC_ISR (void) interrupt 9
{
	AdcFlag = 1;
	#if (BEMF_TAILWIND_FUNCTION_1 == 1) || (BEMF_TAILWIND_FUNCTION_3 == 1)
	if(MotorState == M_TAILWIND)
	{
		#if (BEMF_TAILWIND_FUNCTION_3 == 1)
		if(!ZeroPointCheck)
			Break_Fun();
		else
			PWM_SetActive();
		#endif
		MotorDirCheck_Fun();
	}
	#endif
}

void PmwMin_ISR (void) interrupt 10
{
	PwmFlag = 1;
}

void PmwMax_ISR (void) interrupt 11
{

}

void IIC_ISR (void) interrupt 12
{
	if((IICS & 0x08))// The data Transmit Interrupt Flag
	{
		IIC_MCP4728_Send_Buffer();
		IICS &= (~0x08);
	}

	if((IICS & 0x10))// The data Receive Interrupt Flag
	{
		IICS &= (~0x10);
	}

	if((IICS & 0x40))// The Stop condition Interrupt Flag
	{
		IICS &= (~0x40);
	}
}

void LowVoltage_ISR (void) interrupt 13
{

}

void WatchDoag_ISR (void) interrupt 14
{

}

void Cap_ISR (void) interrupt 15
{
//	SFR_PAGE = 0;
//	if((CAPCONT & 0x40) == 0x40) CAPCONT |= 0x08;
//	BEMF_SPEED = SPEED_VAL / CaptureTemp;
//	CaptureTemp = (CAPT_H * 256) + CAPT_L;
	
	//SFR_PAGE = 1;
	//if((CAPCONT & 0x40) == 0x40) CAPCONT |= 0x08;
	CaptureTotal = (CAPT_H * 256) + CAPT_L;
	CAPCONT |= 0x08;
}
