#ifndef __OLED_H__
#define __OLED_H__


#include "spi.h"

/****
CS      PA4
RES     PC1
D0      PA5 (clk/SCLK) 
D1      PA7 (mosi/SDIN) 
DC      PC2 ������0/����1 ��ʶ��


****/


#define OLED_CS(n)  (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET))
#define OLED_RST(n) (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET))
#define OLED_DC(n)  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET))

//#define OLED_SCLK(n) (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)) 
//#define OLED_SDIN(n) (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET))

#define OLED_CMD    0   //д����
#define OLED_DATA   1   //д����

// ��SSD1306д��һ���ֽ�
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);

//��������
void OLED_Clear(void);

//�����Դ浽LCD
void OLED_Refresh_Gram(void);

// SSD1306 ��ʼ��
void OLED_Init(void);

/**
 * ��ʾ�ַ���
 * x,y:�������  
 * size:�����С 
 * *p:�ַ�����ʼ��ַ 
 */
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);


/**
 ��ָ��λ����ʾһ���ַ�,���������ַ�
 * x:0~127
 * y:0~63
 * mode:0,������ʾ;1,������ʾ
 * size:ѡ������ 12 
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);

/**
 * ���� 
 * x:0~127
 * y:0~63
 * t:1 ��� 0,���
 */
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);

//12*12 ASCII�ַ�������
extern unsigned char asc2_1206[95][12];

#endif
