#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f4xx.h"

extern vu32 TimingDelay;

// 启动系统定时器 SysTick. 
ErrorStatus SysTick_Init(void);

// 延时程序，（以SysTick_Init函数设定的时间为一个单位，现在是10us）
void Delay_10us(vu32 times);
#endif
