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
    // д��һ���ֽ�
    // iic_write_byte(command);
    // iic_wait_ack();
    
    // д�����ֽ�
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

    
    // д�����ֽ�
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

//��ʼ��SSD1306
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
    
    // iic ��ʼ��
    iic_init();
    
    // ��ʼ������
    Write_OLED_Commands(cmd_bytes, sizeof(cmd_bytes));
    
    /*
    ���и����汾
    0xAE, // �ر���Ļ
    0xC8, // ����ӳ��
    0xA1, // ����ӳ��
    0x8D,0x14,    // ��ɱ�
    0xAF,   // ������Ļ
    */
    
    
    
    
}


//��������
void OLED_Set_Pos(unsigned char x, unsigned char y)
{ 
    Write_OLED_Command(0xb0+y);
    Write_OLED_Command(((x&0xf0)>>4)|0x10);
    Write_OLED_Command(x&0x0f);
}
//����OLED��ʾ
void OLED_Display_On(void)
{
     
    Write_OLED_Command(0X8D); //SET DCDC����
    Write_OLED_Command(0X14); //DCDC ON
    Write_OLED_Command(0XAF); //DISPLAY ON
   
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    Write_OLED_Command(0X8D); //SET DCDC����
    Write_OLED_Command(0X10); //DCDC OFF
    Write_OLED_Command(0XAE); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        Write_OLED_Command(0xb0+i);    //����ҳ��ַ��0~7��
        Write_OLED_Command(0x00);      //������ʾλ�á��е͵�ַ
        Write_OLED_Command(0x10);      //������ʾλ�á��иߵ�ַ
        for(n=0;n<128;n++)Write_OLED_Data(0);
    } //������ʾ
}
void OLED_On(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        Write_OLED_Command(0xb0+i);    //����ҳ��ַ��0~7��
        Write_OLED_Command(0x00);      //������ʾλ�á��е͵�ַ
        Write_OLED_Command(0x10);      //������ʾλ�á��иߵ�ַ
        for(n=0;n<128;n++)Write_OLED_Data(1);
    } //������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
        c=chr-' ';//�õ�ƫ�ƺ��ֵ
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
//��ʾһ���ַ���
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
//��ʾ����
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