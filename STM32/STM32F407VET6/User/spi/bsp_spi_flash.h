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
#define FALSH_CS_PIN                GPIO_Pin_3
#define FLASH_CS_GPIO_PORT          GPIOE
#define FLASH_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE









#endif