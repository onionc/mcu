/**
 * ILI394V TFT LCD 驱动
 */

#include "main.h"
#include "bsp_ILI3941V_tftlcd.h"

// 向 LCD 写入命令
void LcdWriteCmd(uint16_t ucmd){
    *(__IO uint16_t *) (FSMC_ADDR_CMD) = ucmd;
}

// 向 LCD 写入数据
void LcdWriteData(uint16_t udata){
    *(__IO uint16_t *) (FSMC_ADDR_DATA) = udata;
}

void write_comm8(uint8_t ucmd){
    *(__IO uint8_t *) (FSMC_ADDR_CMD) = ucmd;
}

void write_data8(uint8_t udata){
    *(__IO uint8_t *) (FSMC_ADDR_DATA) = udata;
}

// 初始化
void LCD_init(void)
{ 
    LED_RST(1);    
    HAL_Delay(100);
    LED_RST(0);
    HAL_Delay(200);
    LED_RST(1);
    HAL_Delay(300);
 
    //************* Start Initial Sequence **********//
    write_comm8(0x11); //Exit Sleep
    HAL_Delay(60);
    write_comm8(0xcf); 
    /*
    write_comm16(0x5555);
    write_comm16(0xAAAA);
    write_comm16(0x5555);
    write_comm8(0xcf); 
    */
    write_data8(0x00);
    write_data8(0x99);
    write_data8(0x30);
     //test
    write_data8(0xed); 
    write_data8(0x64);
    write_data8(0x03);
    write_data8(0x12);
    write_data8(0x81);
    write_comm8(0xcb); 
    write_data8(0x39);
    write_data8(0x2c);
    write_data8(0x00);
    write_data8(0x34);
    write_data8(0x02);
    write_comm8(0xea); 
    write_data8(0x00);
    write_data8(0x00);
    write_comm8(0xe8); 
    write_data8(0x85);
    write_data8(0x00);
    write_data8(0x78);

    write_comm8(0xC0); //Power control
    write_data8(0x23); //VRH[5:0]

    write_comm8(0xC1); //Power control
    write_data8(0x12); //SAP[2:0];BT[3:0]

    write_comm8(0xC2);
    write_data8(0x11);

    write_comm8(0xC5); //VCM control
    write_data8(0x40);
    write_data8(0x30);

    write_comm8(0xc7); 
    write_data8(0xa9); //0xb6

    write_comm8(0x3A); 
    write_data8(0x55);

    write_comm8(0x36); // Memory Access Control
    write_data8(0x48);

    write_comm8(0xB1); // Frame Rate Control
    write_data8(0x00);
    write_data8(0x18);

    write_comm8(0xB6); // Display Function Control
    write_data8(0x0a);
    write_data8(0xa2);

    write_comm8(0xF2); // 3Gamma Function Disable
    write_data8(0x00);

    write_comm8(0xF7);
    write_data8(0x20);

    write_comm8(0x26); //Gamma curve selected
    write_data8(0x01);

    write_comm8(0xE0); //Set Gamma
    write_data8(0x1f);
    write_data8(0x24);
    write_data8(0x23);
    write_data8(0x0b);
    write_data8(0x0f);
    write_data8(0x08);
    write_data8(0x50);
    write_data8(0xd8);
    write_data8(0x3b);
    write_data8(0x08);
    write_data8(0x0a);
    write_data8(0x00);
    write_data8(0x00);
    write_data8(0x00);
    write_data8(0x00);

    write_comm8(0XE1); //Set Gamma
    write_data8(0x00);
    write_data8(0x1b);
    write_data8(0x1c);
    write_data8(0x04);
    write_data8(0x10);
    write_data8(0x07);
    write_data8(0x2f);
    write_data8(0x27);
    write_data8(0x44);
    write_data8(0x07);
    write_data8(0x15);
    write_data8(0x0f);
    write_data8(0x3f);
    write_data8(0x3f);
    write_data8(0x1F);
    write_comm8(0x29); //display on
}

/**
* @brief  在ILI9806G显示器上开辟一个窗口
* @param  usX ：在特定扫描方向下窗口的起点X坐标
* @param  usY ：在特定扫描方向下窗口的起点Y坐标
* @param  usWidth ：窗口的宽度
* @param  usHeight ：窗口的高度
* @retval 无
*/
void OpenWindow ( uint16_t usX, uint16_t usY,
uint16_t usWidth, uint16_t usHeight )
{
    write_comm8 ( 0x2A );          /* 设置X坐标 */
    write_data8 ( usX >> 8  );  /* 先高8位，然后低8位 */
    write_data8 ( usX & 0xff  );  /* 设置起始点和结束点*/
    write_data8 ( ( usX + usWidth - 1 ) >> 8  );
    write_data8 ( ( usX + usWidth - 1 ) & 0xff  );

    write_comm8 ( 0x2B );            /* 设置Y坐标*/
    write_data8 ( usY >> 8  );
    write_data8 ( usY & 0xff  );
    write_data8( ( usY + usHeight - 1 ) >> 8 );
    write_data8 ( ( usY + usHeight - 1) & 0xff );
}

// 全屏区域设置 240*320
void areaset(void)
{
write_comm8(0x2A);   
write_data8(0x00);
write_data8(0x00);
write_data8(0x00);
write_data8(0xEF);
 
write_comm8(0x2B);   
write_data8(0x00);
write_data8(0x00);
write_data8(0x01);
write_data8(0x3F);
       
write_comm8(0x2C);
 }

/**
* @brief  在ILI9806G显示器上以某一颜色填充像素点
* @param  ulAmout_Point ：要填充颜色的像素点的总数目
* @param  usColor ：颜色
* @retval 无
*/
void  FillColor ( uint32_t ulAmout_Point, uint16_t usColor )
{
    uint32_t i = 0;

    /* memory write */
    write_comm8 ( 0x2C );

    for ( i = 0; i < ulAmout_Point; i ++ )
        write_data8 ( usColor );
}


void show_color()
{
 int i,j,k,n;
 unsigned char picH,picL;
 areaset();
 for(n=0;n<4;n++)
  { k=0;
   for(i=0;i<240;i++)
   for(j=0;j<80;j++)
    {
     write_data16(0xf800);
     }
   }
}
