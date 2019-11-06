#ifndef CAPTURE_H
#define CAPTURE_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> CAPCONT °t¸m
// <o.0..2> E_CAP_CLK  <0=> 48MHz/4  <1=> 48MHz/8  <2=> 48MHz/16  <3=> 48MHz/32  <4=> 48MHz/64  <5=> 48MHz/128  <6=>  48MHz/256  <7=> 48MHz/512 
// <o.4> CAP_PIN_SEL <0=> CAP2/P0_7/Data 0 Input <1=> CAP1/P0_6/Data 1 Input
// <o.7> E_CAP_EN <0=> Disable <1=> Enable 
// </h>
#define CAPCONT_REGS 0x87

// <<< end of configuration section >>>

//#if ((CAPCONT_REGS & 0x07) == 0x00)
//#define SPEED_VALUE ((((unsigned long)48000000/4)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x01)
//#define SPEED_VALUE ((((unsigned long)48000000/8)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x02)
//#define SPEED_VALUE ((((unsigned long)48000000/16)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x03)
//#define SPEED_VALUE ((((unsigned long)48000000/32)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x04)
//#define SPEED_VALUE ((((unsigned long)48000000/64)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x05)
//#define SPEED_VALUE ((((unsigned long)48000000/128)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x06)
//#define SPEED_VALUE ((((unsigned long)48000000/256)*60)/PLOE_PAIRS)
//#endif
//#if ((CAPCONT_REGS & 0x07) == 0x07)
//#define SPEED_VALUE ((((unsigned long)48000000/512)*60)/PLOE_PAIRS)
//#endif

#if ((CAPCONT_REGS & 0x07) == 0x00)
#define SPEED_VALUE ((((unsigned long)48000000/4)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x01)
#define SPEED_VALUE ((((unsigned long)48000000/8)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x02)
#define SPEED_VALUE ((((unsigned long)48000000/16)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x03)
#define SPEED_VALUE ((((unsigned long)48000000/32)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x04)
#define SPEED_VALUE ((((unsigned long)48000000/64)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x05)
#define SPEED_VALUE ((((unsigned long)48000000/128)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x06)
#define SPEED_VALUE ((((unsigned long)48000000/256)*60))
#endif
#if ((CAPCONT_REGS & 0x07) == 0x07)
//#define SPEED_VALUE ((((unsigned long)48000000/512)*60))
//#define FREQ_VALUE ((((unsigned long)(float)24.512 * 48000000/512)))
#define SPEED_VALUE (unsigned long)5625000
#define FREQ_VALUE (unsigned long)2298000
#endif
//RealRpm = (long)11250000 / RotorSpeedTemp;

//#define BEMF_MACRO(v) v = SPEED_VAL / v

extern unsigned short CaptureTotal;
extern unsigned short CaptureTotalTemp;

extern void EXCAP_Init (void);
extern void CAP_Init (void);
#endif