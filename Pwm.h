#ifndef PWM_H
#define PWM_H

enum PwmMod {ForceLow = 0, ForceHi = 1, ActiveHi = 2, ActiveLow = 3};
// <<< Use Configuration Wizard in Context Menu >>>
// <o> MPWM SWAP 配置 <0x00=> ET1202/ET1212 <0x70=> ET1232/ET1272
#define SWAP 0x70

// <h> MPWMDATA 配置
// <i> The frequency of MPWMDATA is 48MHz
// <o> 設定 PWM 頻率 <12000-40000 : 10>
// <i> unit : Hz
// </h>
#define PWM_FREQ 20000
#define MPWMDATA_REGS (unsigned short) ((48000000/PWM_FREQ)/2)

// <h> MPWMINV 配置
// <o.0> U INV <0=> Non-Inverse <1=> Inverse
// <o.1> X INV <0=> Non-Inverse <1=> Inverse
// <o.2> V INV <0=> Non-Inverse <1=> Inverse
// <o.3> Y INV <0=> Non-Inverse <1=> Inverse
// <o.4> W INV <0=> Non-Inverse <1=> Inverse
// <o.5> Z INV <0=> Non-Inverse <1=> Inverse
// </h>
 #define MPWMINV_REGS 0x00
 
// <h> MPWMDB 配置
// <i> The frequency of MPWMDB is 48MHz
// <o> 設定 Deadband Time 
// <0x30=> Deadband Time 1us
// <0x48=> Deadband Time 1.5us
// <0x60=> Deadband Time 2us
// <0x78=> Deadband Time 2.5us
// <0xF0=> Deadband Time 5us
// <i> 0x30 : 48MHz * 48 = 1us
// <i> 0x48 : 48MHz * 72 = 1.5us
// <i> 0x60 : 48MHz * 96 = 2us
// <i> 0x78 : 48MHz * 144 = 2.5us
// </h>
#define MPWMDB_REGS 0x30 

#define PWM_Frequency PWM_FREQ
#define PWM_Counter MPWMDATA_REGS
// 1rps (rad/sec) = 1/60 rpm,
// 0 ~ 24512 為電機角 0 ~ 360 deg
// 設定 AngleBase 計算公式 : AngleBase = 24512 * (1/PWM_Frequency) * (1/60) * 32767
//#define AngleBase (long) ((float)(24512 * (1/PWM_Frequency) * (1/60)) * 32767)
#define AngleBase (long) 13386411/PWM_Frequency

// <o> 設定 AngleBaseShift <0-8>
#define AngleBaseShift 0
// <<< end of configuration section >>>

extern void PWM_SetBreakForce (void);
extern void PWM_SetBreak (void);
extern void PWM_SetAllOff (void);
extern void PWM_SetActive (void);
extern void PWM_Init (void);
extern void PWM_Duty (unsigned short PwmDuty);
#endif