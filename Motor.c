#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "Motor.h"
#include "Pwm.h"
#include "IAE.h"
#include "Adc.h"
#include "Moc.h"
#include "Capture.h"
#include "PI_CTRL.h"
//#include "CORDIC.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>

//MOTOR_COMPONENT xdata Motor_Component;
//PMOTOR_COMPONENT xdata pMotor_Component =& Motor_Component;

unsigned char SystemState = 0x08;
unsigned char MotorState = 0;
unsigned char StartUpState = 0;
#if (Vbus_Protect == 1)
unsigned char MotorErrorState = OverVbus | UnderVbus;
#else
unsigned char MotorErrorState = 0;
#endif
unsigned char MotorErrorStateOld = 0;
unsigned char MotorFaultState = 0;
unsigned char MotorDir = 0;

//xdata short RampTemp;
short SpeedCmd = 0;
short SpeedCmdTemp = 0;
long SpeedCmd_Temp = 0;
short SpeedCmdLimit = SPEED_MAX_LIMIT_VALUE;
short CurrentCmd = 0;
short CurrentCmdTemp = 0;
short CurrentCmdLimit = IQ_MAX_LIMIT_VALUE;

unsigned short int V_BUS;
unsigned short int I_BUS;
unsigned long Watt;
#if ((POWER_CONTROL == 1) || (POWER_LIMIT == 1))
unsigned long Watt_InRef = WATT_LIMIT_VALUE;
#endif
unsigned short WattCount;
unsigned short VbusTemp;
unsigned short IbusTemp;
unsigned short Vbus_avg;
unsigned short Ibus_avg;
//unsigned short VbusOffset;
unsigned short IbusOffset;
unsigned short VspTemp;
unsigned short Vsp_avg;

//xdata short SetpointValue = 0;
xdata unsigned short EqualFlag = 0;

xdata unsigned short CloseLoopDelayCount = 0;
xdata unsigned char CloseLoopFlag = 0;
xdata unsigned char CCWFlag = CW_CCW;
xdata unsigned char CCWFlagOld = 0;

xdata short LatestTheta = 0;
xdata short LatestThetaTemp = 0;
xdata short LatestFreq = 0;
xdata short TailwindSpeed = 0;

xdata unsigned char AngleState = 0;
xdata unsigned char AngleStateOld = 0;
xdata char AngleStateCehckCount = 0;

#if (Vbus_Protect == 1)
xdata unsigned short OverVbusProtectCount = BUS_VOLT_DURATION;
xdata unsigned short UnderVbusProtectCount = BUS_VOLT_DURATION;
#endif
#if (Temperture_Protect == 1)
xdata unsigned short OverTempertureProtectCount = TEMPERTURE_DURATION;
#endif
#if (POCP_Protect == 1)
xdata unsigned short POCP_ProtectCount = PHASE_OCP_DURATION;
#endif
xdata unsigned short BreakCount = 0;
xdata unsigned short BreakPwmDuty = 0;

xdata unsigned short StopCheckCount = 0;
xdata unsigned short AlignmentCount = 0;
xdata unsigned short RampDelayCount = 0;
xdata unsigned short TailwindCount = 0;
xdata unsigned short TailwindTimeOut = 0;
xdata unsigned char MotorStartRetryCount = 0;
xdata unsigned short MotorStartDelayCount1 = 0;
xdata unsigned short MotorStartDelayCount2 = 0;
xdata unsigned short BumplessCount = 0;


xdata unsigned short MotorFaultDelayCount = 0;
#if (POWER_LIMIT  == 1)
xdata unsigned short PowerLimitDelayCount = 0;
#endif
#if (POWER_CONTROL == 1)
xdata unsigned short PowerControlDelayCount = 0;
#endif
xdata short EstimatedSpeed;
xdata short EstimatedSpeedOld;

xdata short IaFb;
xdata short IbFb;
xdata short IqFb;
xdata short IqPiOut;

xdata short BmfVW_Sub;
xdata short BmfVW_Sum_Pos_ZeroPoint;
xdata short BmfVW_Sum_Neg_ZeroPoint;
xdata short BmfU;
xdata short BmfV;
xdata short BmfW;
xdata char ZeroPointDebounceCnt;
xdata char ZeroPointCnt;
xdata char ZeroPointCheck = 0;
xdata char ZeroPointFlag;
xdata char ZeroPointFlagOld;

xdata unsigned short BEMF_SPEED = 0;
xdata unsigned short BEMF_SPEED_Cnt = 0;
xdata unsigned char BemfStateOld = 0;
xdata unsigned char BemfState = 0;
xdata unsigned char BemfCnt = 0;

xdata unsigned short HallUEdgeCnt = 20000;
xdata unsigned short RecHallUEdgeCnt = 20000;
xdata unsigned short SlowDownCnt = 0;

//xdata float f32_deltaAngle = 0;
//xdata float f32_deltaAngleTemp = 0;

xdata short PllKpTemp = 0;
xdata short PllOutTemp = 0;
xdata short PllOutTempFreq = 0;
xdata short PllOutTempSpeed = 0;
xdata short PllCount = 0;

xdata unsigned char BmfBreakFlag = 0;
xdata unsigned short BmfBreakCount = 0;

void MotorCloseLoop_Fun (void)
{
	#if (SPEED_CONTROL == 1)
		#if (BUMPLESS_FUNCTION == 1)
		short Temp1,RealSpeed;
		#endif
	#endif
	if(CloseLoopFlag == 1)
	{
		//CloseLoopFlag = 0;
		PllKpTemp = Pll_Kp_Start;
		PLL_KP_MACRO(Pll_Kp_Start);
		PLL_KI_MACRO(Pll_Ki);
		IQ_KP_MACRO(Iq_Kp);
		IQ_KI_MACRO(Iq_Ki);
		ID_KP_MACRO(Id_Kp);
		ID_KI_MACRO(Id_Ki);
		#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
		FOC_ANG_MACRO((LatestTheta));// 角度補償 10 度 = 10<<6 = 640
		#endif
		#if (CURRENT_CONTROL == 1)
		MOTOR_CONT1 |= FOCANGSEL;// Use SMO_Ang
		//CurrentCmdTemp = IQ_FINAL_START_VALUE;
		IQ_CMD_MACRO(CurrentCmdTemp);
		#endif
		#if (SPEED_CONTROL == 1)
			#if (BUMPLESS_FUNCTION == 1)
			SFR_PAGE = 0; Temp1 = PI_FB;// IQ
			SFR_PAGE = 2; PI_TR = Temp1;// IQ to Speed
			SFR_PAGE = 4; RealSpeed = SMO_D2;// SMO_Speed 
			SFR_PAGE = 3; PI_UI = RealSpeed;// SMO_Speed to SpeedUi
			PI_TMSR |= SPEEDTMSEL;
			PI_TMSR |= SPEEDTMEN;
			#else
			SPEED_UI_MACRO(IQ_FINAL_START_VALUE);
			SPEED_OUT_MACRO(IQ_FINAL_START_VALUE);
			#endif
			FOCCONT |= SPEEDEN;
			MOTOR_CONT1 |= FOCANGSEL;// Use SMO_Ang
			MOTOR_CONT1 |= IQINSEL;// Use SPEED_PI_OUT
			SPEED_KP_MACRO(Spd_Kp_Final);
			SPEED_KI_MACRO(Spd_Ki);
			//SpeedCmdTemp = SPEED_FINAL_VALUE;
			SPEED_CMD_MACRO(SpeedCmdTemp);
		#endif
		FOCCONT |= PLLEN;
		ID_CMD_MACRO(ID_FINAL_START_VALUE);
		MOTOR_CONT2 |= MOTOR_CONT2_REGS;
		MotorState = M_RUN;
	}
}

