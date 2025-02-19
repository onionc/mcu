#ifndef _SPI_H
#define _SPI_H

#include <AI8051U.H>
#include "common.h"

// spi 引脚定义
#define CS      P35
#define SCK     P32
#define MISO    P33
#define MOSI    P34





void SPI_Init(void);
void SPI_Start(void);
void SPI_Stop(void);
u8 SPI_SwapByte(u8 ByteSend);
 
#endif