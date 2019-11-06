#ifndef MOTOR_H
#define MOTOR_H

enum ErrorStatus{Clear = 0x00, OverVbus = 0x01, UnderVbus = 0x02, OverTemperture = 0x04, FaultLock = 0x08, AOCP = 0x10, POCP = 0x20};
enum StartUpStatus{S_INIT = 0, S_IPD = 1, S_TAILWIND = 2,S_HEADWIND = 3};
enum MotorStatus
{
	M_OFF = 0, M_INIT = 1, M_TAILWIND = 2, M_IPD = 3,M_START = 4,
	M_RUN = 5, M_STOP = 6, M_BREAK = 7, M_ERROR = 8,M_REVERSE = 9,
	M_HEADWIND_START = 10, M_MOTOR_DIR = 11, M_BMF_BREAK = 12 , M_PARKING = 13, M_PARKING_START = 14
};

// <<< Use Configuration Wizard in Context Menu >>>
// <h> �]�w���F�Ѽ�
// <o> ���F�Ѽ� Pole <0-30:2>
#define POLE 8
#define PLOE_PAIRS POLE>>1

// <o> ���F�Ѽ� Rs <0-10000>
// <i> unit : m-Ohm
#define Rs (float)5600

// <o> ���F�Ѽ� Lq <0-10000>
// <i> unit : mH
#define Lq (float)847 

// <o> ���F�Ѽ� Ld <0-10000>
// <i> unit : mH
#define Ld (float)776 

// <o> ���F�Ѽ� Ls <0-10000>
// <i> unit : m-Ohm
// <i> ���� : Ls = (Ld + Lq) / 2
#define Ls (float)811

// <o> ���FSMO�Ѽ� G <0-32767>
// <i> ���� : G = Ts / Ls/2
#define G 16000

// <o> ���FSMO�Ѽ� F <0-32767>
// <i> ���� : F = 1 - (G * Rs/2)
#define F 18000

// <o> ���FSMO�Ѽ� Kslide <0-32767>
// <i> ���� : Bang-Bang Control Gain : Kslide
#define Kslide 31000

// <o> ���FSMO�Ѽ� Kslf <0-32767>
// <i> ���� : Low Pass Filter Gain : Kslf
#define Kslf 4000

// <o> ���FSMO�Ѽ� Z-Corr <0-32767>
// <i> ���� : Low Pass Filter Gain : Kslf
#define ZCorr 32767

// <o> ���FSMO�Ѽ� BanGain <0-32767>
// <i> ���� : Low Pass Filter Gain : Kslf
#define BanGain 32767

// <o> ���FSMO�Ѽ� MaxSmcError <0-32767>
// <i> ���� : Low Pass Filter Gain : Kslf
#define MaxSmcError 32767
// </h>

// <h> �]�w Shunt �P OPA ��j���v�Ѽ�
// <o> Shunt �Ѽ�
// <i> unit : m-Ohm
#define R_SHUNT (float)100/1000 

// <o.4..5> OPA GAIN �Ѽ� <0=> 1 Gain <1=> 2.5 Gain <2=> 5 Gain <3=> 10 Gain
// <i> �`�N : �ݻP ADCSTR ��j���v�ۦP
#define OPA_GAIN_REGS 0x20
#if (OPA_GAIN_REGS == 0x00)
	#define OPA_GAIN (float)10/10
#endif
#if (OPA_GAIN_REGS == 0x10)
	#define OPA_GAIN (float)25/10 
#endif
#if (OPA_GAIN_REGS == 0x20)
	#define OPA_GAIN (float)50/10 
#endif
#if (OPA_GAIN_REGS == 0x30)
	#define OPA_GAIN (float)100/10 
#endif
#define I_AMPLIFIER (unsigned short)((float)(((R_SHUNT * OPA_GAIN * 1024))/5)*64) 
// </h>

// <e> �]�w���F�վ��y�{
// <o> SOP_LEVEL <0=> 0 <1=> 1 <2=> 2 <3=> 3 <4=> 4
// <i> LEVEL_0 : �q�y Opa �ե� ( �� : �վ� Ia Ib Offset �Ѽ� )
// <i> LEVEL_1 : CpuAng OpenLoop �� CpuAng �p�⨤�ױҰ� ( �� : �վ� G F Kslide Kslf �Ѽ� �B��� �ݤ�ʤ��� CloseLoopFlag )
// <i> LEVEL_2 : CpuAng CloseLoop �� CppuAng �p�⨤�ױҰ� ( �� : �B��� �۰ʤ��� CloseLoopFlag )
// <i> LEVEL_3 : SmoAng OpenLoop �� SmoAng PLL �p�⨤�ױҰ� ( �� : �B��� ��ʤ��� CloseLoopFlag )
// <i> LEVEL_4 : SmoAng CloseLoop �� SmoAng PLL �p�⨤�ױҰ� ( �� : �B��� �۰ʤ��� CloseLoopFlag )
#define SOP_LEVEL 4

#if ((SOP_LEVEL == 1) || (SOP_LEVEL == 2))
// <o> �]�w CPU_ANG_RAMP �[�t�ײv <0-10000>
// <i> unit : Val ( �ȶV�j�[�t�V�� )
#define CPU_ANG_RAMP 10 

