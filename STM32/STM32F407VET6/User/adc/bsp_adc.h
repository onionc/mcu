#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx.h"

/*
// 使用
ADC_GPIO_Config();
ADC_Mode_Config();
ADC_NVIC_Config();
*/

/*
PC0 IN10
pc2  adc1~3_IN12
pc3  adc1~3_IN13
*/

// ADC GPIO
#define ADC_GPIO_PORT1  GPIOC
#define ADC_GPIO_PIN1   GPIO_Pin_0
#define ADC_GPIO_CLK1   RCC_AHB1Periph_GPIOC
#define ADC_CHANNEL1    ADC_Channel_10

/*
#define ADC_GPIO_PORT2  GPIOC
#define ADC_GPIO_PIN2   GPIO_Pin_3
#define ADC_GPIO_CLK2   RCC_AHB1Periph_GPIOC
#define ADC_CHANNEL2    ADC_Channel_13
*/
// ADC 序号
#define ADC_NUM         ADC1
#define ADC_CLK         RCC_APB2Periph_ADC1

// ADC 中断
#define ADC_IRQ         ADC_IRQn
#define ADC_INT_FUNC    ADC_IRQHandler


// GPIO 初始化
void ADC_GPIO_Config(void);

// ADC 工作模式初始化
void ADC_Mode_Config(void);

// 中断配置
void ADC_NVIC_Config(void);
// adc 中断服务函数
//void ADC_INT_FUNC(void);
void ADC_IRQHandler(void);

#endif