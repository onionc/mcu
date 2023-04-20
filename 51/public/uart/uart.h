#ifndef __UART_H__
#define __UART_H__

#include <regx52.h>
#include <stdio.h>


extern void Uart_Init(unsigned char baud);

extern void Send_String(unsigned char *p);
extern char putchar(char c); // ��дputchar�Ա�ʹ��printf����ʱ�Ӵ������


#define RX_BUF_MAX 30
extern unsigned char xdata RX_Buffer[RX_BUF_MAX];
extern bit Flag_Receive_OK, Flag_Send_Busy;




#endif