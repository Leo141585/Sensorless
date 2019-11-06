#ifndef GPIO_H
#define GPIO_H

// <<< Use Configuration Wizard in Context Menu >>>
// <h> PINCONG1 配置
// <o.0..1> CH7 
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> CH6 
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> CH5 
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> CH4
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG1_REGS 0xA9

// <h> PINCONG2 配置
// <o.0..1> CH3
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> CH2
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> CH1P
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> CH0P
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG2_REGS 0xAA

// <h> PINCONG3 配置
// <o.0..1> XTALI 
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> XTALO
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> PWM U
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> PWM X
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG3_REGS 0x5A

// <h> PINCONG4 配置
// <o.0..1> PWM V
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> PWM Y
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> PWM W
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> PWM Z
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG4_REGS 0x55

// <h> PINCONG5 配置
// <o.0..1> AOCP
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> CH0N
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> CH1N
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> DOCP
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG5_REGS 0xAA

// <h> PINCONG6 配置
// <o.0..1> TX
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> RX
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.4..5> IIC SDA
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.6..7> IIC SDL
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG6_REGS 0x00

// <h> PINCONG7 配置
// <o.0..1> IPWM
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// <o.2..3> FG
// <0=> Quasi-bidirectional(standard 8051 port outputs) 
// <1=> Push-pull output 
// <2=> Input-only (High impedance) 
// <3=> Open drain output
// </h>
#define PINCONG7_REGS 0x05

// <h> PINSET1 配置
// <o.0..1> CH7 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> CH6 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> CH5 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> CH4
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET1_REGS 0x00

// <h> PINSET2 配置
// <o.0..1> CH3 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> CH2 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> CH1P 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> CH0P
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET2_REGS 0x00

// <h> PINSET3 配置
// <o.0..1> XTALI
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> XTALO 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> PWM U 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> PWM X
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET3_REGS 0x0A

// <h> PINSET4 配置
// <o.0..1> PWM V 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> PWM Y
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> PWM W
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> PWM Z
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET4_REGS 0x00

// <h> PINSET5 配置
// <o.0..1> AOCP 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> CH0N 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> CH1N 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> DOCP
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET5_REGS 0x00

// <h> PINSET6 配置
// <o.0..1> TX 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> RX 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> IIC SDA
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.6..7> IIC SDL
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// </h>
#define PINSET6_REGS 0xA0

// <h> PINSET7 配置
// <o.0..1> IPWM
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.2..3> FG 
// <0=> No Pull
// <1=> Pull Down
// <2=> Pull Up
// <3=> No Pull
// <o.4..5> DOCPDB 
// <0=> 0ns
// <1=> 250ns
// <2=> 500ns
// <3=> 1000ns
// <o.7> MOC
// <0=> U V W X Y Z is GPIO Mode 
// <1=> U V W X Y Z is SVPWM Mode 
// <i> MOCS = 1, Pin set must to Push-pull output mode. (PINSET3 [7:4] and PINSET4 [7:0])
// </h>
#define PINSET7_REGS 0x8A
// <<< end of configuration section >>>

extern void GPIO_Init (void);
#endif