// <o> �]�w CPU_ANG �W�v <0-30000>
// <i> unit : Hz ( �� : 1 = 0.04165 Hz �� Va Ia �W�v )
// <i> ���� : StepAngleMax = 24512 * ���_�g�� ( PWM�BTimer )
// <i> ex. StepAngleMax = 24512 * 1ms = 24.512
// <i> ex. StepAngleMax = 24512 * 50us = 1.2256
#define CPU_FREQ_FINAL_TARGET 9 // unit : Hz
#define CPU_FREQ_FINAL_TARGET_VALUE_1 (unsigned short)((float) CPU_FREQ_FINAL_TARGET * 24.512) // unit : Hz
#define CPU_FREQ_FINAL_TARGET_VALUE_2 (unsigned short)((long)((((120 / POLE) * CPU_FREQ_FINAL_TARGET) * PLOE_PAIRS) >> AngleBaseShift))
#endif

#if ((SOP_LEVEL == 3) || (SOP_LEVEL == 4))
// <o> �]�w SMO_ANG_RAMP �[�t�ײv <0-10000>
// <i> unit : Val ( �ȶV�j�[�t�V�� )
#define SMO_ANG_RAMP 10 

// <e> SMO_ANG �Ұ� Enable/Disable
// <o2> �]�w SMO_ANG �W�v <0-30000>
// <i> unit : Hz ( �� : 1 = 0.04165 Hz �� Va Ia �W�v )
// <i> ���� : StepAngleMax = 24512 * ���_�g�� ( PWM�BTimer )
// <i> ex. StepAngleMax = 24512 * 1ms = 24.512
// <i> ex. StepAngleMax = 24512 * 50us = 1.2256
// </e>
#define SMO_ANG_CONTROL 1
#if (SMO_ANG_CONTROL == 1)
#define SMO_FREQ_FINAL_TARGET_1 9 // unit : Hz
#define SMO_FREQ_FINAL_TARGET_VALUE_1 (short)((float) SMO_FREQ_FINAL_TARGET_1 * 24.512) // unit : Hz
#define SMO_FREQ_FINAL_TARGET_VALUE_2 (short)((long)((((120 / POLE) * SMO_FREQ_FINAL_TARGET_1) * PLOE_PAIRS) >> AngleBaseShift))
#endif
// <e> SMO_PLL �Ұ� Enable/Disable
// <o2> �]�w SMO_PLL ��l��t
// <i> unit : Val 
// <o3> �]�w SMO_PLL ������t
// <i> unit : Val 
// </e>
#define SMO_PLL_CONTROL 0
#if (SMO_PLL_CONTROL == 1)
#define FREQ_FIRST_START_VALUE (short)(((long) 1 * PLOE_PAIRS) >> AngleBaseShift) // unit : Rpm
#define FREQ_FINAL_START_VALUE (short)(((long) 150 * PLOE_PAIRS) >> AngleBaseShift) // unit : Rpm
#endif
#endif

// <o> �]�w SMO_ANG_DELAY ���ɮɶ� <0-65535>
// <i> unit : Val 
#define SMO_ANG_DELAY_DURATION 10
// </e>

//#if (SMO_ANG_CONTROL == 1) && (SMO_PLL_CONTROL == 1)
//#error Wrong setting SMO_ANG_CONTROL and SMO_PLL_CONTROL !!!!
//#endif

// <h> �]�w IQ �q�y�Ѽ�
// <o> �]�w IQ �Ұʹq�y 
// <i> unit : mA
#define IQ_FIRST_START (float)500/1000 
#define IQ_FIRST_START_VALUE (signed short)((float) IQ_FIRST_START * I_AMPLIFIER) // unit : A * Gain

// <o> �]�w IQ �����q�y 
// <i> unit : mA
#define IQ_FINAL_START (float)500/1000
#define IQ_FINAL_START_VALUE (signed short)((float) IQ_FINAL_START * I_AMPLIFIER) // unit : A * Gain

// <o> �]�w IQ ���f���Ұʹq�y 
// <i> unit : mA
#define IQ_FINAL_TAILWIND_START (float)0/1000 
#define IQ_FINAL_TAILWIND_START_VALUE (signed short)((float) IQ_FINAL_TAILWIND_START * I_AMPLIFIER) // unit : A * Gain
// </h>

// <h> �]�w ID �q�y�Ѽ�
// <o> �]�w ID �Ұʹq�y 
// <i> unit : mA
#define ID_FIRST_START (float)0/1000 
#define ID_FIRST_START_VALUE (signed short)((float) -ID_FIRST_START * I_AMPLIFIER) // unit : A * Gain

// <o> �]�w ID �����q�y 
// <i> unit : mA
#define ID_FINAL_START (float)0/1000 
#define ID_FINAL_START_VALUE (signed short)((float) -ID_FINAL_START * I_AMPLIFIER) // unit : A * Gain

// <o> �]�w ID ���f���Ұʹq�y 
// <i> unit : mA
#define ID_FINAL_TAILWIND_START (float)0/1000
#define ID_FINAL_TAILWIND_START_VALUE (signed short)((float) -ID_FINAL_TAILWIND_START * I_AMPLIFIER) // unit : A * Gain
// </h>

// <h> �]�w SPEED �Ѽ�
// <o> �]�w SPEED �Ұ�
// <i> unit : RPM
#define SPEED_START_VALUE (short)(((long) 100 * PLOE_PAIRS) >> AngleBaseShift)


// <o> �]�w SPEED ����
// <i> unit : RPM
#define SPEED_FINAL_VALUE (short)(((long) 100 * PLOE_PAIRS) >> AngleBaseShift)
// </h>

