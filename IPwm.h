#ifndef IPWM_H
#define IPWM_H

#define IPWM_Frequency_20KHz (unsigned short) 20000
#define IPWM_Counter (unsigned short) ((48000000/IPWM_Frequency_20KHz)/2)

#define IPWM_Duty(PwmDuty)\
IPWM_DUTY_H = (PwmDuty & 0xFF00)>>8;\
IPWM_DUTY_L = (PwmDuty & 0x00FF);\
SYNC = 0x55


extern void IPWM_Init (void);
//extern void IPWM_Duty (unsigned short PwmDuty);
#endif