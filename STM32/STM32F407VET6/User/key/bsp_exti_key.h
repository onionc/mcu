#ifndef __BSP_EXTI_KEY_H
#define __BSP_EXTI_KEY_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"



#define KEY1_INT_PIN                GPIO_Pin_10
#define KEY1_INT_CLK                RCC_AHB1Periph_GPIOE
#define KEY1_INT_PORT               GPIOE
#define KEY1_INT_EXTI_PORTSOURCE    EXTI_PortSourceGPIOE
#define KEY1_INT_EXTI_PINSOURCE     EXTI_PinSource10
#define KEY1_INT_EXTI_LINE          EXTI_Line10
#define KEY1_INT_EXTI_IRQ           EXTI15_10_IRQn

#define KEY1_IRQHandler             EXTI15_10_IRQHandler


/**
 * @brief ���� EXTI �ж�
- ���� NVIC
- ��������GPIOʱ�Ӻ�SYSCFGʱ��
- ���ð���GPIOΪ����ģʽ
- ������GPIO���ӵ�EXTI����Դ
- ���ð���EXTI�ж�
 */
void EXTI_Key_Cfg(void);
#endif
