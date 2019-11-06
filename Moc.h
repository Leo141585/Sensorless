#ifndef MOC_H
#define MOC_H
// <<< Use Configuration Wizard in Context Menu >>>
// <h> MOTOR_CONT1 配置
// <o.0> SPFB_FILETER <0=> Disable <1=> Enable 
// <o.5> MPWMEN <0=> Disable <1=> Enable 
// <o.6> MPWMSEL <0=> From Register <1=> From SVPWM Register
// </h>
#define MOTOR_CONT1_REGS 0x61

// <h> MOTOR_CONT2 配置
// <o.0> CPS_OP_SEL <0=> Valpha and Vbeta addition with Compensation factor (Default) <1=> Valpha and Vbeta subtraction with Compensation factor
// <o.2> CPS_LIMIT_EN <0=> Dead time compensation without Lowest duty limit <1=> Dead time compensation + Lowest duty limit (Default)
// <o.4> SVPWMMODE <0=> SVPWM <1=> D-SVPWM 
// <o.5..6> MPWMCPSTP <0=> Dead time compensate <1=> Dead time compensate in low amplitude (Default) <2=> Lowest duty limit is 2xDeadband
// <o.7> MPWMCPSEN <0=> Disable <1=> Enable
// </h>
#define MOTOR_CONT2_REGS 0xC4

// <h> MOTOR_CONT3 配置
// <o.0> CPS_LMT_SEL <0=> Dead time compensation min duty limit is 2xdeadband <1=> Dead time compensation min duty limit is deadband
// <o.2> CH2 EN <0=> Disable <1=> Enable
// <o.3> CH3 EN <0=> Disable <1=> Enable
// <o.4..5> IDQ_FB_SEL <0=> Iq/Id direct feedback <1=> Two Iq/Id value average <2=> Four Iq/Id value average
// <o.6> SMO_CMP_VALUE <0=> Compare with MAX_SMC_ERR <1=> Compare with 0
// <o.7> SMO_BIT_RANGE <0=> [29:14] <1=> [30:15]
// </h>
#define MOTOR_CONT3_REGS 0x01

// <h> MPWMCPSF 配置 
// <o> MPWMCPSF <0-255>
// </h>
#define MPWMCPSF_REGS 60

// <h> SVPWM_AMP 配置 
// <o> SVPWM_AMP <0-65535>
// </h>
#define SVPWM_AMP_REGS 0x4000

// <h> PI_GAIN 配置 
// <o.0> PLL KI Gain x16 <0=> Disable <1=> Enable
// <o.1> PLL KP Gain x16 <0=> Disable <1=> Enable
// <o.2> SPEED KI Gain x16 <0=> Disable <1=> Enable
// <o.3> SPEED KP Gain x16 <0=> Disable <1=> Enable
// <o.4> ID KI Gain x16 <0=> Disable <1=> Enable
// <o.5> ID KP Gain x16 <0=> Disable <1=> Enable
// <o.6> IQ KI Gain x16 <0=> Disable <1=> Enable
// <o.7> IQ KP Gain x16 <0=> Disable <1=> Enable
// </h>
#define PI_GAIN_REGS 0xA0

// <h> PI_TMSR 配置 
// <o.0> PLL Tracking Mode Enable <0=> Disable <1=> Enable
// <o.1> PLL Tracking Mode Select <0=> Auto Mode <1=> Manual Mode
// <o.2> SPEED Tracking Mode Enable <0=> Disable <1=> Enable
// <o.3> SPEED Tracking Mode Select <0=> Auto Mode <1=> Manual Mode
// <o.4> ID Tracking Mode Enable <0=> Disable <1=> Enable
// <o.5> ID Tracking Mode Select <0=> Auto Mode <1=> Manual Mode
// <o.6> IQ Tracking Mode Enable <0=> Disable <1=> Enable
// <o.7> IQ Tracking Mode Select <0=> Auto Mode <1=> Manual Mode
// </h>
#define PI_TMSR_REGS 0x00
// <<< end of configuration section >>>
#endif