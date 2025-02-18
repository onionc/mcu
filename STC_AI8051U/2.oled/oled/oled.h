
#ifndef OLED_OLED_H_
#define OLED_OLED_H_


#include "i2c.h"


#define OLED_ADDRESS    0x78 // oled地址
#define OLED_CMD        0x00  // 命令
#define OLED_DATA       0x40  // 数据

#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64


// 发送数据和命令，发送多个和单个
void Write_OLED_Datas(u8 *bytes, u8 len);
void Write_OLED_Commands(u8 *bytes, u8 len);
void Write_OLED_Data(u8 dat);
void Write_OLED_Command(u8 command);

// 初始化
void OLED_Init(void);
//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y);
//开启OLED显示
void OLED_Display_On(void);
//关闭OLED显示
void OLED_Display_Off(void);
// 清屏和显示
void OLED_Clear(void);
void OLED_On(void);
// 显示一个字符
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
//显示一个字符串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
//显示汉字
void OLED_ShowZh(u8 x,u8 y,u8 no);
// 显示图片
//void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
// 显示旋转的福
//void SHOW_FU(unsigned char x0, unsigned char y0);

#endif /* OLED_OLED_H_ */