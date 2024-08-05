#ifndef __BSP_ILI3941V_LCD__
#define __BSP_ILI3941V_LCD__

#include "main.h"

/**
 * 引脚定义
 *
 *  RST     PE4
 *
 */

#define LED_RST(n) (n?HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET));


// 用于LCD命令操作的地址
#define FSMC_ADDR_CMD       ( (uint32_t) 0x60000000 )

// 用于LCD数据操作的地址
#define FSMC_ADDR_DATA      ( (uint32_t) 0x60080000 ) // A 18   (0x60000000 | (1<<(18+1)))

// 向 LCD 写入命令
 void LcdWriteCmd(uint16_t ucmd);

// 向 LCD 写入数据
void LcdWriteData(uint16_t udata);

//#define write_comm8(x)  LcdWriteCmd(x)
//#define write_data8(x)  LcdWriteData(x)
#define write_comm16(x)  LcdWriteCmd(x)
#define write_data16(x)  LcdWriteData(x)

// 初始化
void LCD_init(void);

// 全屏区域设置 240*320
void fullAreaSet(void);

// 区域设置
void areaSet ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight );

void  FillColor ( uint32_t ulAmout_Point, uint16_t usColor );

void show_color();


#endif

