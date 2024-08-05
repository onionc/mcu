#ifndef __BSP_ILI3941V_LCD__
#define __BSP_ILI3941V_LCD__

#include "main.h"

/**
 * ���Ŷ���
 *
 *  RST     PE4
 *
 */

#define LED_RST(n) (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET));


// ����LCD��������ĵ�ַ
#define FSMC_ADDR_CMD       ( (uint32_t) 0x60000000 )

// ����LCD���ݲ����ĵ�ַ
#define FSMC_ADDR_DATA      ( (uint32_t) 0x60080000 ) // A 18   (0x60000000 | (1<<(18+1)))

// �� LCD д������
 void LcdWriteCmd(uint16_t ucmd);

// �� LCD д������
void LcdWriteData(uint16_t udata);

//#define write_comm8(x)  LcdWriteCmd(x)
//#define write_data8(x)  LcdWriteData(x)
#define write_comm16(x)  LcdWriteCmd(x)
#define write_data16(x)  LcdWriteData(x)

// ��ʼ��
void LCD_init(void);

// ȫ���������� 240*320
void fullAreaSet(void);

// ��������
void areaSet ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );

void  FillColor ( uint32_t ulAmout_Point, uint16_t usColor );

void show_color();


#endif