// <h> �]�w IQ PI �Ѽ�
// <o> Kp �Ѽ� <0-32767>
// <o1> Ki �Ѽ� <0-32767>
// <o2> Kt �Ѽ� <0-32767>
// <o3> MaxLimit �Ѽ� <0-32767>
// <o4> MinLimit �Ѽ� <0-32767>
#define Iq_Kp 16000
#define Iq_Ki 10
#define Iq_Kt 31000
#define Iq_MaxLimit 32700
#define Iq_MinLimit -32700
// </h>

// <h> �]�w ID PI �Ѽ�
// <o> Kp �Ѽ� <0-32767>
// <o1> Ki �Ѽ� <0-32767>
// <o2> Kt �Ѽ� <0-32767>
// <o3> MaxLimit �Ѽ� <0-32767>
// <o4> MinLimit �Ѽ� <0-32767>
#define Id_Kp 16000
#define Id_Ki 10
#define Id_Kt 31000
#define Id_MaxLimit 32000
#define Id_MinLimit -32000
// </h>

// <h> �]�w SPEED PI �Ѽ�
// <o> Start Kp �Ѽ� <0-32767>
// <o1> Final Kp �Ѽ� <0-32767>
// <o2> Ki �Ѽ� <0-32767>
// <o3> Kt �Ѽ� <0-32767>
// <o4> MaxLimit �Ѽ� <0-32767>
// <o5> MinLimit �Ѽ� <0-32767>
// <o6> Speed Cycle �Ѽ� <0-255>
#define Spd_Kp_Start 0
#define Spd_Kp_Final 200
#define Spd_Ki 10
#define Spd_Kt 31000
#define Spd_MaxLimit 32000
#define Spd_MinLimit -32000
#define Spd_Cyc 10
// </h>

// <h> �]�w PLL PI �Ѽ�
// <o> TailWind Kp �Ѽ� <0-32767>
// <o1> HeadWind Kp �Ѽ� <0-32767>
// <o2> Start Kp �Ѽ� <0-32767>
// <o3> Final Kp �Ѽ� <0-32767>
// <o4> Ki �Ѽ� <0-32767>
// <o5> Kt �Ѽ� <0-32767>
// <o6> MaxLimit �Ѽ� <0-32767>
// <o7> MinLimit �Ѽ� <0-32767>
#define Pll_Kp_TailWind 16000
#define Pll_Kp_HeadWind 16000
#define Pll_Kp_Start 8000
#define Pll_Kp_Final 8000
#define Pll_Ki 10
#define Pll_Kt 0
#define Pll_MaxLimit 32000
#define Pll_MinLimit -32000
// </h>

// <h> �]�w���F����{��
// <e> VR ����R�O Enable/Disable
// </e>
#define VSP_TRI 0

// <e> UART ����R�O Enable/Disable
// </e>
#define UART_TRI 0

#if (VSP_TRI == 1) && (UART_TRI == 1)
#error Wrong setting VR_TRI and UART_TRI !!!!
#endif
// <e> �w�q�y���� Enable/Disable
// </e>
#define CURRENT_CONTROL 1
#if (CURRENT_CONTROL == 1)
#endif
// <o> �]�w�q�y����d�� Max
// <i> unit : mA
// <o2> �]�w�q�y����d�� Min
// <i> unit : mA
#define IQ_MAX_LIMIT (float)5000/1000
#define IQ_MAX_LIMIT_VALUE (signed short)((float)(IQ_MAX_LIMIT * I_AMPLIFIER)) // unit : A * Gain
#define IQ_MIN_LIMIT (float)0/1000 
#define IQ_MIN_LIMIT_VALUE (signed short)((float)(-IQ_MIN_LIMIT * I_AMPLIFIER)) // unit : A * Gain

// <e> �w��t���� Enable/Disable
// </e>
#define SPEED_CONTROL 0
#if (SPEED_CONTROL == 1)
#endif
// <o> �]�w��t����d�� Max
// <i> unit : rpm
// <o1> �]�w��t����d�� Min
// <i> unit : rpm
#define SPEED_MAX_LIMIT_VALUE (short)(((long) 1300 * PLOE_PAIRS) >> AngleBaseShift)
#define SPEED_MIN_LIMIT_VALUE (short)(((long) 0 * PLOE_PAIRS) >> AngleBaseShift)

#if (CURRENT_CONTROL == 1) && (SPEED_CONTROL == 1)
#error Wrong setting CURRENT_CONTROL and SPEED_CONTROL !!!!
#endif
// <e> �\�v���� (Power Limit) Enable/Disable
// <o2> �q�y���� MAX <0-32767>
// <i> unit : mA
// <o4> �q�y���� MIN <0-32767>
// <i> unit : mA
// <o6> ��J Vbus �d�� MAX <0-1023>
// <i> �� : ��g��J Vbus ������AŪ�� AD ��
// <o7> ��J Vbus �d�� MIN <0-1023>
// <i> �� : ��g��J Vbus ������AŪ�� AD ��
// <o8> ��J Pwoer Limit
// <i> �� : ��g��J Vbus_avg�BIbus_avg �ۭ���AWATT ��
// <o9> ��J Pwoer Limit ����g��
// <i> unit : ms
// <o10> �]�w Pwoer Limit ���ɮɶ�
// <i> unit : ms
// </e>
#define POWER_LIMIT 0
#if (POWER_LIMIT == 1)
#define CURRENT_LIMIT_MAX (float)5189/1000 
#define CURRENT_LIMIT_MAX_VALUE (signed short)((float)(CURRENT_LIMIT_MAX * I_AMPLIFIER)) // unit : A * Gain
#define CURRENT_LIMIT_MIN (float)4273/1000 
#define CURRENT_LIMIT_MIN_VALUE (signed short)((float)(CURRENT_LIMIT_MIN * I_AMPLIFIER)) // unit : A * Gain
#define V_BUS_MAX 680
#define V_BUS_MIN 580
#define K (short)((CURRENT_LIMIT_MAX_VALUE - CURRENT_LIMIT_MIN_VALUE)/(V_BUS_MAX - V_BUS_MIN))
#define WATT_LIMIT_VALUE (unsigned long) 32500
#define POWER_LIMIT_TIMER 2
#define POWER_LIMIT_DELAY_DURATION 1000
#endif

