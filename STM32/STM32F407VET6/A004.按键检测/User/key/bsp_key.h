// 按键

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx.h"

// 引脚定义
#define KEY1_PIN        GPIO_Pin_10
#define KEY2_PIN        GPIO_Pin_11
#define KEY3_PIN        GPIO_Pin_12

#define KEY1_GPIO_PORT  GPIOE
#define KEY2_GPIO_PORT  GPIOE
#define KEY3_GPIO_PORT  GPIOE

#define KEY1_GPIO_CLK   RCC_AHB1Periph_GPIOE
#define KEY2_GPIO_CLK   RCC_AHB1Periph_GPIOE
#define KEY3_GPIO_CLK   RCC_AHB1Periph_GPIOE

// 按键按下。按下为高电平时 ON=1 OFF=0 低电平相反
#define KEY_ON  0
#define KEY_OFF 1

// 按键GPIO配置
void Key_GPIO_Config(void);

// 检测是否有按键按下
uint8_t Key_Scan(GPIO_TypeDef* gpiox, uint16_t pin);

#endif