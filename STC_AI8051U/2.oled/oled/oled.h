
#ifndef OLED_OLED_H_
#define OLED_OLED_H_


#include "i2c.h"


#define OLED_ADDRESS    0x78 // oled��ַ
#define OLED_CMD        0x00  // ����
#define OLED_DATA       0x40  // ����

#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64


// �������ݺ�������Ͷ���͵���
void Write_OLED_Datas(u8 *bytes, u8 len);
void Write_OLED_Commands(u8 *bytes, u8 len);
void Write_OLED_Data(u8 dat);
void Write_OLED_Command(u8 command);

// ��ʼ��
void OLED_Init(void);
//��������
void OLED_Set_Pos(unsigned char x, unsigned char y);
//����OLED��ʾ
void OLED_Display_On(void);
//�ر�OLED��ʾ
void OLED_Display_Off(void);
// ��������ʾ
void OLED_Clear(void);
void OLED_On(void);
// ��ʾһ���ַ�
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
//��ʾһ���ַ���
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
//��ʾ����
void OLED_ShowZh(u8 x,u8 y,u8 no);
// ��ʾͼƬ
//void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
// ��ʾ��ת�ĸ�
//void SHOW_FU(unsigned char x0, unsigned char y0);

#endif /* OLED_OLED_H_ */