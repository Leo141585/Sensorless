#ifndef EEPROM_H
#define EEPROM_H

#define EepErase 0x8
#define EepWrite 0x2
#define EepRead 0x1

#define EepEraseFlag 0x80
#define EepWriteFlag 0x20
#define EepReadFlag 0x10

extern unsigned char EEPBuf;

extern void EEP_Init (unsigned char Add);
extern void EEP_Erase (void);
extern void EEP_Write (unsigned char Add,unsigned char Data);
extern unsigned char EEP_Read (unsigned char Add);
#endif