#if 1
#if (BEMF_TAILWIND_FUNCTION_1 == 1) || (BEMF_TAILWIND_FUNCTION_3 == 1)
void MotorDirCheck_Fun (void)
{
	if(!ZeroPointCheck)
	{
		BmfV = Adc_Cannel(BEMF_V_CH);
		BmfW = Adc_Cannel(BEMF_W_CH);
		BmfVW_Sub = BmfV - BmfW; 
		if(BmfVW_Sub > -10)
		{
			if(++ZeroPointDebounceCnt > 10)
			{
				ZeroPointDebounceCnt = 0;
				ZeroPointFlag = 1;
			}
		}
		else
		{
			if(++ZeroPointDebounceCnt > 10)
			{
				ZeroPointDebounceCnt = 0;
				ZeroPointFlag = 0;
			}
		}
		P3_3 = ZeroPointFlag;
		
		HallUEdgeCnt++;
		if(HallUEdgeCnt >= 20000)// 1Hz = 20000 * 50us 
		{
			HallUEdgeCnt = 20000;
			RecHallUEdgeCnt = HallUEdgeCnt;
		}
		
		if(ZeroPointFlag != ZeroPointFlagOld)
		{
			if(ZeroPointFlag == 1)
			{
				SFR_PAGE = 1; CAPCONT |= 0x10;
				CaptureTotalTemp = CaptureTotal;
				BmfVW_Sum_Pos_ZeroPoint = BmfV + BmfW;
				if(BmfVW_Sum_Pos_ZeroPoint > BmfVW_Sum_Neg_ZeroPoint)
				{
					ZeroPointCnt++;
					if(ZeroPointCnt > 5)
					{
						ZeroPointCnt = 5;
						#if (BEMF_TAILWIND_FUNCTION_1 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								MotorDir = 2;
								#else
								MotorDir = 1;// CW BMF
								#endif
							#endif
						#endif
						#if (BEMF_TAILWIND_FUNCTION_3 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								MotorDir = 1;
								#else
								MotorDir = 2;// CCW Diode BMF
								#endif
							#endif
						#endif
						TailwindTimeOut = 0;
					}
				}
				else if(BmfVW_Sum_Pos_ZeroPoint < BmfVW_Sum_Neg_ZeroPoint)
				{
					ZeroPointCnt--;
					if(ZeroPointCnt < -5)
					{
						ZeroPointCnt = -5;
						#if (BEMF_TAILWIND_FUNCTION_1 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								MotorDir = 1;
								#else
								MotorDir = 2;// CCW BMF
								#endif
							#endif
						#endif
						#if (BEMF_TAILWIND_FUNCTION_3 == 1)
							#if (CW_CCW_FUNCTION == 1) 
								#if (CW_CCW == 1)
								MotorDir = 2;
								#else
								MotorDir = 1;// CW  Diode BMF
								#endif
							#endif
						#endif
						TailwindTimeOut = 0;
					}
				}
			}
			else
			{
				SFR_PAGE = 1; CAPCONT &= ~(0x10);
				RecHallUEdgeCnt = HallUEdgeCnt;
				HallUEdgeCnt = 1;
				BmfVW_Sum_Neg_ZeroPoint = BmfV + BmfW;
				if((MotorDir == 1) || (MotorDir == 2))
				{
					if(MotorDir == 1)
					{
						#if (BEMF_TAILWIND_FUNCTION_3 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								LatestTheta = (short)96<<6;
								#else
								LatestTheta = (short)128<<6;// 64 96 128
								#endif
							#endif
						#endif
						#if (BEMF_TAILWIND_FUNCTION_1 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								LatestTheta = (short)192<<6;
								#else
								LatestTheta = (short)288<<6;
								#endif
							#endif
						#endif
						//LatestTheta = (short)64<<6;
						//LatestTheta = (short)128<<6;
						//LatestTheta = (short)192<<6;
						//LatestTheta = (short)256<<6;
						//LatestTheta = (short)320<<6;
						//LatestTheta = (short)382<<6;
						
						#if 1 // Disable ZeroPointCheck
							if((RecHallUEdgeCnt < 2000)// 10Hz
								|| (RecHallUEdgeCnt > 200)// 100Hz
								)
							{
								#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
									PllOutTemp = (FREQ_VALUE / CaptureTotalTemp);
									PllOutTempSpeed = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									ZeroPointCheck = 1;
								#endif
								#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
									#if (SMO_PLL_CONTROL == 1)
									PllOutTemp = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									ZeroPointCheck = 1;
									#endif
									#if (SMO_ANG_CONTROL == 1)
									PllOutTemp = (FREQ_VALUE / CaptureTotalTemp);
									PllOutTempSpeed = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									ZeroPointCheck = 1;
									#endif
								#endif
							}
						#endif
					}
					else if(MotorDir == 2)
					{
						#if (BEMF_TAILWIND_FUNCTION_3 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								LatestTheta = (short)288<<6;
								#else
								LatestTheta = (short)320<<6;
								#endif
							#endif
						#endif
						#if (BEMF_TAILWIND_FUNCTION_1 == 1)
							#if (CW_CCW_FUNCTION == 1)
								#if (CW_CCW == 1)
								LatestTheta = (short)0<<6;
								#else
								LatestTheta = (short)96<<6;
								#endif
							#endif
						#endif
						//LatestTheta = (short)64<<6;
						//LatestTheta = (short)128<<6;
						//LatestTheta = (short)192<<6;
						//LatestTheta = (short)256<<6;
						//LatestTheta = (short)320<<6;
						//LatestTheta = (short)382<<6;
					
						#if 1 // Disable ZeroPointCheck
							if((RecHallUEdgeCnt > 2000)// 10Hz
								|| (RecHallUEdgeCnt > 200)// 100Hz
								)
							{
								#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
									PllOutTemp = (FREQ_VALUE / CaptureTotalTemp);
									PllOutTemp = -PllOutTemp;
									PllOutTempSpeed = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									PllOutTempSpeed = -PllOutTempSpeed;
									ZeroPointCheck = 1;
								#endif
								#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
									#if (SMO_PLL_CONTROL == 1)
									PllOutTemp = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									PllOutTemp = -PllOutTemp;
									ZeroPointCheck = 1;
									#endif
									#if (SMO_ANG_CONTROL == 1)
									PllOutTemp = (FREQ_VALUE / CaptureTotalTemp);
									PllOutTemp = -PllOutTemp;
									PllOutTempSpeed = ((SPEED_VALUE / CaptureTotalTemp) >> AngleBaseShift);
									PllOutTempSpeed = -PllOutTempSpeed;
									ZeroPointCheck = 1;
									#endif
								#endif
							}
							else if((RecHallUEdgeCnt < 1000))
							{
								BmfBreakCount = 1200;
								MotorState = M_BMF_BREAK;
							}
						#endif
					}
					
					if(ZeroPointCheck)
					{
						PLL_KP_MACRO(0);
						PLL_KI_MACRO(0);
						IQ_KP_MACRO(0);
						IQ_KI_MACRO(0);
						ID_KP_MACRO(0);
						ID_KI_MACRO(0);
						#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
						MOTOR_CONT1 &= ~(FOCANGSEL);// Use CPU_Ang
						#endif
						#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
						MOTOR_CONT1 |= (FOCANGSEL);// Use SMO_Ang
						#endif
						#if (CURRENT_CONTROL == 1)
						//IQ_CMD_MACRO(0);// 耗能煞車
						//ID_CMD_MACRO(0);
						IQ_CMD_MACRO(IQ_FIRST_START_VALUE);
						ID_CMD_MACRO(ID_FIRST_START_VALUE);
						IQ_UI_MACRO(BEMF_TAILWIND_IQ_OUT_VALUE);
						IQ_OUT_MACRO(BEMF_TAILWIND_IQ_OUT_VALUE);
						ID_UI_MACRO(0);
						ID_OUT_MACRO(0);
						#endif
						#if (SPEED_CONTROL == 1)
						//SPEED_UI_MACRO(0);// 耗能煞車
						//SPEED_OUT_MACRO(0);
						SPEED_UI_MACRO(IQ_FIRST_START_VALUE);
						SPEED_OUT_MACRO(IQ_FIRST_START_VALUE);
						IQ_UI_MACRO(BEMF_TAILWIND_IQ_OUT_VALUE);
						IQ_OUT_MACRO(BEMF_TAILWIND_IQ_OUT_VALUE);
						ID_UI_MACRO(0);
						ID_OUT_MACRO(0);
						#endif
						RampDelayCount = 0;
						CloseLoopDelayCount = 0;
						FOCCONT |= PLLEN;
						MOTOR_CONT1 |= (MPWMSEL);// Enable PWM Smo Mode
						MOTOR_CONT2 |= MOTOR_CONT2_REGS;
						PWM_SetActive();
						FOC_ANG_MACRO(LatestTheta);
						CPU_ANG_MACRO(LatestTheta);
						PLL_UI_MACRO(0);
						PLL_OUT_MACRO(0);
					}
				}
			}
		}
		ZeroPointFlagOld = ZeroPointFlag;
	}
}
	
