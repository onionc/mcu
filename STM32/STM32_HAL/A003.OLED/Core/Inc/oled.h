#ifndef __OLED_H__
#define __OLED_H__


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

/**
 * 显示字符串
 * x,y:起点坐标  
 * size:字体大小 
 * *p:字符串起始地址 
 */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);


/**
 在指定位置显示一个字符,包括部分字符
 * x:0~127
 * y:0~63
 * mode:0,反白显示;1,正常显示
 * size:选择字体 12 
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);

/**
 * 画点 
 * x:0~127
 * y:0~63
 * t:1 填充 0,清空
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);

//12*12 ASCII字符集点阵
extern unsigned char asc2_1206[95][12];

#endif
