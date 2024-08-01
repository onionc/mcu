#ifndef __OLED_H__
#define __OLED_H__

#include "main.h"
#include "spi.h"

/****
CS      PA4
RES     PC1
D0      PA5 (clk/SCLK) 
D1      PA7 (mosi/SDIN) 
DC      PC2 （命令0/数据1 标识）


****/


#define OLED_CS(n)  (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET))
#define OLED_RST(n) (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET))
#define OLED_DC(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET))

//#define OLED_SCLK(n) (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)) 
//#define OLED_SDIN(n) (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET))

#define OLED_CMD    0   //写命令
#define OLED_DATA   1   //写数据

// 向SSD1306写入一个字节
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);

//清屏函数
void OLED_Clear(void);

//更新显存到LCD
void OLED_Refresh_Gram(void);

// SSD1306 初始化
void OLED_Init(void);

#endif