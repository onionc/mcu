
#include "oled.h"

/**
 * OLED的显存
 * 存放格式如下.
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

// 向SSD1306写入一个字节
// dat:要写入的数据/命令
// cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
    uint8_t i,r;
    OLED_DC(cmd); //写命令 
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

//清屏函数
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0xff;  
	OLED_Refresh_Gram();//更新显示
}

//更新显存到LCD
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

// SSD1306 初始化
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
                      
    OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
    
    OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
    OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
     OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
    OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
    OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
    OLED_WR_Byte(0X00,OLED_CMD); //默认为0

    OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
                                                        
    OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
    OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
    OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
    OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
    OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
         
    OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
    OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
    OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
    OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WR_Byte(0xAF,OLED_CMD); //开启显示
    OLED_Clear();
}  