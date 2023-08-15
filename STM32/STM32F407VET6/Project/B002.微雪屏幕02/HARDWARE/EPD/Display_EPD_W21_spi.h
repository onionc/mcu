#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "stm32f4xx.h"


#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOC, GPIO_Pin_3)
#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOC, GPIO_Pin_3)

#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define EPD_W21_CLK_1	GPIO_SetBits(GPIOB, GPIO_Pin_10)

#define EPD_W21_CS_0	GPIO_ResetBits(GPIOE, GPIO_Pin_7)
#define EPD_W21_CS_1	GPIO_SetBits(GPIOE, GPIO_Pin_7)

#define EPD_W21_DC_0	GPIO_ResetBits(GPIOE, GPIO_Pin_8)
#define EPD_W21_DC_1	GPIO_SetBits(GPIOE, GPIO_Pin_8)

#define EPD_W21_RST_0	GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define EPD_W21_RST_1	GPIO_SetBits(GPIOC, GPIO_Pin_2)

#define isEPD_W21_BUSY GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) // for solomen solutions


void SPI_Write(unsigned char value);
void Epaper_Write_Data(unsigned char data);
void Epaper_Write_Command(unsigned char command);
void EPD_GPIO_Init(void);

#endif  //#ifndef _MCU_SPI_H_

/***********************************************************
						end file
***********************************************************/