void MotorAngle_Fun (void)
{
	if(!ZeroPointCheck)
	{
		#if 1
		if(MotorDir == 0)
		{
			if(TailwindTimeOut > (TAILWIND_DURATION))
			{
				TailwindTimeOut = 0;
				MotorDir = 0;
				StartUpState = S_IPD;
				MotorState = M_IPD;
			}
			else
				TailwindTimeOut++;
		}
		#endif
	}
	else
	{
		#if 1
			if(MotorDir == 1)
			{
				#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
				LatestTheta += PllOutTemp;
				if(LatestTheta > 24512)
					LatestTheta -= 24512;
				else if (LatestTheta < 0)
					LatestTheta += 24512;
				CPU_ANG_MACRO(LatestTheta>>6);
				
				#if (SOP_LEVEL == 2)
					CloseLoopDelayCount++;
					if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
					{
						PLL_UI_MACRO(PllOutTempSpeed);
						PLL_OUT_MACRO(PllOutTempSpeed);
						CloseLoopDelayCount = 0;
						CloseLoopFlag = 1;
					}
				#endif
				#endif
				#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
				#if (SMO_ANG_CONTROL == 1)
				LatestTheta += PllOutTemp;
				if(LatestTheta > 24512)
					LatestTheta -= 24512;
				else if (LatestTheta < 0)
					LatestTheta += 24512;
				FOC_ANG_MACRO(LatestTheta);
				#endif
				
				#if (SMO_PLL_CONTROL == 1)
				PLL_UI_MACRO(PllOutTemp);
				PLL_OUT_MACRO(PllOutTemp);
				#endif
				
				#if (SOP_LEVEL == 4)
					CloseLoopDelayCount++;
					if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
					{
						#if (SMO_ANG_CONTROL == 1)
						PLL_UI_MACRO(PllOutTempSpeed);
						PLL_OUT_MACRO(PllOutTempSpeed);
						#endif
						CloseLoopDelayCount = 0;
						CloseLoopFlag = 1;
					}
				#endif
				#endif
			}
			else if(MotorDir == 2)
			{
				#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
				RampDelayCount++;
				if(RampDelayCount >= CPU_ANG_RAMP)
				{
					RampDelayCount = 0;
					PllOutTemp = Ramp_Fun(CPU_FREQ_FINAL_TARGET_VALUE_1,PllOutTemp,32700,-32700);
				}
				
				LatestTheta += PllOutTemp;
				if(LatestTheta > 24512)
					LatestTheta -= 24512;
				else if (LatestTheta < 0)
					LatestTheta += 24512;
				CPU_ANG_MACRO(LatestTheta>>6);
				
				#if (SOP_LEVEL == 2)
				if(PllOutTemp >= CPU_FREQ_FINAL_TARGET_VALUE_1)
				{
					PLL_UI_MACRO(CPU_FREQ_FINAL_TARGET_VALUE_2);
					PLL_OUT_MACRO(CPU_FREQ_FINAL_TARGET_VALUE_2);
					CloseLoopFlag = 1;
				}
				#endif
				#endif
				#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
				RampDelayCount++;
				if(RampDelayCount >= SMO_ANG_RAMP)
				{
					RampDelayCount = 0;
					#if (SMO_ANG_CONTROL == 1)
					PllOutTemp = Ramp_Fun(SMO_FREQ_FINAL_TARGET_VALUE_1,PllOutTemp,32700,-32700);
					#endif
					#if (SMO_PLL_CONTROL == 1)
					PllOutTemp = Ramp_Fun(BEMF_TAILWIND_SPEED_VALUE,PllOutTemp,32700,-32700);
					#endif
				}
				
				#if (SMO_ANG_CONTROL == 1)
				LatestTheta += PllOutTemp;
				if(LatestTheta > 24512)
					LatestTheta -= 24512;
				else if (LatestTheta < 0)
					LatestTheta += 24512;
				FOC_ANG_MACRO(LatestTheta);
				#endif
				
				#if (SMO_PLL_CONTROL == 1)
				PLL_UI_MACRO(PllOutTemp);
				PLL_OUT_MACRO(PllOutTemp);
				#endif
				
				#if (SOP_LEVEL == 4)
					#if (SMO_ANG_CONTROL == 1)
					if(PllOutTemp >= SMO_FREQ_FINAL_TARGET_VALUE_1)
					#endif
					#if (SMO_PLL_CONTROL == 1)
					if(PllOutTemp >= BEMF_TAILWIND_SPEED_VALUE)
					#endif
					{
						CloseLoopDelayCount++;
						if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
						{
							#if (SMO_ANG_CONTROL == 1)
							PLL_UI_MACRO(SMO_FREQ_FINAL_TARGET_VALUE_2);
							PLL_OUT_MACRO(SMO_FREQ_FINAL_TARGET_VALUE_2);
							#endif
							CloseLoopDelayCount = 0;
							CloseLoopFlag = 1;
						}
					}
				#endif
				#endif
			}
			MotorCloseLoop_Fun();
		#else
			RampDelayCount++;
			if(RampDelayCount >= SMO_ANG_RAMP)
			{
				RampDelayCount = 0;
				#if (SMO_ANG_CONTROL == 1)
				PllOutTemp = Ramp_Fun(SMO_FREQ_FINAL_TARGET_VALUE_1,PllOutTemp,32700,-32700);
				#endif
				#if (SMO_PLL_CONTROL == 1)
				PllOutTemp = Ramp_Fun(BEMF_TAILWIND_SPEED_VALUE,PllOutTemp,32700,-32700);
				#endif
			}
			
			#if (SMO_ANG_CONTROL == 1)
			LatestTheta += PllOutTemp;
			if(LatestTheta > 24512)
				LatestTheta -= 24512;
			else if (LatestTheta < 0)
				LatestTheta += 24512;
			FOC_ANG_MACRO(LatestTheta);
			#endif
			
			#if (SMO_PLL_CONTROL == 1)
			PLL_UI_MACRO(PllOutTemp);
			PLL_OUT_MACRO(PllOutTemp);
			#endif
			
			#if (SOP_LEVEL == 4)
				#if (SMO_ANG_CONTROL == 1)
				if(PllOutTemp >= SMO_FREQ_FINAL_TARGET_VALUE_1)
				#endif
				#if (SMO_PLL_CONTROL == 1)
				if(PllOutTemp >= BEMF_TAILWIND_SPEED_VALUE)
				#endif
				{
					CloseLoopDelayCount++;
					if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
					{
						#if (SMO_ANG_CONTROL == 1)
						PLL_UI_MACRO(SMO_FREQ_FINAL_TARGET_VALUE_2);
						PLL_OUT_MACRO(SMO_FREQ_FINAL_TARGET_VALUE_2);
						#endif
						CloseLoopDelayCount = 0;
						CloseLoopFlag = 1;
					}
				}
				MotorCloseLoop_Fun();
			#endif
		#endif
	}
}
#endif
#endif
#if (POWER_LIMIT == 1)
void PowerLimit_Fun (void)
{
	short Temp;
	
	if(MotorState == M_RUN)
	{
		Watt = ((unsigned long)V_BUS * (unsigned long)I_BUS) >> 15;
		if(PowerLimitDelayCount > POWER_LIMIT_DELAY_DURATION)
		{
			if(++WattCount > POWER_LIMIT_TIMER)
			{
				WattCount = 0;
				#if (CURRENT_CONTROL == 1)
				Temp = ((Vbus_avg - V_BUS_MIN) * K) + CURRENT_LIMIT_MIN_VALUE;
				if(Watt > Watt_InRef)
				{
					CurrentCmdLimit--;
					if(CurrentCmdLimit < 100)
						CurrentCmdLimit = 100;
				}
				else
				{
					CurrentCmdLimit++;
					if(CurrentCmdLimit > Temp)
						CurrentCmdLimit = Temp;
				}
				#endif
				
				#if (SPEED_CONTROL == 1)
				Temp = ((Vbus_avg - V_BUS_MIN) * K) + CURRENT_LIMIT_MIN_VALUE;
				if(Watt > Watt_InRef)
				{
					CurrentCmdLimit--;
					if(CurrentCmdLimit < 100)
						CurrentCmdLimit = 100;
				}
				else
				{
					CurrentCmdLimit++;
					if(CurrentCmdLimit > Temp)
						CurrentCmdLimit = Temp;
				}
				SPEED_MAX_MACRO(CurrentCmdLimit);
				#endif
			}
		}
		else
			PowerLimitDelayCount++;
	}
}
#endif
#if (POWER_CONTROL == 1)
void PowerControl_Fun (void)
{
	if(MotorState == M_RUN)
	{
		Watt = ((unsigned long)V_BUS * (unsigned long)I_BUS) >> 15;
		if(PowerControlDelayCount > POWER_CONTROL_DELAY_DURATION)
		{
			PIParm_Watt.Cnt++;
			if(PIParm_Watt.Cnt >= POWER_LIMIT_TIMER)//WattFreSet
			{
				PIParm_Watt.Cnt = 0;
				PIParm_Watt.InRef = Watt_InRef;//WattRef;
				PIParm_Watt.InMeas = Watt;//Pe_Watt;
				CalcPI(&PIParm_Watt);
				#if (CURRENT_CONTROL == 1)
				CurrentCmdLimit = ((Vbus_avg - V_BUS_MIN) * K) + CURRENT_LIMIT_MIN_VALUE;
				if(PIParm_Watt.Out > CurrentCmdLimit)
					PIParm_Watt.Out = CurrentCmdLimit;
				CurrentCmd = PIParm_Watt.Out;
				#endif
				#if (SPEED_CONTROL == 1)
				CurrentCmdLimit = ((Vbus_avg - V_BUS_MIN) * K) + CURRENT_LIMIT_MIN_VALUE;
				if(PIParm_Watt.Out > CurrentCmdLimit)
					PIParm_Watt.Out = CurrentCmdLimit;
				SPEED_MAX_MACRO(PIParm_Watt.Out);
				#endif
			}
		}
		else
			PowerControlDelayCount++;
	}
}
#endif
#if (FG_FUNCTION == 1)
void FG_Fun (void)
{
	unsigned short Temp;

	SFR_PAGE = 5;
	Temp = FOC_D2;

	if((MotorState == M_RUN)
		|| (MotorState == M_START)
		|| (MotorState == M_STOP))
	{
		if(Temp > 12256)
			FG = 1;
		else
			FG = 0;
	}
	else
		FG = 0;
}
#endif
#if (FaultLock_Protect == 1)
void FaultLock_Fun (void)
{
	if(MotorState == M_RUN)
	{
		if(MotorFaultDelayCount > FAULTLOCK_DELAY_DURATION)
		{
			if(EstimatedSpeed > RpmErrorMax)
			{
				MotorFaultState = 1;
				MotorErrorState = FaultLock;
			}

			if(EstimatedSpeed < RpmErrorMin)
			{
				MotorFaultState = 2;
				MotorErrorState = FaultLock;
			}

			//if((IqPiOut < IqFbErrorValue) || (IqFb < IqFbErrorValue))
			if((IqPiOut < 0) && (IqFb < 0))
			{
				MotorFaultState = 3;
				MotorErrorState = FaultLock;
			}
			
			if(IqFb >IqPiOut)
			{
				MotorFaultState = 4;
				MotorErrorState = FaultLock;
			}
		}
		else
			MotorFaultDelayCount++;
	}
	else
	{
		MotorFaultDelayCount = 0;
		if(MotorState != M_ERROR)
			MotorFaultState = 0;
	}
}
#endif
#if (VSP_TRI == 1)
void Vsp_Fun (void)
{
	if(Vsp_Avg > 204)
	{
		if(MotorState == M_STOP)
			MotorState = M_RUN;
		else
			SystemState |= 0x04;
		SpeedCmd = Vsp_Avg * 2;
	}
	else
	{
		if(Vsp_Avg < 102)
		{
			MotorState = M_STOP;
		}
	}
}
#endif
void Motor_Control (void)
{
	#if (CW_CCW_FUNCTION == 1)
	if(CCWFlag != CCWFlagOld)// 正反轉控制
		MotorState = M_INIT;
	CCWFlagOld = CCWFlag;
	#endif
	
	if(MotorErrorState)
	{
		PWM_SetAllOff();
		MotorStartRetry_Flow();
	}
	else if((SystemState & 0x08) == 0x08)// 系統延時
	{
		PWM_SetAllOff();
	}
	else if((SystemState & 0x02) == 0x02)// 煞車
	{
		Break_Fun();
		MotorState = M_BREAK;
	}
	else if((SystemState & 0x04) == 0x04)// 啟動
	{
		MotorStart_Flow();
	}
	else
	{
		PWM_SetAllOff();
		MotorStartRetryCount = 0;
		MotorErrorState = Clear;
		MotorState = M_OFF;
	}
}

