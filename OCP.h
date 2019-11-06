#ifndef OCP_H
#define OCP_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> AOCPCONT °t¸m
// <o.0..2> I_SHORT <0=> 0.15V <1=> 0.2V <2=> 0.25V <3=> 0.3V <4=> 0.35V <5=> 0.4V <6=> 0.45V <7=> 0.5V
// <o.4> IPD <0=> From AOCP <1=> From OPA 
// <o.5> OPAPD <0=> Normal <1=> OPA Power Down 
// <o.6> AOCPEN <0=> Disable <1=> Enable 
// <o.7> DOCPEN <0=> Disable <1=> Enable 
// </h>
#define AOCPCONT_REGS 0x47

// <h> OCPCONT °t¸m
// <o.0> OCPMS <0=> Auto Mode <1=> User Mode 
// <o.2..6> AOCPDBT <0-31>
// <i> Analog OCP input de-bounce time (default 41.67nS) 
// <i> 0~31 = 0~1.291uS (48MHz/2 fixed)
// </h>
#define OCPCONT_REGS 0x7D 
// <<< end of configuration section >>>

// AOCPCONT
#define AOCPEN 0x40
#define DOCPEN 0x80
#define AOCP_LEVEL_VOLTAGE_015 0x0
#define AOCP_LEVEL_VOLTAGE_02  0x1
#define AOCP_LEVEL_VOLTAGE_025 0x2
#define AOCP_LEVEL_VOLTAGE_03  0x3
#define AOCP_LEVEL_VOLTAGE_035 0x4
#define AOCP_LEVEL_VOLTAGE_04  0x5
#define AOCP_LEVEL_VOLTAGE_045 0x6
#define AOCP_LEVEL_VOLTAGE_05  0x7

// OCPCONT
#define OCPST 0x80 // OCP Short status
#define OCPDBT 0x1F<<6 // Analog OCP input de-bounce time (0 ~ 31) = (0 ~ 1.291us)
#define OCPC 0x02 // OCP status clear
#define OCPMS 0x01// User Mode

extern void OCP_Init (void);
#endif