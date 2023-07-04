#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "stm32f4xx.h"

// SPI号和时钟初始化函数
#define FLASH_SPI           SPI2
#define FLASH_SPI_CLK       RCC_APB1Periph_SPI2
#define FLASH_SPI_CLK_INIT  RCC_APB1PeriphClockCmd

// sck 引脚
#define FLASH_SPI_SCK_PIN       GPIO_Pin_10
#define FLASH_SPI_SCK_GPIO_PORT GPIOB
#define FLASH_SPI_SCK_GPIO_CLK  RCC_AHB1Periph_GPIOB
#define FLASH_SPI_SCK_PINSOURCE GPIO_PinSource10
#define FLASH_SPI_SCK_AF        GPIO_AF_SPI2

// miso 引脚
#define FLASH_SPI_MISO_PIN          GPIO_Pin_2
#define FLASH_SPI_MISO_GPIO_PORT    GPIOC
#define FLASH_SPI_MISO_GPIO_CLK     RCC_AHB1Periph_GPIOC
#define FLASH_SPI_MISO_PINSOURCE    GPIO_PinSource2
#define FLASH_SPI_MISO_AF           GPIO_AF_SPI2

// MOSI 引脚
#define FLASH_SPI_MOSI_PIN          GPIO_Pin_3
#define FLASH_SPI_MOSI_GPIO_PORT    GPIOC
#define FLASH_SPI_MOSI_GPIO_CLK     RCC_AHB1Periph_GPIOC
#define FLASH_SPI_MOSI_PINSOURCE    GPIO_PinSource3
#define FLASH_SPI_MOSI_AF           GPIO_AF_SPI2

// CS/NSS 引脚
#define FLASH_CS_PIN                GPIO_Pin_3
#define FLASH_CS_GPIO_PORT          GPIOE
#define FLASH_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE




// 控制CS引脚输出低电平，其他引脚不变
#define SPI_FLASH_CS_LOW()          FLASH_CS_GPIO_PORT->BSRRH=FLASH_CS_PIN;
#define SPI_FLASH_CS_HIGH()         FLASH_CS_GPIO_PORT->BSRRL=FLASH_CS_PIN;

/* 超时时间 */
#define SPIT_FLAG_TIMEOUT ((u32)0x1000)

/* 方法 */
// 检测事件
ErrorStatus SPI_CheckEvent(uint32_t eventState, char *errorMsg);
// SPI FLASH 初始化
void SPI_FLASH_INIT();
// 使用SPI发送一个字节数据
u8 SPI_FLASH_sendByte(u8 byte);
// 使用SPI读取一个字节数据
u8 SPI_FLASH_readByte();


#endif