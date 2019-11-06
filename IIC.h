#ifndef IIC_H
#define IIC_H

extern unsigned char IICWorkFlag;
extern unsigned char IICStep;
extern unsigned char IICFlag;
extern unsigned char IICSendFlag;
extern unsigned short IICCount;
extern xdata unsigned char IICBuf[10];

extern void IIC_MCP4628_Package_Build (void);
extern void IIC_MCP4728_Send_Buffer (void);
#endif