void MotorStartRetry_Flow (void)
{
	MotorErrorStateOld = MotorErrorState;
	#if (FaultLock_Protect == 1)
	if(
		((MotorErrorState & AOCP) == AOCP)
	//||((MotorErrorState & POCP) == POCP)
	||((MotorErrorState & FaultLock) == FaultLock)
	//||((MotorErrorState & OverVbus) == OverVbus)
	//||((MotorErrorState & UnderVbus) == UnderVbus)
		)
	{
		if(MotorStartDelayCount1 > RESTART_DURATIONT_1)
		{
			MotorStartDelayCount1 = 0;
			if(MotorStartDelayCount2 > RESTART_DURATIONT_2)
			{
				MotorStartDelayCount2 = 0;
				if(MotorStartRetryCount > RETRY_COUNT)
				{
					MotorState = M_ERROR;
				}
				else
				{
					MotorStartRetryCount++;
					if((MotorErrorState & AOCP) == AOCP)
						OCPCONT |= 0x02;// Clear AOCP / DOCP Status
					MotorState = M_INIT;
					MotorErrorState = Clear;
				}
			}
			MotorStartDelayCount2++;
		}
		else
			MotorStartDelayCount1++;
	}
	else
	{
		MotorState = M_INIT;
		//if((MotorErrorState & FaultLock) == FaultLock)
		//	MotorErrorState &= ~(FaultLock);
	}
	#else
	if((MotorErrorState & AOCP) == AOCP)
	{
		if(MotorStartDelayCount1 > RESTART_DURATIONT_1)
		{
			MotorStartDelayCount1 = 0;
			if(MotorStartDelayCount2 > RESTART_DURATIONT_2)
			{
				MotorStartDelayCount2 = 0;
				if(MotorStartRetryCount > RETRY_COUNT)
				{
					MotorState = M_ERROR;
				}
				else
				{
					MotorStartRetryCount++;
					if((MotorErrorState & AOCP) == AOCP)
						OCPCONT |= 0x02;// Clear AOCP / DOCP Status
					MotorState = M_INIT;
					MotorErrorState = Clear;
				}
			}
			MotorStartDelayCount2++;
		}
		else
			MotorStartDelayCount1++;
	}
	#endif
}

