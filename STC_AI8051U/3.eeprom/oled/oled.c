#include "oled.h"
#include "oledfont.h"



void Write_OLED_Commands(u8 *bytes, u8 len)
{
    u8 i;
    iic_start();
    iic_write_byte(OLED_ADDRESS);
    if(iic_wait_ack())
        return ;
    iic_write_byte(OLED_CMD);
    if(iic_wait_ack())
        return ;
    // 写入一个字节
    // iic_write_byte(command);
    // iic_wait_ack();
    
    // 写入多个字节
    for(i=0; i<len; i++){
        iic_write_byte(bytes[i]);
        if(iic_wait_ack())
            return ;
    }
    iic_stop();
}

void Write_OLED_Datas(u8 *bytes, u8 len)
{
    u8 i;
    iic_start();
    iic_write_byte(OLED_ADDRESS);
    if(iic_wait_ack())
        return ;
    iic_write_byte(OLED_DATA);
    if(iic_wait_ack())
        return ;

    
    // 写入多个字节
    for(i=0; i<len; i++){
        iic_write_byte(bytes[i]);
        if(iic_wait_ack())
            return ;
    }
    iic_stop();
}
void Write_OLED_Data(u8 dat){
    u8 bytes[1];
    bytes[0] = dat;
    Write_OLED_Datas(bytes, 1);
}
void Write_OLED_Command(u8 dat){
    u8 bytes[1];
    bytes[0] = dat;
    Write_OLED_Commands(bytes, 1);
}

//初始化SSD1306
void OLED_Init(void)
{
    
    
    u8 cmd_bytes[] = {
        0xAE,   //--display off
        0x00,    //---set low column address
        0x10,    //---set high column address
        0x40,    //--set start line address
        0xB0,    //--set page address
        0x81,    // contract control
        0xFF,    //--128
        0xA1,    //set segment remap
        0xA6,    //--normal / reverse
        0xA8,    //--set multiplex ratio(1 to 64)
        0x3F,    //--1/32 duty
        0xC8,    //Com scan direction
        0xD3,    //-set display offset
        0x00,    //
        0xD5,    //set osc division
        0x80,    //
        0xD8,    //set area color mode off
        0x05,    //
        0xD9,    //Set Pre-Charge Period
        0xF1,    //
        0xDA,    //set com pin configuartion
        0x12,    //
        0xDB,    //set Vcomh
        0x30,    //
        0x8D,    //set charge pump enable
        0x14,    //
        0xAF    //--turn on oled panel
    };
    
    // iic 初始化
    iic_init();
    
    // 初始化命令
    Write_OLED_Commands(cmd_bytes, sizeof(cmd_bytes));
    
    /*
    还有个简洁版本
    0xAE, // 关闭屏幕
    0xC8, // 行重映射
    0xA1, // 列重映射
    0x8D,0x14,    // 电荷泵
    0xAF,   // 开启屏幕
    */
    
    
    
    
}


//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y)
{ 
    Write_OLED_Command(0xb0+y);
    Write_OLED_Command(((x&0xf0)>>4)|0x10);
    Write_OLED_Command(x&0x0f);
}
//开启OLED显示
void OLED_Display_On(void)
{
     
    Write_OLED_Command(0X8D); //SET DCDC命令
    Write_OLED_Command(0X14); //DCDC ON
    Write_OLED_Command(0XAF); //DISPLAY ON
   
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    Write_OLED_Command(0X8D); //SET DCDC命令
    Write_OLED_Command(0X10); //DCDC OFF
    Write_OLED_Command(0XAE); //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        Write_OLED_Command(0xb0+i);    //设置页地址（0~7）
        Write_OLED_Command(0x00);      //设置显示位置—列低地址
        Write_OLED_Command(0x10);      //设置显示位置—列高地址
        for(n=0;n<128;n++)Write_OLED_Data(0);
    } //更新显示
}
void OLED_On(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        Write_OLED_Command(0xb0+i);    //设置页地址（0~7）
        Write_OLED_Command(0x00);      //设置显示位置—列低地址
        Write_OLED_Command(0x10);      //设置显示位置—列高地址
        for(n=0;n<128;n++)Write_OLED_Data(1);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
        c=chr-' ';//得到偏移后的值
        if(x>Max_Column-1){x=0;y=y+2;}
        if(Char_Size ==16)
            {
            OLED_Set_Pos(x,y);
            for(i=0;i<8;i++)
            Write_OLED_Data(F8X16[c*16+i]);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            Write_OLED_Data(F8X16[c*16+i+8]);
            }
            else {
                OLED_Set_Pos(x,y);
                for(i=0;i<6;i++)
                Write_OLED_Data(F6x8[c][i]);

            }
}
//显示一个字符串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {       OLED_ShowChar(x,y,chr[j],Char_Size);
            x+=8;
        if(x>120){x=0;y+=2;}
            j++;
    }
}
/*
//显示汉字
void OLED_ShowZh(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
    {
        Write_OLED_Data(Hzk[2*no][t]);
        adder+=1;
    }
    OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
    {
        Write_OLED_Data(Hzk[2*no+1][t]);
        adder+=1;
    }
}
*/

/*
void OLED_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

  if(y1%8==0)
    {
        y=y1/8;
    }
  else
        y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}

void SHOW_FU(unsigned char x0, unsigned char y0){
    char i=0;
    for(;i<8;i++){
        OLED_BMP(x0,y0,32,4, gImage_fu[i]);
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    }
}*/