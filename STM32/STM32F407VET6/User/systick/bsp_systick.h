#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f4xx.h"

extern vu32 TimingDelay;

// ����ϵͳ��ʱ�� SysTick. 
ErrorStatus SysTick_Init(void);

// ��ʱ���򣬣���SysTick_Init�����趨��ʱ��Ϊһ����λ��������10us��
void Delay_10us(vu32 times);
#endif
