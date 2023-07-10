#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>



/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ��
 * ����1��6��      APB2
 * ����2/3/4/5��   APB1
 * �����ú궨���ʾ
 */

#define USART1_CLK          RCC_APB2Periph_USART1
#define USART1_CLOCK_CMD    RCC_APB2PeriphClockCmd

#define USART1_RX_GPIO_PORT GPIOA
#define USART1_RX_GPIO_CLK  RCC_AHB1Periph_GPIOA
#define USART1_RX_PIN       GPIO_Pin_10
#define USART1_RX_AF        GPIO_AF_USART1
#define USART1_RX_SOURCE    GPIO_PinSource10

#define USART1_TX_GPIO_PORT GPIOA
#define USART1_TX_GPIO_CLK  RCC_AHB1Periph_GPIOA
#define USART1_TX_PIN       GPIO_Pin_9
#define USART1_TX_AF        GPIO_AF_USART1
#define USART1_TX_SOURCE    GPIO_PinSource9

#define USART3_CLK          RCC_APB1Periph_USART3
#define USART3_CLOCK_CMD    RCC_APB1PeriphClockCmd

#define USART3_RX_GPIO_PORT GPIOD
#define USART3_RX_GPIO_CLK  RCC_AHB1Periph_GPIOD
#define USART3_RX_PIN       GPIO_Pin_9
#define USART3_RX_AF        GPIO_AF_USART3
#define USART3_RX_SOURCE    GPIO_PinSource9

#define USART3_TX_GPIO_PORT GPIOD
#define USART3_TX_GPIO_CLK  RCC_AHB1Periph_GPIOD
#define USART3_TX_PIN       GPIO_Pin_8
#define USART3_TX_AF        GPIO_AF_USART3
#define USART3_TX_SOURCE    GPIO_PinSource8


#ifdef PRINT_USART1
    #define PRINT_USART USART1
#elif PRINT_USART3
    #define PRINT_USART USART3
#else 
    #error please set usart number by output
#endif

// �������ú���
void Usart1_Cfg(u32 baudrate);
void Usart3_Cfg(u32 baudrate);
// ����һ���ַ�
void Usart_SendByte(USART_TypeDef* pUsart, u8 ch);
// �����ַ�������, len==0ʱ����str�ַ�����\0��β��
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len);

#endif
