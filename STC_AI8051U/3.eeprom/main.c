
#include <AI8051U.H>
#include "w25q128.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>


u8 MID;
u16 DID;

void Delay10ms(unsigned int times)	//@11.0592MHz
{
    unsigned char data i, j;
    while(times-->0){
        
        i = 108;
        j = 145;
        do
        {
            while (--j);
        } while (--i);
    }
}


u8 ArrayWrite[] = {0x10,0x12,0xfe,0xef};//��ʼ��д��ı���
u8 ArrayRead[4];//��ʼ����ȡ�ı���



int main(){

    u8 msg[20];

    
    
    // oled
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"flash test", 12);

    // w25q128
    W25Q128_Init();
    Delay10ms(100);
	W25Q128_ReadID(&MID,&DID); // MID����id, DID �洢�����ͺ�����
    sprintf(msg, "JEDEC: %bx, %hx",MID, DID); // ef 4018 // b8λ h16λ
    
    OLED_ShowString(0,1, msg, 12);
    
    
    // ����
    W25Q128_SectorErase(0x000000);
    // д
    W25Q128_PageProgram(0x000000,ArrayWrite,4);
    // ��
    W25Q128_ReadData(0x000000,ArrayRead,4);
    
    sprintf(msg, "%bx %bx %bx %bx", ArrayRead[0], ArrayRead[1], ArrayRead[2], ArrayRead[3]);
    OLED_ShowString(0,3, "read: ", 12);
    OLED_ShowString(0,4, msg, 16);
    
	while(1)
	{
        
    }

    return 0;
}