// <e> �\�v���� (Power Control) Enable/Disable
// <o2> �q�y���� MAX <0-32767>
// <i> unit : mA
// <o4> �q�y���� MIN <0-32767>
// <i> unit : mA
// <o6> ��J Vbus �d�� MAX <0-1023>
// <i> �� : ��g��J Vbus ������AŪ�� AD ��
// <o7> ��J Vbus �d�� MIN <0-1023>
// <i> �� : ��g��J Vbus ������AŪ�� AD ��
// <o8> ��J Pwoer Control <0-32767>
// <i> �� : ��g��J Vbus_avg�BIbus_avg �ۭ���AWATT �� (Q15 �榡)
// <o9> ��J Pwoer Control ����g��
// <i> unit : ms
// <o10> �]�w Pwoer Control ���ɮɶ�
// <i> unit : ms
// </e>
#define POWER_CONTROL 0
#if (POWER_CONTROL == 1)
#define CURRENT_LIMIT_MAX (float)5189/1000 
#define CURRENT_LIMIT_MAX_VALUE (signed short)((float)(CURRENT_LIMIT_MAX * I_AMPLIFIER)) // unit : A * Gain
#define CURRENT_LIMIT_MIN (float)4273/1000 
#define CURRENT_LIMIT_MIN_VALUE (signed short)((float)(CURRENT_LIMIT_MIN * I_AMPLIFIER)) // unit : A * Gain
#define V_BUS_MAX 680
#define V_BUS_MIN 580
#define K (short)((CURRENT_LIMIT_MAX_VALUE - CURRENT_LIMIT_MIN_VALUE)/(V_BUS_MAX - V_BUS_MIN))
#define WATT_LIMIT_VALUE (unsigned long) 32000
#define POWER_LIMIT_TIMER 100
#define POWER_CONTROL_DELAY_DURATION 1000
#endif

#if (POWER_LIMIT == 1) && (POWER_CONTROL == 1)
#error Wrong setting POWER_LIMIT and POWER_CONTROL !!!!
#endif
// <e> �����౱�� Enable/Disable
// </e>
#define CW_CCW_FUNCTION 1
// <o> �]�w CW/CCW ��V <0=> CW <1=>CCW
// <i> unit : rpm
#define CW_CCW 1

// <e> ��l��m���� Enable/Disable
// </e>
#define IPD_FUNCTION 0

// <e> ���f���P�_ Enable/Disable
// <o2> �]�w������t <0-1000>
// <i> ���� : �]�w�������A�U�s������t
// <i> unit : rpm
// </e>
#define TAILWIND_FUNCTION 0
#if (TAILWIND_FUNCTION == 1)
#define SPEED_TAILWIND_VALUE (short)(((long) 50 * PLOE_PAIRS) >> AngleBaseShift)
#endif

// <e> BEMF ���f���P�_ (Diode) Enable/Disable
// <o2> �]�w���f���q�y <0-60000>
// <o3> �]�w���f����t <0-60000>
// <o4> �]�w BEMF_V_CH <0=> CH0 <1=> CH1 <2=> CH2 <3=> CH3 <4=> CH4 <5=> CH5 <6=> CH6 <7=> CH7 
// <o5> �]�w BEMF_W_CH <0=> CH0 <1=> CH1 <2=> CH2 <3=> CH3 <4=> CH4 <5=> CH5 <6=> CH6 <7=> CH7 
// </e>
#define BEMF_TAILWIND_FUNCTION_3 1
#if (BEMF_TAILWIND_FUNCTION_3 == 1)
#define BEMF_TAILWIND_IQ_OUT_VALUE (short)(2000)
#define BEMF_TAILWIND_SPEED_VALUE (short)(((long) 150 * PLOE_PAIRS) >> AngleBaseShift)
#define BEMF_V_CH 5
#define BEMF_W_CH 6
#endif

// <e> BEMF ���f���P�_ (�����q��) Enable/Disable
// <o2> �]�w���f���q�y <0-60000>
// <o3> �]�w���f����t <0-60000>
// <o4> �]�w BEMF_V_CH <0=> CH0 <1=> CH1 <2=> CH2 <3=> CH3 <4=> CH4 <5=> CH5 <6=> CH6 <7=> CH7 
// <o5> �]�w BEMF_W_CH <0=> CH0 <1=> CH1 <2=> CH2 <3=> CH3 <4=> CH4 <5=> CH5 <6=> CH6 <7=> CH7 
// </e>
#define BEMF_TAILWIND_FUNCTION_1 0
#if (BEMF_TAILWIND_FUNCTION_1 == 1)
#define BEMF_TAILWIND_IQ_OUT_VALUE (short)(8000)
#define BEMF_TAILWIND_SPEED_VALUE (short)(((long) 150 * PLOE_PAIRS) >> AngleBaseShift)
#define BEMF_V_CH 5
#define BEMF_W_CH 6
#endif

