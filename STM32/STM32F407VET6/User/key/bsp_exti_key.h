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
 * @brief 配置 EXTI 中断
- 配置 NVIC
- 开启按键GPIO时钟和SYSCFG时钟
- 配置按键GPIO为输入模式
- 将按键GPIO连接到EXTI输入源
- 配置按键EXTI中断
 */
void EXTI_Key_Cfg(void);
#endif
