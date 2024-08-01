
#include "oled.h"

/**
 * OLED���Դ�
 * ��Ÿ�ʽ����.
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
 */
__align(4) uint8_t OLED_GRAM[128][8];	 

// ��SSD1306д��һ���ֽ�
// dat:Ҫд�������/����
// cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i,r;
    OLED_DC(cmd); //д���� 
    OLED_CS(0);
    
    HAL_StatusTypeDef errorcode = HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
    
    /*
    for(i=0;i<8;i++)
    {
        OLED_SCLK(0);
        if(dat&0x80)OLED_SDIN(1);
        else OLED_SDIN(0);
        OLED_SCLK(1);
        dat<<=1;   
    }*/
    
    OLED_CS(1);
    OLED_DC(1);
    
} 

//��������
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0xff;  
	OLED_Refresh_Gram();//������ʾ
}

//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

// SSD1306 ��ʼ��
void OLED_Init(void)
{
    /*
	OLED_SDIN(1);
    OLED_SCLK(1);
*/
    
    //test
    OLED_DC(0);
    OLED_CS(0);    
    HAL_Delay(10);
    
    OLED_CS(1);
    OLED_DC(1);	 
    HAL_Delay(5);

    OLED_RST(0);
    HAL_Delay(100);

    OLED_RST(1); 
                      
    OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
    
    OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
    OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
     OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
    OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
    OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
    OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

    OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
                                                        
    OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
    OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
    OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
    OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
    OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
         
    OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
    OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
    OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
    OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
    OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ
    OLED_Clear();
}  