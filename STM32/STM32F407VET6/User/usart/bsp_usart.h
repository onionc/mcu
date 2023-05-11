#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

#define USARTx          USART1

/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ��
 * ����1��6��      APB2
 * ����2/3/4/5��   APB1
 * �����ú궨���ʾ
 */
#define USARTx_CLK          RCC_APB2Periph_USART1
#define USARTx_CLOCK_CMD    RCC_APB2PeriphClockCmd

#define USARTx_RX_GPIO_PORT GPIOA
#define USARTx_RX_GPIO_CLK  RCC_AHB1Periph_GPIOA
#define USARTx_RX_PIN       GPIO_Pin_10
#define USARTx_RX_AF        GPIO_AF_USART1
#define USARTx_RX_SOURCE    GPIO_PinSource10

#define USARTx_TX_GPIO_PORT GPIOA
#define USARTx_TX_GPIO_CLK  RCC_AHB1Periph_GPIOA
#define USARTx_TX_PIN       GPIO_Pin_9
#define USARTx_TX_AF        GPIO_AF_USART1
#define USARTx_TX_SOURCE    GPIO_PinSource9

#define USARTx_IRQHandler   USART1_IRQHandler
#define USARTx_IRQ          USART1_IRQn

// �������ú���
void Usart_Cfg(u32 baudrate);
// ����һ���ַ�
void Usart_SendByte(USART_TypeDef* pUsart, u8 ch);
// �����ַ�������, len==0ʱ����str�ַ�����\0��β��
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len);

// ��־λ
extern u8 RxFlag;
extern u8 chTemp;

#endif