void MotorStart_Flow (void)
{
	switch (MotorState)
	{
		case M_INIT:
			PWM_SetAllOff();
			MotorInit_Fun();
			#if (CW_CCW_FUNCTION == 1)
			if(CCWFlag)
				MOTOR_CONT2 |= 0x08;
			else
				MOTOR_CONT2 &= ~(0x08);
			#endif
			break;
		case M_BMF_BREAK:
			if(BmfBreakCount > 1)
			{
				BmfBreakCount--;
				PWM_SetActive();
			}
			else
			{
				MotorState = M_TAILWIND;
				PWM_SetAllOff();
			}
			break;
		case M_TAILWIND:
			#if (BEMF_TAILWIND_FUNCTION_1 == 1) || (BEMF_TAILWIND_FUNCTION_3 == 1)
			MotorAngle_Fun();
			#endif
			#if (BEMF_TAILWIND_FUNCTION_2 == 1)
			BemfTailWindDetect_Fun();
			#endif
			#if (TAILWIND_FUNCTION == 1)
			Break_Fun();
			TailWindDetect_Fun();
			#endif
			#if ((TAILWIND_FUNCTION == 0) && (BEMF_TAILWIND_FUNCTION_1 == 0) && (BEMF_TAILWIND_FUNCTION_2 == 0) && (BEMF_TAILWIND_FUNCTION_3 == 0))
			StartUpState = S_IPD;
			MotorState = M_IPD;
			#endif
			break;
		case M_IPD:
			#if (IPD_FUNCTION == 1)
			IPDDetect_Fun();
			#else 
			StartUpState = S_IPD;
			MotorState = M_START;
			LatestTheta = 0;
			#endif
			break;
		case M_PARKING:
			Parking_Fun();
			break;
		case M_PARKING_START:
			ParkingStart_Fun();
			break;
		case M_START:
			if(StartUpState == S_IPD)
				IPDStart_Fun();
			if(StartUpState == S_TAILWIND)
				TailWindStart_Fun();
			break;
		case M_RUN:
			#if (CURRENT_CONTROL == 1)
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					CurrentCmdTemp = Ramp_Fun(CurrentCmd,CurrentCmdTemp,CurrentCmdLimit,0);
					IQ_CMD_MACRO(CurrentCmdTemp);
				}
				
				if(PllKpTemp != Pll_Kp_Final)
				{
					PllCount++;
					if(PllCount > 1000)
					{
						PllCount = 0;
						if(PllKpTemp > Pll_Kp_Final)
							PllKpTemp--;
						else if(PllKpTemp < Pll_Kp_Final)
							PllKpTemp++;
						PLL_KP_MACRO(PllKpTemp);
					}
				}
			#endif
			#if (SPEED_CONTROL == 1)
				#if (BUMPLESS_FUNCTION == 1)
				if(BumplessCount > BUMPLESS_DURATION)
				{
					PI_TMSR &= ~(SPEEDTMSEL);
					PI_TMSR &= ~(SPEEDTMEN);
				}
				else
					BumplessCount++;
				#endif
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					SpeedCmdTemp = Ramp_Fun(SpeedCmd,SpeedCmdTemp,SpeedCmdLimit,0);
					SPEED_CMD_MACRO(SpeedCmdTemp);
				}
				
				if(PllKpTemp != Pll_Kp_Final)
				{
					PllCount++;
					if(PllCount > 1000)
					{
						PllCount = 0;
						if(PllKpTemp > Pll_Kp_Final)
							PllKpTemp--;
						else if(PllKpTemp < Pll_Kp_Final)
							PllKpTemp++;
						PLL_KP_MACRO(PllKpTemp);
					}
				}
			#endif
			break;
		case M_STOP:
			#if (CURRENT_CONTROL == 1)
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					CurrentCmdTemp = Ramp_Fun(CurrentCmd,CurrentCmdTemp,CurrentCmdLimit,0);
					IQ_CMD_MACRO(CurrentCmdTemp);
				}
				
				if(PllKpTemp != Pll_Kp_Final)
				{
					PllCount++;
					if(PllCount > 1000)
					{
						PllCount = 0;
						if(PllKpTemp > Pll_Kp_Final)
							PllKpTemp--;
						else if(PllKpTemp < Pll_Kp_Final)
							PllKpTemp++;
						PLL_KP_MACRO(PllKpTemp);
					}
				}
			#endif
			#if (SPEED_CONTROL == 1)
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					SpeedCmdTemp = Ramp_Fun(SpeedCmd,SpeedCmdTemp,SpeedCmdLimit,0);
					SPEED_CMD_MACRO(SpeedCmdTemp);
				}
				
				if(PllKpTemp != Pll_Kp_Final)
				{
					PllCount++;
					if(PllCount > 1000)
					{
						PllCount = 0;
						if(PllKpTemp > Pll_Kp_Final)
							PllKpTemp--;
						else if(PllKpTemp < Pll_Kp_Final)
							PllKpTemp++;
						PLL_KP_MACRO(PllKpTemp);
					}
				}
			#endif
			Stop_Fun();
			break;
		case M_REVERSE:
			#if (CURRENT_CONTROL == 1)
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					CurrentCmdTemp = Ramp_Fun(CurrentCmd,CurrentCmdTemp,CurrentCmdLimit,0);
					IQ_CMD_MACRO(CurrentCmdTemp);
				}
			#endif
			#if (SPEED_CONTROL == 1)
				RampDelayCount++;
				if(RampDelayCount >= 10)
				{
					RampDelayCount = 0;
					SpeedCmdTemp = Ramp_Fun(SpeedCmd,SpeedCmdTemp,SpeedCmdLimit,0);
					SPEED_CMD_MACRO(SpeedCmdTemp);
				}
			#endif
			Reverse_Fun();
			break;
		default:
			ResetMOC();
			PWM_SetAllOff();
			MotorState = M_INIT;
			break;
	}
}

