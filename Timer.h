#ifndef TIMER_H
#define TIMER_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> PFCON 配置
// <i> 系統時脈 F_PER : 24MHz
// <o.0..1> T0PS 
// <0=> F_PER/12 (2MHz)
// <1=> F_PER/1 (24MHz)
// <2=> F_PER/96 (250KHz)

// <o.2..3> T1PS 
// <0=> F_PER/12 (2MHz)
// <1=> F_PER/1 (24MHz)
// <2=> F_PER/96 (250KHz)

// <o.4..5> SRELPS 
// <0=> F_PER/64
// <1=> F_PER/32
// <2=> F_PER/16
// <3=> F_PER/8
// </h>
#define PFCON_REGS 0x00

// <h> TMOD 配置
// <o.0..1> T0 Mode
// <0=> 13-bit Counter/Timer (Not auto-reload)
// <1=> 16-bit Counter/Timer (Not auto-reload)
// <2=> 8-bit Counter/Timer (Auto-reload)
// <3=> 8-bit Counter/Timer (Not auto-reload)

// <o.2> C/T0 
// <0=> Timer
// <1=> Counter

// <o.3> GATE0 
// <0=> --
// <1=> Timer 0 Gate Control

// <o.4..5> T1 Mode
// <0=> 13-bit Counter/Timer (Not auto-reload)
// <1=> 16-bit Counter/Timer (Not auto-reload)
// <2=> 8-bit Counter/Timer (Auto-reload)
// <3=> 8-bit Counter/Timer (Not auto-reload)

// <o.6> C/T1 
// <0=> Timer
// <1=> Counter

// <o.7> GATE1 
// <0=> --
// <1=> Timer 1 Gate Control
// </h>
#define TMOD_REGS 0x11

// <h> T2CON 配置
// <i> 系統時脈 F_PER : 24MHz
// <o.0..1> T2PS 
// <0=> F_PER/12 (2MHz)
// <1=> F_PER/1 (24MHz)
// <2=> F_PER/96 (250KHz)

// <o.2..3> T2 Mode 
// <0=> 13-bit Counter/Timer (Not auto-reload)
// <1=> 16-bit Counter/Timer (Not auto-reload)
// <2=> 8-bit Counter/Timer (Auto-reload)
// <3=> 8-bit Counter/Timer (Not auto-reload)
// </h>
#define T2CON_REGS 0x05

// <e> TIMER0 配置
// <o1> 中斷頻率 TIMER0_FREQ 
// <i> unit : Hz
// </e>
#define TIMER0_ENABLE 1
#define TIMER0_FREQ 1000 // unit : Hz

#if ((PFCON_REGS & 0x03) == 0x00)
#define TIMER0_COUNT 2000000/TIMER0_FREQ
#elif ((PFCON_REGS & 0x03) == 0x01)
#define TIMER0_COUNT 24000000/TIMER0_FREQ
#elif ((PFCON_REGS & 0x03) == 0x02)
#define TIMER0_COUNT 250000/TIMER0_FREQ
#endif

//#define TIMER0_COUNT 2000000/TIMER0_FREQ
#define TIMER0_TH (65535-TIMER0_COUNT)>>8
#define TIMER0_TL (65535-TIMER0_COUNT)&0xFF

// <e> TIMER1 配置
// <o1> 中斷頻率 TIMER1_FREQ 
// <i> unit : Hz  
// </e>
#define TIMER1_ENABLE 0
#define TIMER1_FREQ 100 // unit : Hz

#if ((PFCON_REGS & 0x0C) == 0x00)
#define TIMER1_COUNT 2000000/TIMER1_FREQ
#elif ((PFCON_REGS & 0x0C) == 0x04)
#define TIMER1_COUNT 24000000/TIMER1_FREQ
#elif ((PFCON_REGS & 0x0C) == 0x08)
#define TIMER1_COUNT 250000/TIMER1_FREQ
#endif

//#define TIMER1_COUNT 2000000/TIMER1_FREQ
#define TIMER1_TH (65535-TIMER1_COUNT)>>8
#define TIMER1_TL (65535-TIMER1_COUNT)&0xFF

// <e> TIMER2 配置
// <o1> 中斷頻率 TIMER2_FREQ 
// <i> unit : Hz
// </e>
#define TIMER2_ENABLE 1
#define TIMER2_FREQ 100 // unit : Hz

#if ((T2CON_REGS & 0x03) == 0x00)
#define TIMER2_COUNT 2000000/TIMER2_FREQ
#elif ((T2CON_REGS & 0x03) == 0x01)
#define TIMER2_COUNT 24000000/TIMER2_FREQ
#elif ((T2CON_REGS & 0x03) == 0x02)
#define TIMER2_COUNT 250000/TIMER2_FREQ
#endif

//#define TIMER2_COUNT 2000000/TIMER2_FREQ
#define TIMER2_TH (65535-TIMER2_COUNT)>>8
#define TIMER2_TL (65535-TIMER2_COUNT)&0xFF
// <<< end of configuration section >>>

extern void Timer_Init (void);
#endif