// <e> BEMF ���f���P�_ (�����) Enable/Disable
// <o2> �]�w������t 1 <0-20000>
// <i> ���� : �]�w�������A�U�s������t
// <i> unit : rpm
// <o3> �]�w������t 2 <0-20000>
// <i> ���� : �]�w�������A�U�s������t
// <i> unit : rpm
// </e>
#define BEMF_TAILWIND_FUNCTION_2 0
#if (BEMF_TAILWIND_FUNCTION_2 == 1)
#define BEMF_SPEED_TAILWIND_1 50
#define BEMF_SPEED_TAILWIND_2 100
#endif

//#if (TAILWIND_FUNCTION == 1) && (BEMF_TAILWIND_FUNCTION == 1)
//#error Wrong setting TAILWIND_FUNCTION and BEMF_TAILWIND_FUNCTION !!!!
//#endif

// <e> �٨����� Enable/Disable
// <o2> �]�w�٨��O�D <0-100>
// <i> ���� : �]�w�������A�U�٨��O�D
// <i> unit : %
// </e>
#define BREAK_FUNCTION 1
#if (BREAK_FUNCTION == 1)
#define BREAK_DUTY (float)10/100
#define BREAK_DUTY_VALUE (unsigned short)((float)(MPWMDATA_REGS * BREAK_DUTY))
#else
#define BREAK_DUTY (float)99/100
#define BREAK_DUTY_VALUE (unsigned short)((float)(MPWMDATA_REGS * BREAK_DUTY))
#endif

// <e> FG �H�� Enable/Disable
// </e>
#define FG_FUNCTION 0

// <e> Bumpless ���� Enable/Disable
// <o2> BUMPLESS_DURATION �Ѽ� <0-65535>
// </e>
#define BUMPLESS_FUNCTION 0
#if (BUMPLESS_FUNCTION == 1)
#define BUMPLESS_DURATION 1000
#endif
// </h>

// <h> �]�w���F�O�@�\��
// <e> �L�q���O�@ Enable/Disable
// <i> ���� : �ݨϥ�Vbus�����q��
// <i> (OVER) (CLEAR_OVER)                         (CLEAR_UNDER) (UNDER)
// <i> ----|-----------|------------------------------|-------------|---->
// <o2> OVER_BUS_VOLT_VALUE �Ѽ�
// <o3> CLEAR_OVER_BUS_VOLT_VALUE �Ѽ�
// <o4> CLEAR_UNDER_BUS_VOLT_VALUE �Ѽ�
// <o5> UNDER_BUS_VOLT_VALUE �Ѽ�
// <o6> BUS_VOLT_DURATION �Ѽ�
// </e>
#define Vbus_Protect 0
#if (Vbus_Protect == 1)
#define OVER_BUS_VOLT_VALUE 720 // unit : V * Gain
#define CLEAR_OVER_BUS_VOLT_VALUE 710 // unit : V * Gain
#define CLEAR_UNDER_BUS_VOLT_VALUE 700 // unit : V * Gain
#define UNDER_BUS_VOLT_VALUE 690 // unit : V * Gain
#define BUS_VOLT_DURATION 50 // unit : mSec
#endif
// <e> �L�ū׫O�@ Enable/Disable
// <i> ���� : �ݨϥηū׷P����
// <i> (OVER) (CLEAR_OVER)                         (CLEAR_UNDER) (UNDER)
// <i> ----|-----------|------------------------------|-------------|---->
// <o2> OVER_TEMPERTURE �Ѽ�
// <o3> CLEAR_OVER_TEMPERTURE �Ѽ�
// <o4> TEMPERTURE_DURATION �Ѽ�
// </e>
#define Temperture_Protect 0
#if (Temperture_Protect == 1)
#define OVER_TEMPERTURE_VALUE 900 // unit : DegC * Gain
#define CLEAR_OVER_TEMPERTURE_VALUE 800 // unit : DegC * Gain
#define TEMPERTURE_DURATION 50 // unit : mSec
#endif
// <e> ����O�@ Enable/Disable
// <i> ���� : Speed Iq Id �^�²��`����
// <o2> ��t�L�����`
// <i> unit : rpm
// <o3> ��t�L�C���`
// <i> unit : rpm
// <o4> IQ �q�y���`
// <i> unit : mA
// <o6> ����O�@�P�_���ɮɶ�
// <i> unit : ms
// </e>
#define FaultLock_Protect 0
#if (FaultLock_Protect == 1)
#define RpmErrorMax (short)(((long) 1500 * PLOE_PAIRS) >> AngleBaseShift)
#define RpmErrorMin (short)(((long) 0 * PLOE_PAIRS) >> AngleBaseShift)
#define IqFbError (float)0/1000 
#define IqFbErrorValue (signed short)((float)(IqFbError * I_AMPLIFIER)) // unit : A * Gain
#endif
#define FAULTLOCK_DELAY_DURATION 1000

