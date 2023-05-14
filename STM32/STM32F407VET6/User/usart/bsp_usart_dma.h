#ifndef __BSP_USART_DMA_H
#define __BSP_USART_DMA_H

#include "stm32f4xx.h"
#include <stdio.h>

/* USART宏定义 */
#define USARTx          USART1
/* 不同的串口挂载的总线不一样，时钟使能函数也不一样
 * 串口1和6是      APB2
 * 串口2/3/4/5是   APB1
 * 这里用宏定义表示
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

/* DMA 宏定义 */
#define USARTx_DR_BASE      (USART1_BASE+0x04)   // +0x04是USART的数据寄存器地址
#define USARTx_DMA_CLK      RCC_AHB1Periph_DMA2
#define USARTx_DMA_STREAM   DMA2_Stream7
#define USARTx_DMA_CHANNEL  DMA_Channel_4
#define SEND_BUF_SIZE       10 // 一次发送的数据量
#define TIMEOUT_MAX         10000

// DMA配置函数，存储器到外设（USART1.DR)
ErrorStatus DMA_Cfg(void);
// 串口配置函数
void Usart_Cfg(u32 baudrate);

// 发送一个字符
static void Usart_SendByte(USART_TypeDef* pUsart, u8 ch);
// 发送字符串函数, len==0时则发送str字符串（\0结尾）
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len);

// buf
extern u8 SendBuf[SEND_BUF_SIZE];


#endif