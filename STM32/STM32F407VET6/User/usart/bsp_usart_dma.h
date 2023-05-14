#ifndef __BSP_USART_DMA_H
#define __BSP_USART_DMA_H

#include "stm32f4xx.h"
#include <stdio.h>

/* USART�궨�� */
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

/* DMA �궨�� */
#define USARTx_DR_BASE      (USART1_BASE+0x04)   // +0x04��USART�����ݼĴ�����ַ
#define USARTx_DMA_CLK      RCC_AHB1Periph_DMA2
#define USARTx_DMA_STREAM   DMA2_Stream7
#define USARTx_DMA_CHANNEL  DMA_Channel_4
#define SEND_BUF_SIZE       10 // һ�η��͵�������
#define TIMEOUT_MAX         10000

// DMA���ú������洢�������裨USART1.DR)
ErrorStatus DMA_Cfg(void);
// �������ú���
void Usart_Cfg(u32 baudrate);

// ����һ���ַ�
static void Usart_SendByte(USART_TypeDef* pUsart, u8 ch);
// �����ַ�������, len==0ʱ����str�ַ�����\0��β��
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len);

// buf
extern u8 SendBuf[SEND_BUF_SIZE];


#endif