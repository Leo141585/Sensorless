#ifndef UART_SYSTEM_H
#define UART_SYSTEM_H

enum TXStatus{TX_SEND, TX_FINISH};
enum RXStatus{RX_INT, RX_HANDLE, RX_FINISH};

typedef xdata unsigned char xuint8_t;
typedef xdata unsigned short xuint16_t;
typedef xdata signed short xint16_t;

extern xint16_t Debug_A;
extern xint16_t Debug_B;
extern xint16_t Debug_C;
extern xint16_t Debug_D;
extern xint16_t Debug_E;
extern xint16_t Debug_F;

extern xuint8_t TxFinish;
extern xuint8_t RxState;
extern xuint8_t Debug_TxCounter;
extern xuint8_t Debug_RxCounter;
extern xuint8_t Debug_PreRxCounter;
extern xuint8_t Debug_TxBuffer[20];
extern xuint8_t Debug_RxBuffer[20];
extern xuint8_t Debug_TxDataLen;

//extern unsigned short CheckSUM;

extern void Uart_Definition(void);
extern void Uart_Package_Build(void);
extern void Uart_ReadPackage_Build(void);
extern void Uart_Read_Handle(void);
extern void Uart_Send_Buffer(void);
extern void Uart_Read_Buffer(void);
extern void Uart_CheckSum_Fun(void);
#endif