void MotorInit_Fun (void)
{
	#if (POWER_CONTROL == 1)
	InitPI(&PIParm_Watt);
	#endif
	ResetMOC();
	Iq_PI_Control_Init();
	Id_PI_Control_Init();
	Speed_PI_Control_Init();
	PLL_PI_Control_Init();

	MOTOR_CONT3 = MOTOR_CONT3_REGS;
	
	GS_MACRO(G);
	SMO_KSLIDE_MACRO(Kslide);
	ANG_BASE_MACRO(AngleBase << AngleBaseShift);
	Z_CORR_MACRO(ZCorr);
	BAN_GAIN_MACRO(BanGain);
	
	FS_MACRO(F);
	SMO_FILTER_MACRO(Kslf);
	ANG_SHIFT_MACRO(0);
	MAXSMC_ERR_MACRO(MaxSmcError);
	
	SVPWM_AMP_MACRO(SVPWM_AMP_REGS); 
	
	// Enable Anti-Windup Mode
	PI_TMSR = PI_TMSR_REGS;
	
	FOCCONT |= (PLLEN);//FOCCONT &= ~(PLLEN);
	MOTOR_CONT1 |= (FOCANGSEL);// Enable SMO_Ang

	FOCCONT &= ~(SPEEDEN);
	#if (CURRENT_CONTROL == 1)
	MOTOR_CONT1 &= ~(IQINSEL);// Disable SPEED_PI_OUT
	#endif
	#if (SPEED_CONTROL == 1)
	MOTOR_CONT1 |= IQINSEL;// Use SPEED_PI_OUT
	#endif
	MOTOR_CONT2 &= ~(0x10);

	SpeedCmd = START_SPEED_VALUE;
	SpeedCmdTemp = SPEED_FINAL_VALUE;//START_SPEED_VALUE;

	CurrentCmd = IQ_START_VALUE;
	CurrentCmdTemp = IQ_FINAL_START_VALUE;//IQ_START_VALUE;
	
	PllKpTemp = Pll_Kp_TailWind;
	
	LatestTheta = 0;
	RampDelayCount = 0;
	AngleStateCehckCount = 0;
	TailwindCount = 0;
	TailwindTimeOut = 0;
	AlignmentCount = 0;
	BreakCount = 0;
	BreakPwmDuty = 0;
	CloseLoopFlag = 0;
	BumplessCount = 0;
	
	StartUpState = S_INIT;
	MotorState = M_TAILWIND;
}
#if (BEMF_TAILWIND_FUNCTION_2 == 1)
void BemfTailWindDetect_Fun (void)
{
	#if 1
	if(TailwindTimeOut > (TAILWIND_DURATION << 1))
	{
		TailwindTimeOut = 0;
		MotorDir = 0;
		StartUpState = S_IPD;
		MotorState = M_IPD;
	}
	else
		TailwindTimeOut++;
	
	if(BemfStateOld == BemfState)
	{
		TailwindCount++;
		if(TailwindCount >= TAILWIND_DURATION)
		{
			TailwindCount = 0;
			TailwindTimeOut = 0;
			MotorDir = 0;
			StartUpState = S_IPD;
			MotorState = M_IPD;
		}
	}
	else
	{
		TailwindCount = 0;
	}
	#endif

	BemfStateOld = BemfState;
	BemfState = (P0_7 * 2) | P0_6;
	if(BemfState != BemfStateOld)
	{
		if((BemfState == 0) && (BemfStateOld == 2))
		{
			MotorDir = 1;// CW
			//MOTOR_CONT2 &= ~(0x08);
		}
		else if((BemfState == 0) && (BemfStateOld == 1))
		{
			MotorDir = 2;// CCW
			//MOTOR_CONT2 |= 0x08;
		}
		
		#if (CW_CCW == 0)
		if((MotorDir == 1) && (BEMF_SPEED > BEMF_SPEED_TAILWIND_1) && (BEMF_SPEED < BEMF_SPEED_TAILWIND_2))
		#endif
		#if (CW_CCW == 1)
		if((MotorDir == 2) && (BEMF_SPEED > BEMF_SPEED_TAILWIND_1) && (BEMF_SPEED < BEMF_SPEED_TAILWIND_2))
		#endif
		{
			if(AngleStateCehckCount > 2)// 判斷 N 次後 執行順風啟動
			{
				AngleStateCehckCount = 0;
				MotorDir = 0;
				CloseLoopFlag = 1;
				StartUpState = S_TAILWIND;
				MotorState = M_START;
			}
			else
				AngleStateCehckCount++;
		}
	}
}
#endif
#if (TAILWIND_FUNCTION == 1)
void TailWindDetect_Fun (void)
{
	/* 靜止 順風 逆風判斷 */
	SFR_PAGE = 4; TailwindSpeed = SMO_D2;
	
	SFR_PAGE = 4; LatestTheta = SMO_D1;
	LatestThetaTemp = LatestTheta;
	LatestTheta -= (191<<6);// 將估測角度 Offest 至 -180 ~ 180 度
	
	#if 1
	if(TailwindTimeOut > (TAILWIND_DURATION << 1))
	{
		TailwindTimeOut = 0;
		MotorDir = 0;
		StartUpState = S_IPD;
		MotorState = M_IPD;
	}
	else
		TailwindTimeOut++;
	
	if(AngleStateOld == AngleState)
	{
		TailwindCount++;
		if(TailwindCount >= TAILWIND_DURATION)
		{
			TailwindCount = 0;
			TailwindTimeOut = 0;
			MotorDir = 0;
			StartUpState = S_IPD;
			MotorState = M_IPD;
		}
	}
	else
	{
		TailwindCount = 0;
	}
	#endif
	AngleStateOld = AngleState;
	
	// 判斷順風狀態
	if(LatestTheta < (-160<<6))// 小於 -170 度時
	{
		AngleState = 1;
	}
	else if((LatestTheta > (-10<<6)) && (LatestTheta < (10<<6)))// 於 -10 ~ 10 度時
	{
		if((AngleState == 1) || (AngleState == 3))
		{
			if(AngleState == 1)
				MotorDir = 1;// 順風 CW
			else if(AngleState == 3)
				MotorDir = 2;// 逆風 CCW
			
			if((MotorDir == 2) && (abs(TailwindSpeed) > SPEED_TAILWIND_VALUE))
			{
				//TailwindCount = 0;// 逆風狀態 重置 TailwindCount = 0;
				if(AngleStateCehckCount < -2)// 判斷 2 次後 執行順風啟動
				{
					AngleStateCehckCount = 0;
					MotorDir = 0;
					CloseLoopFlag = 1;
					StartUpState = S_HEADWIND;
					MotorState = M_HEADWIND_START;
					//FOC_ANG_MACRO(LatestThetaTemp);// 執行順風啟動時 補償角度
				}
				else
					AngleStateCehckCount--;
			}
			
			if((MotorDir == 1) && (abs(TailwindSpeed) > SPEED_TAILWIND_VALUE))
			{
				if(AngleStateCehckCount > 2)// 判斷 2 次後 執行順風啟動
				{
					AngleStateCehckCount = 0;
					MotorDir = 0;
					CloseLoopFlag = 1;
					StartUpState = S_TAILWIND;
					MotorState = M_START;
					//FOC_ANG_MACRO(LatestThetaTemp);// 執行順風啟動時 補償角度
				}
				else
					AngleStateCehckCount++;
			}
		}
		AngleState = 2;
	}
	else if(LatestTheta > (160<<6))// 大於 170 度時
	{
		AngleState = 3;
	}
}
#endif

#if (IPD_FUNCTION == 1)
void IPDDetect_Fun (void)
{
	#define IPDAdvanceAng 0//96
	//WatchDog_Disable(); // ( 註 : 當 IPD 時 會觸發 WatchDog Reset , Disable WatchDog)
	IPD_Init();
	if(IPDPattern == 4) LatestTheta = (64+IPDAdvanceAng)<<6;
	else if(IPDPattern == 5) LatestTheta = (128+IPDAdvanceAng)<<6;
	else if(IPDPattern == 2) LatestTheta = (192+IPDAdvanceAng)<<6;
	else if(IPDPattern == 3) LatestTheta = (256+IPDAdvanceAng)<<6;
	else if(IPDPattern == 6) LatestTheta = (320+IPDAdvanceAng)<<6;
	else if(IPDPattern == 1) LatestTheta = (383+IPDAdvanceAng)<<6;
	else LatestTheta = (383+IPDAdvanceAng)<<6;
	MotorErrorState &= ~(AOCP);
	MotorState = M_START;
	//WatchDog_Init();
}
#endif

short Ramp_Fun (short TargetValue,SetpointValue,HighLimit,LowLimit)
{
	if(TargetValue != SetpointValue)
	{
			if(TargetValue >= SetpointValue)
			{
				SetpointValue++;
			}
			else
			{
				SetpointValue--;
			}
	}
	
	if(SetpointValue > HighLimit)
		SetpointValue = HighLimit;
	else if(SetpointValue < LowLimit)
		SetpointValue = LowLimit;

	return SetpointValue;
}

void Parking_Fun (void)
{
	if(MotorDir == 0)
	{
	
	}
	else if(MotorDir == 1)
	{
	
	}
	else if(MotorDir == 2)
	{
	
	}
}

void ParkingStart_Fun (void)
{
	if(MotorDir == 0)
	{
	
	}
	else if(MotorDir == 1)
	{
	
	}
	else if(MotorDir == 2)
	{
		
	}
	
	MotorCloseLoop_Fun();
}

