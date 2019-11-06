#ifndef ADC_H
#define ADC_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> ADCCONT °t¸m
// <o.0..2> ADCCH <0=> CH0 <1=> CH1 <2=> CH2 <3=> CH3 <4=> CH4 <5=> CH5 <6=> CH6 <7=> CH7 
// <o.3> ADCCKS <0=> 24MHz <1=> 12MHz
// <o.4> ADCDS <0=> ADCD2 MSB <1=> ADCD2 LSB
// <o.5..6> ADCSH <0=> 1 clock <1=> 2 clock <2=> 3 clock <3=> 4 clock
// <o.7> ADCPD <0=> Normal <1=> Power Down
// </h>
#define ADCCONT_REGS 0x10

// <h> ADCSTR °t¸m
// <o.6..7> SHCKS <0=> 6MHz <1=> 3MHz <2=> 2.4MHz <3=> 2MHz
// </h>
#define ADCSTR_REGS 0x00
// <<< end of configuration section >>>

extern void Adc_Init (void);
extern unsigned short Adc_Cannel (unsigned char Channel);
#endif