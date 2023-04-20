#ifndef __COMMON_H
#define __COMMON_H

#include <intrins.h>
typedef unsigned char u8; 
typedef unsigned short u16;

#define hz 110592 // 时钟频率

void delay10us();
#if hz == 12
#define Delay2us _nop_();_nop_(); // 12hz下1微秒

#elif hz == 110592
#define Delay2us _nop(); // 11.0592下2微秒

#endif

void delay1ms(unsigned int);
void delay10us();


#endif