// <e> �۹q�y�O�@ Enable/Disable
// <i> ���� : Ia�BIb�BIc
// <o2> Phase Ia�BIb�BIc �Ѽ�
// <i> unit : mA
// <o4> PHASE_OCP_DURATION �Ѽ�
#define POCP_Protect 0
#if (POCP_Protect == 1)
#define PHASE_OCP (float)700/1000 
#define PHASE_OCP_VALUE (signed short)((float)(PHASE_OCP * I_AMPLIFIER)) // unit : A * Gain
#define PHASE_OCP_DURATION 50
#endif
// </e>

// </h>

// <o> �]�w�w�����ɶ�
// <i> unit : mSec ex. 100 * 50us = 5ms
#define I_ALIG_DURATION 10 // unit : mSec ex. 100 * 50us = 5ms

// <o> �]�w�����ҰʧP�_����ɶ�
// <i> unit : mSec ex. 2000 * 50us = 100ms
#define TAILWIND_DURATION 2000 // unit : mSec ex. 2000 * 50us = 100ms

// <o> �]�w Start ��t
// <i> unit : rpm
#define START_SPEED_VALUE (short)(((long) 800 * PLOE_PAIRS) >> AngleBaseShift)

// <o> �]�w Start �q�y
// <i> unit : mA
#define IQ_START (float)400/1000 
#define IQ_START_VALUE (signed short)((float)(IQ_START * I_AMPLIFIER)) // unit : A * Gain
	
// <o> �]�w Stop_Fun ������t
// <i> unit : rpm
#define STOP_SPEED_VALUE (short)((long) 100 * PLOE_PAIRS) >> AngleBaseShift // unit : Val
	
// <o> �]�w Stop_Fun ���ɮɶ�
// <i> unit : mSec
#define STOP_SPEED_DURATION 1000

// <o> �]�w Retry ���� <0-255>
// <o1> �]�w Restart ���ɮɶ� 1 <0-65535>
// <i> Ex. 20000 = 1 Sec
// <o2> �]�w Restart ���ɮɶ� 2 <0-65535> 
#define RETRY_COUNT 10
#define RESTART_DURATIONT_1 1000
#define RESTART_DURATIONT_2 5
// <<< end of configuration section >>>

// �]�w ���F FG �H��
#define FG P3_3

// �p�� SPEED ��t���⤽�� : Value = (rpm * PLOE_PAIRS) >> AngleBaseShift

// �]�w IQ �f���P�_�q�y
#define IQ_TAILWIND (float)(-0.0305) // unit : A
#define IQ_TAILWIND_VALUE (signed short)((float)(IQ_TAILWIND * I_AMPLIFIER)) // unit : A * Gain

// Vbus �����q���]�w �P �L�� �L�C���O�@����
#define R1 20 // unit : k-Ohm
#define R2 10  // unit : k-Ohm
#define V_AMPLIFIER (unsigned short)((float)(R2/(R1+R2) * 1024/5)) // unit : Gain

#define OVER_BUS_VOLT 30 // unit : V
#define CLEAR_OVER_BUS_VOLT 28 // unit : V
#define UNDER_BUS_VOLT 18// unit : V
#define CLEAR_UNDER_BUS_VOLT 20// unit : V

#define OVER_TEMPERTURE 110 // unit : DegC
#define CLEAR_OVER_TEMPERTURE 90 // unit : DegC


// MOTOR_CONT1
#define MPWMSEL 0x40
#define MPWMEN 0x20
#define IQINSEL 0x10
#define FOCANGSEL 0x08

// MOTOR_CONT2
#define MPWMCPSEN 0x80
#define MPWMCPSTP 0x00
//#define MPWMCPSTP 0x20
//#define MPWMCPSTP 0x60
#define SVPWMPS 0x08 // CW or CCW
#define CPS_LIMIT_EN 0x00
//#define SVPWMMODE 0x00 // SVPwm
#define SVPWMMODE 0x10 // DPwm

// MOTOR_CONT3
#define SMO_CMP_VALUE 0x40

// PI_GAIN
#define PLLKIGEN 0x01
#define PLLKPGEN 0x02
#define SPEEDKIGEN 0x04
#define SPEEDKPGEN 0x08
#define IDKIGEN 0x10
#define IDKPGEN 0x20
#define IQKIGEN 0x40
#define IQKPGEN 0x80

// PI_TMSR
#define PLLTMEN 0x01
#define PLLTMSEL 0x02
#define SPEEDTMEN 0x04
#define SPEEDTMSEL 0x08
#define IDTMEN 0x10
#define IDTMSEL 0x20
#define IQTMEN 0x40
#define IQTMSEL 0x80

//FOCCONT
#define SPEEDEN 0x01
#define PLLEN 0x02
#define ADCTRIG 0x04
#define INVADCD 0x08
#define ESTCR 0x00
#define PICLEAR 0x80

#define IQ_CMD_MACRO(v) SFR_PAGE = 0; PI_CMD = v
#define ID_CMD_MACRO(v) SFR_PAGE = 1; PI_CMD = v
#define SPEED_CMD_MACRO(v) SFR_PAGE = 2; PI_CMD = v
#define PLL_CMD_MACRO(v) SFR_PAGE = 3; PI_CMD = v
#define USER_CMD_MACRO(v) SFR_PAGE = 4; PI_CMD = v

#define IQ_KP_MACRO(v) SFR_PAGE = 0; PI_KP = v
#define ID_KP_MACRO(v) SFR_PAGE = 1; PI_KP = v
#define SPEED_KP_MACRO(v) SFR_PAGE = 2; PI_KP = v
#define PLL_KP_MACRO(v) SFR_PAGE = 3; PI_KP = v
#define USER_KP_MACRO(v) SFR_PAGE = 4; PI_KP = v