void IPDStart_Fun (void)
{
	if(AlignmentCount < 1)
	{
		AlignmentCount++;
		PllOutTemp = 0;
		//PllOutTempFreq = 0;
		#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
		PllOutTempSpeed = CPU_FREQ_FINAL_TARGET_VALUE_2;
		#endif
		#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
		PllOutTempSpeed = SMO_FREQ_FINAL_TARGET_VALUE_2;
		#endif
		//PllKpTemp = Pll_Kp_Start;
		PLL_KP_MACRO(0);
		PLL_KI_MACRO(0);
		SPEED_KP_MACRO(0);
		SPEED_KI_MACRO(0);
		IQ_KP_MACRO(Iq_Kp);
		IQ_KI_MACRO(Iq_Ki);
		ID_KP_MACRO(0);
		ID_KI_MACRO(0);
		#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
		MOTOR_CONT1 &= ~(FOCANGSEL);// Use CPU_Ang
		#endif
		#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
		MOTOR_CONT1 |= FOCANGSEL;// Use SMO_Ang
		#endif
		#if (CURRENT_CONTROL == 1)
		IQ_CMD_MACRO(IQ_FIRST_START_VALUE);
		ID_CMD_MACRO(ID_FIRST_START_VALUE);
		#endif
		#if (SPEED_CONTROL == 1)
		SPEED_UI_MACRO(IQ_FIRST_START_VALUE);
		SPEED_OUT_MACRO(IQ_FIRST_START_VALUE);
		#endif
		RampDelayCount = 0;
		CloseLoopDelayCount = 0;
		FOCCONT |= PLLEN;
		#if (IPD_FUNCTION == 1)
		FOC_ANG_MACRO(LatestTheta);
		CPU_ANG_MACRO(LatestTheta>>6);
		#else
		FOC_ANG_MACRO(LatestTheta);
		CPU_ANG_MACRO(LatestTheta>>6);
		#endif
		MOTOR_CONT1 |= (MPWMSEL);// Enable PWM Smo Mode
		PWM_SetActive();
	}
	else if(AlignmentCount >= I_ALIG_DURATION)
	{
		//AlignmentCount = I_ALIG_DURATION;
		#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2)) //CpuAngStartUp_Fun
		RampDelayCount++;
		if(RampDelayCount >= CPU_ANG_RAMP)
		{
			RampDelayCount = 0;
			PllOutTemp = Ramp_Fun(CPU_FREQ_FINAL_TARGET_VALUE_1,PllOutTemp,CPU_FREQ_FINAL_TARGET_VALUE_1,-CPU_FREQ_FINAL_TARGET_VALUE_1);
		}
		
		LatestTheta += PllOutTemp;
		if(LatestTheta > 24512)
			LatestTheta -= 24512;
		else if (LatestTheta < 0)
			LatestTheta += 24512;
		CPU_ANG_MACRO(LatestTheta>>6);
		
		#if (SOP_LEVEL == 2)
		if(PllOutTemp >= CPU_FREQ_FINAL_TARGET_VALUE_1)
		{
			CloseLoopDelayCount++;
			if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
			{
				PLL_UI_MACRO(PllOutTempSpeed);
				PLL_OUT_MACRO(PllOutTempSpeed);
				CloseLoopDelayCount = 0;
				CloseLoopFlag = 1;
			}
		}
		#endif
		#endif
		#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4)) //SmoAngStartUp_Fun
		RampDelayCount++;
		if(RampDelayCount >= SMO_ANG_RAMP)
		{
			RampDelayCount = 0;
			#if (SMO_ANG_CONTROL == 1)
			PllOutTemp = Ramp_Fun(SMO_FREQ_FINAL_TARGET_VALUE_1,PllOutTemp,SMO_FREQ_FINAL_TARGET_VALUE_1,-SMO_FREQ_FINAL_TARGET_VALUE_1);
			#endif
			#if (SMO_PLL_CONTROL == 1)
			PllOutTemp = Ramp_Fun(FREQ_FINAL_START_VALUE,PllOutTemp,FREQ_FINAL_START_VALUE,FREQ_FIRST_START_VALUE);
			#endif
		}
		
		#if (SMO_ANG_CONTROL == 1)
		LatestTheta += PllOutTemp;
		if(LatestTheta > 24512)
			LatestTheta -= 24512;
		else if (LatestTheta < 0)
			LatestTheta += 24512;
		FOC_ANG_MACRO(LatestTheta);
		#endif
		
		#if (SMO_PLL_CONTROL == 1)
		PLL_UI_MACRO(PllOutTemp);
		PLL_OUT_MACRO(PllOutTemp);
		#endif
		
		#if (SOP_LEVEL == 4)
		#if (SMO_ANG_CONTROL == 1)
		if(PllOutTemp >= SMO_FREQ_FINAL_TARGET_VALUE_1)
		#endif
		#if (SMO_PLL_CONTROL == 1)
		if(PllOutTemp >= FREQ_FINAL_START_VALUE)
		#endif
		{
			CloseLoopDelayCount++;
			if(CloseLoopDelayCount > SMO_ANG_DELAY_DURATION)
			{
				#if (SMO_ANG_CONTROL == 1)
				PLL_UI_MACRO(PllOutTempSpeed);
				PLL_OUT_MACRO(PllOutTempSpeed);
				#endif
				#if (SMO_PLL_CONTROL == 1)
				#endif
				CloseLoopDelayCount = 0;
				CloseLoopFlag = 1;
			}
		}
		#endif
		#endif
		MotorCloseLoop_Fun();
	}
	else
	{
		AlignmentCount++;
	}
}

void TailWindStart_Fun (void)
{
	if(CloseLoopFlag == 1)
	{
		//CloseLoopFlag = 0;
		#if (CURRENT_CONTROL == 1)
		MOTOR_CONT1 |= FOCANGSEL;// Use SMO_Ang
		CurrentCmdTemp = IQ_FINAL_TAILWIND_START_VALUE;
		//CurrentCmdTemp = 0;// RampFun 從零開始累加 
		#endif
		#if (SPEED_CONTROL == 1)
		IQ_CMD_MACRO(0);
		SPEED_UI_MACRO(IQ_FINAL_TAILWIND_START_VALUE);
		SPEED_OUT_MACRO(IQ_FINAL_TAILWIND_START_VALUE);
		FOCCONT |= SPEEDEN;
		MOTOR_CONT1 |= FOCANGSEL;// Use SMO_Ang
		MOTOR_CONT1 |= IQINSEL;// Use SPEED_PI_OUT
		SPEED_KP_MACRO(Spd_Kp_Final);// 調整啟動後 SPEED_KP 馬達響應會較好
		SPEED_KI_MACRO(Spd_Ki);
		#endif
		MOTOR_CONT2 |= MOTOR_CONT2_REGS;
		MOTOR_CONT1 |= MPWMSEL;// Enable From SVPWM register
		PWM_SetActive();
		ID_CMD_MACRO(ID_FINAL_START_VALUE);
		IQ_KP_MACRO(Iq_Kp);
		IQ_KI_MACRO(Iq_Ki);
		ID_KP_MACRO(Id_Kp);
		ID_KI_MACRO(Id_Ki);
		MotorState = M_RUN;
	}
}

void Break_Fun (void)
{
	#if (BREAK_FUNCTION == 1)
	MOTOR_CONT1 &= ~(MPWMSEL);// Enable PWM User Mode
	PWM_SetBreak();
	if(BreakPwmDuty < BREAK_DUTY_VALUE) BreakPwmDuty++;
	PWM_Duty(BreakPwmDuty);
	#else
	MOTOR_CONT1 &= ~(MPWMSEL);// Enable PWM User Mode
	PWM_SetBreakForce();
	if(BreakPwmDuty < BREAK_DUTY_VALUE) BreakPwmDuty++;
	PWM_Duty(BreakPwmDuty);
	#endif
}

void Stop_Fun (void)
{
	if(abs(EstimatedSpeed) < STOP_SPEED_VALUE)
	{
		StopCheckCount++;
		if(StopCheckCount > STOP_SPEED_DURATION)
		{
			ResetMOC();
			StopCheckCount = 0;
			SystemState = 0;
			MotorState = M_INIT;
		}
	}
}

void Reverse_Fun (void)
{
	if(abs(EstimatedSpeed) < STOP_SPEED_VALUE)
	{
		StopCheckCount++;
		if(StopCheckCount > STOP_SPEED_DURATION)
		{
			StopCheckCount = 0;
			MOTOR_CONT2 ^= 0x08;// Reverse Flag
			AlignmentCount = 0;
			CloseLoopFlag = 0;
			FOCCONT &= ~(SPEEDEN);// Disable SPEED_FOC
			MOTOR_CONT1 &= ~(IQINSEL);// Disable SPEED_PI_OUT
			StartUpState = S_IPD;
			MotorState = M_START;
			LatestTheta = 0;
		}
	}
}

