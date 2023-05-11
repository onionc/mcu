#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

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

#define USARTx_IRQHandler   USART1_IRQHandler
#define USARTx_IRQ          USART1_IRQn

// 串口配置函数
void Usart_Cfg(u32 baudrate);
// 发送一个字符
void Usart_SendByte(USART_TypeDef* pUsart, u8 ch);
// 发送字符串函数, len==0时则发送str字符串（\0结尾）
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len);

// 标志位
extern u8 RxFlag;
extern u8 chTemp;

#endif