#define IQ_KI_MACRO(v) SFR_PAGE = 0; PI_KI = v
#define ID_KI_MACRO(v) SFR_PAGE = 1; PI_KI = v
#define SPEED_KI_MACRO(v) SFR_PAGE = 2; PI_KI = v
#define PLL_KI_MACRO(v) SFR_PAGE = 3; PI_KI = v
#define USER_KI_MACRO(v) SFR_PAGE = 4; PI_KI = v

#define IQ_KT_MACRO(v) SFR_PAGE = 0; PI_KT = v
#define ID_KT_MACRO(v) SFR_PAGE = 1; PI_KT = v
#define SPEED_KT_MACRO(v) SFR_PAGE = 2; PI_KT = v
#define PLL_KT_MACRO(v) SFR_PAGE = 3; PI_KT = v
#define USER_KT_MACRO(v) SFR_PAGE = 4; PI_KT = v

#define IQ_TR_MACRO(v) SFR_PAGE = 0; PI_KT = v
#define ID_TR_MACRO(v) SFR_PAGE = 1; PI_KT = v
#define SPEED_TR_MACRO(v) SFR_PAGE = 2; PI_KT = v
#define PLL_TR_MACRO(v) SFR_PAGE = 3; PI_KT = v
#define USER_TR_MACRO(v) SFR_PAGE = 4; PI_KT = v

#define IQ_UI_MACRO(v) SFR_PAGE = 0; PI_UI = v
#define ID_UI_MACRO(v) SFR_PAGE = 1; PI_UI = v
#define SPEED_UI_MACRO(v) SFR_PAGE = 2; PI_UI = v
#define PLL_UI_MACRO(v) SFR_PAGE = 3; PI_UI = v
#define USER_UI_MACRO(v) SFR_PAGE = 4; PI_UI = v

#define IQ_OUT_MACRO(v) SFR_PAGE = 0; PI_OUT = v
#define ID_OUT_MACRO(v) SFR_PAGE = 1; PI_OUT = v
#define SPEED_OUT_MACRO(v) SFR_PAGE = 2; PI_OUT = v
#define PLL_OUT_MACRO(v) SFR_PAGE = 3; PI_OUT = v
#define USER_OUT_MACRO(v) SFR_PAGE = 4; PI_OUT = v

#define IQ_MAX_MACRO(v) SFR_PAGE = 0; PI_MAX = v
#define ID_MAX_MACRO(v) SFR_PAGE = 1; PI_MAX = v
#define SPEED_MAX_MACRO(v) SFR_PAGE = 2; PI_MAX = v
#define PLL_MAX_MACRO(v) SFR_PAGE = 3; PI_MAX = v
#define USER_MAX_MACRO(v) SFR_PAGE = 4; PI_MAX = v

#define IQ_MIN_MACRO(v) SFR_PAGE = 0; PI_MIN = v
#define ID_MIN_MACRO(v) SFR_PAGE = 1; PI_MIN = v
#define SPEED_MIN_MACRO(v) SFR_PAGE = 2; PI_MIN = v
#define PLL_MIN_MACRO(v) SFR_PAGE = 3; PI_MIN = v
#define USER_MIN_MACRO(v) SFR_PAGE = 4; PI_MIN = v

#define GS_MACRO(v) SFR_PAGE = 0; SMO_D1 = v
#define SMO_KSLIDE_MACRO(v) SFR_PAGE = 1; SMO_D1 = v
#define ANG_BASE_MACRO(v) SFR_PAGE = 2; SMO_D1 = v
#define Z_CORR_MACRO(v) SFR_PAGE = 3; SMO_D1 = v
#define SMO_ANG_MACRO(v) SFR_PAGE = 4; SMO_D1 = v
#define BAN_GAIN_MACRO(v) SFR_PAGE = 5; SMO_D1 = v

#define FS_MACRO(v) SFR_PAGE = 0; SMO_D2 = v
#define SMO_FILTER_MACRO(v) SFR_PAGE = 1; SMO_D2 = v
#define ANG_SHIFT_MACRO(v) SFR_PAGE = 2; SMO_D2 = v
#define MAXSMC_ERR_MACRO(v) SFR_PAGE = 3; SMO_D2 = v
#define SPEED_MACRO(v) SFR_PAGE = 4; SMO_D2 = v
#define VQ_OFFSET_MACRO(v) SFR_PAGE = 0; FOC_D2 = v; SYNC = 0x55
#define VD_OFFSET_MACRO(v) SFR_PAGE = 1; FOC_D2 = v; SYNC = 0x55
#define AS_MACRO(v) SFR_PAGE = 3; FOC_D2 = v
#define CPU_ANG_MACRO(v) SFR_PAGE = 4; FOC_D2 = v; SYNC = 0x55
#define FOC_ANG_MACRO(v) SFR_PAGE = 5; FOC_D2 = v
#define SVPWM_AMP_MACRO(v) SFR_PAGE = 6; FOC_D2 = v

#define Uint32_Div_Uint16_MACRO(Dividend,Divisor,Answer)\
MD_MODE |= 0x02;\
MD0 = Dividend & 0xFF;\
MD1 = (Dividend & 0xFF00) >> 8);\
MD2 = (Dividend & 0xFF0000) >> 16);\
MD3 = (Dividend & 0xFF000000) >> 24);\
MD4 = Divisor & 0xFF;\
MD5 = (Divisor & 0xFF00) >> 8);\
while((MD_MODE & 0x10) == 0x10);\
Answer = MD0;\
Answer |= MD1 << 8;\
Answer |= MD2 << 16;\
Answer |= MD3 << 24

