#ifndef MAIN_H
#define MAIN_H

#define Timer1ms 1
#define Timer5ms 5
#define Timer10ms 10

#define PeriodTime10ms 10
#define PeriodTime50ms 50
#define PeriodTime100ms 100
#define PeriodTime1000ms 1000

#define SDModeEnable 1
#define SDModeDisable 0

#define SDMode0 0x0
#define SDMode1 0x1
#define SDMode2 0x2
#define SDMode3 0x3

#define PLLDiv1 0x0<<2 // 48MHz
#define PLLDiv2 0x1<<2 // 24MHz
#define PLLDiv4 0x2<<2 // 12MHz
#define PLLDiv8 0x3<<2 // 6MHz

#define SET(reg,bit) (reg |= (1<<bit))
#define CLR(reg,bit) (reg &= ~(1<<bit))
#define SetBit(reg,bit,val) val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit) (reg & (1<<bit))
#define SetReg(reg,val) (reg = val)
#define Q15(n) (signed short)(n*32767.0)
	
typedef struct _MOTOR_COMPONENT// the struct name is a suggestion
{
	unsigned short a;
	unsigned short b;
	unsigned short c;
}MOTOR_COMPONENT,*PMOTOR_COMPONENT;// here are two types

extern unsigned short TimeOut;
extern unsigned char UartFlag;
extern unsigned char IPDFlag;
#endif