#if (Vbus_Protect == 1)
void Vbus_Protect_Fun (unsigned short Adc_Vbus)
{
	if(Adc_Vbus > OVER_BUS_VOLT_VALUE)
	{
		if(OverVbusProtectCount >= BUS_VOLT_DURATION)
		{
			OverVbusProtectCount = BUS_VOLT_DURATION; 
			MotorErrorState |= OverVbus;
		}
		else
			OverVbusProtectCount++;
	}
	else
	{
		if(Adc_Vbus < CLEAR_OVER_BUS_VOLT_VALUE)
		{
			if(OverVbusProtectCount < 1)
			{
				OverVbusProtectCount = 0;
				MotorErrorState &= ~(OverVbus);
			}
			else
				OverVbusProtectCount--;
		}
	}

	if(Adc_Vbus < UNDER_BUS_VOLT_VALUE)
	{
		if(UnderVbusProtectCount >= BUS_VOLT_DURATION)
		{
			UnderVbusProtectCount = BUS_VOLT_DURATION; 
			MotorErrorState |= UnderVbus;
		}
		else
			UnderVbusProtectCount++;
	}
	else
	{
		if(Adc_Vbus > CLEAR_UNDER_BUS_VOLT_VALUE)
		{
			if(UnderVbusProtectCount < 1)
			{
				UnderVbusProtectCount = 0;
				MotorErrorState &= ~(UnderVbus);
			}
			else
				UnderVbusProtectCount--;
		}
	}
}
#endif

#if (Temperture_Protect == 1)
void Temperture_Protect_Fun (unsigned short Adc_Temperture)
{
	if(Adc_Temperture > OVER_TEMPERTURE_VALUE)
	{
		if(OverTempertureProtectCount >= TEMPERTURE_DURATION)
		{
			OverTempertureProtectCount = TEMPERTURE_DURATION; 
			MotorErrorState |= OverTemperture;
		}
		else
			OverTempertureProtectCount++;
	}
	else
	{
		if(Adc_Temperture < CLEAR_OVER_TEMPERTURE_VALUE)
		{
			if(OverTempertureProtectCount < 1)
			{
				OverTempertureProtectCount = 0;
				MotorErrorState &= ~(OverTemperture);
			}
			else
				OverTempertureProtectCount--;
		}
	}
}
#endif

void AOCP_Protect_Fun (void)
{
	if(MotorState == M_RUN)
		MotorErrorState |= AOCP;
}

#if (POCP_Protect == 1)
void Phase_OCP_Protect_Fun (void)
{
	int IcFb;
	IaFb = dFB_Ia_DRH << 8;
	IaFb |= dFB_Ia_DRL;
	IbFb = dFB_Ib_DRH << 8;
	IbFb |= dFB_Ib_DRL;
	IcFb = -IaFb - IbFb;
	if((abs(IaFb) > PHASE_OCP_VALUE) || (abs(IbFb) > PHASE_OCP_VALUE) || (abs(IcFb) > PHASE_OCP_VALUE))
	{
		if(POCP_ProtectCount >= PHASE_OCP_DURATION)
		{
			POCP_ProtectCount = PHASE_OCP_DURATION; 
			MotorErrorState |= POCP;
		}
		else
			POCP_ProtectCount++;
	}
	else
	{
		if(POCP_ProtectCount < 1)
		{
			POCP_ProtectCount = 0;
			MotorErrorState &= ~(POCP);
		}
		else
			POCP_ProtectCount--;
	}
}
#endif

void ResetMOC (void)
{
	FOCCONT |= 0x80;// Clear PI & SMO
}

void Iq_PI_Control_Init (void)
{
	SFR_PAGE = 0;
	PI_CMD = 0;
	PI_FB = 0;
	PI_KP = 0;
	PI_KI = 0;
	PI_KT = Iq_Kt;
	PI_TR = 0;
	PI_UI = 0;
	PI_MAX = Iq_MaxLimit;
	PI_MIN = Iq_MinLimit;
	PI_OUT = 0;
}

void Id_PI_Control_Init (void)
{
	SFR_PAGE = 1;
	PI_CMD = 0;
	PI_FB = 0;
	PI_KP = 0;
	PI_KI = 0;
	PI_KT = Id_Kt;
	PI_TR = 0;
	PI_UI = 0;
	PI_MAX = Id_MaxLimit;
	PI_MIN = Id_MinLimit;
	PI_OUT = 0;
}

void Speed_PI_Control_Init (void)
{
	SP_CYC = Spd_Cyc;// 設定 執行 xx 次 PWM 週期 計算 CloseLoop 
	SFR_PAGE = 2;
	PI_CMD = 0;
	PI_FB = 0;
	PI_KP = 0;
	PI_KI = 0;
	PI_KT = Spd_Kt;
	PI_TR = 0;
	PI_UI = 0;
	PI_MAX = Spd_MaxLimit;
	PI_MIN = Spd_MinLimit;
	PI_OUT = 0;
}

void PLL_PI_Control_Init (void)
{
	SFR_PAGE = 3;
	PI_CMD = 0;
	PI_FB = 0;
	#if (BEMF_TAILWIND_FUNCTION_1 == 1) || (BEMF_TAILWIND_FUNCTION_2 == 1) || (BEMF_TAILWIND_FUNCTION_3 == 1)
	PI_KP = 0;
	PI_KI = 0;
	#else
	PI_KP = Pll_Kp_TailWind;
	PI_KI = Pll_Ki;
	#endif
	PI_KT = Pll_Kt;
	PI_TR = 0;
	PI_UI = 0;
	PI_MAX = Pll_MaxLimit;
	PI_MIN = Pll_MinLimit;
	PI_OUT = 0;
}
#if 0
void User_PI_Control_Init (void)
{
	SFR_PAGE = 4;
	PI_CMD = 0;
	PI_FB = 0;
	PI_KP = 32700;
	PI_KI = 200;
	PI_KT = 1;
	//PI_TR = 0;
	PI_UI = 0;
	PI_MAX = 32000;
	PI_MIN = -32000;
	PI_OUT = 0;
}

void VdcCorrection (void)
{
	unsigned short Temp;
	SFR_PAGE = 1;
	Temp = (ADCAUTO_H<<8) + ADCAUTO_L;
	SFR_PAGE = 0;
	FOC_D1 = Temp;
}

void VdcCorrection_Init (void)
{
#define COREN 0x01
#define Type 0x10 // Vq Vd Voltage
#define TargetSEL 0x20 // TargetSEL : 8191
	
	VDCCONT = TargetSEL | Type | COREN;
	SFR_PAGE = 1;
	FOC_D1 = 13;// 8191 / 512 = 16
}
#endif

void MOC_Init (void)
{
	MOTOR_CONT1 = MOTOR_CONT1_REGS;
	MOTOR_CONT2 &= ~(0x10);
	MOTOR_CONT3 = MOTOR_CONT3_REGS;
	
	// Motor PWM Compensate Factor Register
	MPWMCPSF = MPWMCPSF_REGS;
	
	FG_CTRL = 0x00 | 0x00;// FG Enable, 1 Plus
	
	FOCCONT = PICLEAR | ESTCR | INVADCD | ADCTRIG;// PI Clear
	
	PI_GAIN = PI_GAIN_REGS;
	PI_TMSR = PI_TMSR_REGS;
}

void Read_Iq (void)
{
	SFR_PAGE = 0; IqFb = PI_FB;
	//IqFb = (IqFb + IqFbOld + IqFbOld + IqFbOld)>>2;
	//IqFbOld = IqFb;
}

void Read_Vq (void)
{
	SFR_PAGE = 0; IqPiOut = PI_OUT;
	//IqPiOut = (IqPiOut + IqPiOutOld + IqPiOutOld + IqPiOutOld)>>2;
	//IqPiOutOld = IqPiOut;
}

void Read_Speed (void)
{
	SFR_PAGE = 4; EstimatedSpeed = SMO_D2;
	//EstimatedSpeed = (EstimatedSpeed + EstimatedSpeedOld + EstimatedSpeedOld + EstimatedSpeedOld)>>2;
	EstimatedSpeedOld = EstimatedSpeed;
}