#define Uint16_Div_Uint16_MACRO(Dividend,Divisor,Answer)\
MD_MODE |= 0x02;\
MD0 = Dividend & 0xFF;\
MD1 = (Dividend & 0xFF00) >> 8);\
MD4 = Divisor & 0xFF;\
MD5 = (Divisor & 0xFF00) >> 8;\
while((MD_MODE & 0x10) == 0x10);\
Answer = MD0;\
Answer |= MD1 << 8

#define Uint16_Mult_Uint16_MACRO(MultCand,MultCator,Answer)\
MD_MODE |= 0x02;\
MD0 = MultCand & 0xFF;\
MD4 = MultCator & 0xFF;\
MD1 = (MultCand & 0xFF00) >> 8;\
MD5 = (MultCator & 0xFF00) >> 8;\
while((MD_MODE & 0x10) == 0x10);\
Answer = MD0;\
Answer |= MD1 << 8;\
Answer |= MD2 << 16;\
Answer |= MD3 << 24

extern short SpeedCmd;
extern short CurrentCmd;
extern unsigned char MotorDir;

extern unsigned char SystemState;
extern unsigned char MotorState;
extern unsigned char StartUpState;
extern unsigned char MotorErrorState;
extern unsigned char MotorFaultState;

extern unsigned short VbusTemp;
extern unsigned short IbusTemp;
extern unsigned short Vbus_avg;
extern unsigned short Ibus_avg;
//extern unsigned short VbusOffset;
extern unsigned short IbusOffset;
extern unsigned short VspTemp;
extern unsigned short Vsp_avg;

extern xdata short InitAngle;
extern xdata signed short LatestTheta;
extern xdata unsigned char CloseLoopFlag;
extern xdata unsigned char CCWFlag;
extern xdata unsigned char BemfStateOld;
extern xdata unsigned char BemfState;
extern xdata short EstimatedSpeed;
extern xdata short EstimatedSpeedOld;
extern xdata unsigned short BEMF_SPEED_Cnt;
extern xdata short mtrAngle;
extern xdata unsigned short RecHallUEdgeCnt;
extern xdata unsigned short HallUEdgeCnt;
extern unsigned short int V_BUS;
extern unsigned short int I_BUS;
extern unsigned long Watt;
extern unsigned long Watt_InRef;

extern xdata short BmfVW_Sub;
extern xdata short BmfVW_Sum_Pos_ZeroPoint;
extern xdata short BmfVW_Sum_Neg_ZeroPoint;
extern xdata short BmfU;
extern xdata short BmfV;
extern xdata short BmfW;
extern xdata char ZeroPointCheck;
extern xdata char ZeroPointDebounceCnt;
extern xdata char ZeroPointCnt;
extern xdata char ZeroPointFlag;
extern xdata char ZeroPointFlagOld;

extern void Motor_Control (void);
extern void ResetMOC (void);
//extern void VdcCorrection (void);
//extern void VdcCorrection_Init (void);
extern void MOC_Init (void);
extern void Iq_PI_Control_Init (void);
extern void Id_PI_Control_Init (void);
extern void Speed_PI_Control_Init (void);
extern void PLL_PI_Control_Init (void);
//extern void User_PI_Control_Init (void);

// Protect
#if (FaultLock_Protect == 1)
extern void FaultLock_Fun (void);
#endif
#if (Vbus_Protect == 1)
extern void Vbus_Protect_Fun (unsigned short Adc_Vbus);
#endif
#if (Temperture_Protect == 1)
extern void Temperture_Protect_Fun (unsigned short Adc_Temperture);
#endif
extern void AOCP_Protect_Fun (void);
#if (POCP_Protect == 1)
extern void Phase_OCP_Protect_Fun (void);
#endif

// Function
#if (POWER_LIMIT == 1)
extern void PowerLimit_Fun (void);
#endif
#if (POWER_CONTROL == 1)
extern void PowerControl_Fun (void);
#endif
extern void FG_Fun (void);
#if (VSP_TRI == 1)
extern void Vsp_Fun (void);
#endif
#if (BEMF_TAILWIND_FUNCTION_1 == 1) || (BEMF_TAILWIND_FUNCTION_3 == 1)
extern void MotorDirCheck_Fun (void);
extern void MotorAngle_Fun(void);
#endif

extern void MotorInit_Fun (void);
extern void MotorStart_Flow (void);
extern void MotorStartRetry_Flow (void);
extern short Ramp_Fun (short TargetValue,SetpointValue,HighLimit,LowLimit);

extern void MotorCloseLoop_Fun (void);
extern void BemfTailWindDetect_Fun (void);
extern void TailWindDetect_Fun (void);
extern void TailWindStart_Fun (void);
//extern void HeadWindStart_Fun (void);

extern void IPDDetect_Fun (void);
extern void IPDStart_Fun (void);

extern void Parking_Fun (void);
extern void ParkingStart_Fun (void);
extern void Break_Fun (void);
extern void Stop_Fun (void);
extern void Reverse_Fun(void);

extern void Read_Iq (void);
extern void Read_Vq (void);
extern void Read_Speed (void);
#endif