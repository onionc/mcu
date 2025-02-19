#include "spi.h"



// SS
void SPI_W_SS(bit BitValue)
{
    CS = BitValue;
}
// SCK
void SPI_W_SCK(bit BitValue)
{
    SCK = BitValue;
}
// MOSI
void SPI_W_MOSI(bit BitValue)
{
    MOSI = BitValue;
}
//读取 MISO
bit SPI_R_MISO(void)
{
    return MISO;
}


void SPI_Init(void)
{
    // 输出口推挽输出模式 (PxM0=1, PxM1=0)
    // CS（P3.5） SCK（P3.2） MOSI（P3.4） 三个口配置
    P3M0 |= 0x34; P3M1 &= ~0x34; 

    // 输入口上拉输入，配置为准双向模式 （PxM0=0, PxM1=0）
    // MISO （P3.3）
    P3M0 &= ~0x08; P3M1 &= ~0x08; 


    //默认电平
    SPI_W_SS(1);    //从机默认高电平
    SPI_W_SCK(0);   //时钟线默认低电平
}


// 开始通讯，CS拉低
void SPI_Start(void){
    SPI_W_SS(0);
}

void SPI_Stop(void){
    SPI_W_SS(1);
}


//交换数据
u8 SPI_SwapByte(u8 ByteSend)
{
    u8 i,ByteReceive = 0x00;

    for (i = 0;i < 8;i++){
        SPI_W_MOSI(ByteSend  & (0x80 >> i));
        SPI_W_SCK(1);
        if(SPI_R_MISO() == 1){
            ByteReceive |= 0x80 >> i;
        }
        SPI_W_SCK(0);
    }
    return ByteReceive;
}