// ����

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f4xx.h"

// ���Ŷ���
#define KEY1_PIN        GPIO_Pin_10
#define KEY2_PIN        GPIO_Pin_11
#define KEY3_PIN        GPIO_Pin_12

#define KEY1_GPIO_PORT  GPIOE
#define KEY2_GPIO_PORT  GPIOE
#define KEY3_GPIO_PORT  GPIOE

#define KEY1_GPIO_CLK   RCC_AHB1Periph_GPIOE
#define KEY2_GPIO_CLK   RCC_AHB1Periph_GPIOE
#define KEY3_GPIO_CLK   RCC_AHB1Periph_GPIOE

// �������¡�����Ϊ�ߵ�ƽʱ ON=1 OFF=0 �͵�ƽ�෴
#define KEY_ON  0
#define KEY_OFF 1

// ����GPIO����
void Key_GPIO_Config(void);

// ����Ƿ��а�������
uint8_t Key_Scan(GPIO_TypeDef* gpiox, uint16_t pin);

#endif