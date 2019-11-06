#include "ET1202_SFR.h"
#include "XSFR_GLOBAL.h"
#include "main.h"
#include "IAE.h"
#include "OCP.h"

// <<< Use Configuration Wizard in Context Menu >>>
// <h> IPD LEVEL 配置
// <o.0..2> I_SHORT <0=> 0.15V <1=> 0.2V <2=> 0.25V <3=> 0.3V <4=> 0.35V <5=> 0.4V <6=> 0.45V <7=> 0.5V
// <i> 說明 : IPD 聲音與電壓成正比
// <o.6> AOCPEN <0=> Disable <1=> Enable 
// <o.7> DOCPEN <0=> Disable <1=> Enable 
// </h>
#define IPD_LEVEL 0x53

// <<< end of configuration section >>>

unsigned char IPDPattern = 0;

#define IAE_SOFT 0x02

#define IAES 0x00
#define IAEPS 0x00
#define IAEPNS 0x08
#define IAECYC 0x00
#define IAEEN 0x01

void IPD_Init (void)
{
	//MOTOR_CONT2 |= IAE_SOFT;
	
	AOCPCONT = IPD_LEVEL;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_015;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_02;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_025;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_03;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_035;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_04;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_045;
//	AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_05;
	
	INI_ANG_CTRL = IAEPS | IAEPNS | IAECYC | IAEEN;
	while((INI_ANG_CTRL & 0x80) == 0x80);
	IPDPattern = (INI_ANG_CTRL & 0x70)>>4;
	
	//AOCPCONT = AOCPEN | AOCP_LEVEL_VOLTAGE_05;
	//OCPCONT |= 0x02;// Clear OCP Status
	
	AOCPCONT = AOCPCONT_REGS;
	OCPCONT = OCPCONT_REGS | OCPC;
}
