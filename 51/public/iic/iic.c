#include "iic.h"
#include "../common.h"

void iic_delay(){
    delay10us();
}

// 发送开始信号。scl为高电平时，SDA由高到低跳变
void iic_start(){
    IIC_SDA = 1;
    IIC_SCL = 1;
    iic_delay();
    
    IIC_SDA = 0;
    iic_delay();
    IIC_SCL = 0;
}

// 发送停止信号。 scl为高电平时，SDA由低到高跳变
void iic_stop(){
    IIC_SDA = 0;
    IIC_SCL = 1;
    iic_delay();
    
    IIC_SDA = 1;
    iic_delay();
}

// 发送 ACK 信号。SCL由低到高再低时，SDA为低电平
void iic_ack(){
    IIC_SCL = 0;
    IIC_SDA = 0;
    iic_delay();

    IIC_SCL = 1;
    iic_delay();
    IIC_SCL = 0;
    iic_delay();
}

// 发送 no Ack 信号。SCL由低到高再低时，SDA为高电平
void iic_nack(){
    IIC_SCL = 0;
    IIC_SDA = 1;
    iic_delay();

    IIC_SCL = 1;
    iic_delay();
    IIC_SCL = 0;
    iic_delay();
}

// 等待ACK信号
u8 iic_wait_ack(){
    u8 timeTemp = 0;

    IIC_SCL = 1;
    while(IIC_SDA){ // 等待SDA为低电平
        if(timeTemp++>120){
            iic_stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

// 发送一个字节。数据有效性：SCL为高电平期间，SDA数据必须稳定
void iic_write_byte(u8 txd){
    u8 i = 0;

    IIC_SCL = 0;
    for(i=0; i<8; i++){
        if(txd&0x80){ // 发送最高位
            IIC_SDA = 1;
        }else{
            IIC_SDA = 0;
        }

        txd <<= 1;
        iic_delay();
        IIC_SCL = 1;
        iic_delay();
        IIC_SCL = 0;
        iic_delay();
    }
}

/**
 * @brief 读取一个字节
 * 
 * @param ack ack不为0 发送ack, 或者发送 nack
 * @return u8 
 */
u8 iic_read_byte(u8 ack){
    u8 i=0, r=0;
    for(i=0; i<8; i++){
        IIC_SCL = 0;
        iic_delay();
        IIC_SCL = 1;
        r <<= 1;
        if(IIC_SDA) r++;
        iic_delay();
    }

    if(ack){
        iic_ack();
    }else{
        iic_nack();
    }
    return r;
}

/**
 * @brief 对指定器件的寄存器值的写入
 * 传输数据过程为：S 从机地址 0 A 数据 A 数据 A/noA P
 * @param devAddr 器件地址
 * @param addr 器件寄存器地址
 * @param pData 待输入数据首地址
 * @param len 待输入数据长度
 * @return u8 0成功，1失败
 */
u8 iic_write_mem(u8 devAddr, u8 addr, u8 *pData, u16 len){
    iic_start();
    iic_write_byte(devAddr<<1); // 左移一位，后面填充的0表示主机向从机写数据
    if(iic_wait_ack())
        return 1;
    iic_write_byte(addr);
}

u8 iic_read_mem(u8 devAddr, u8 addr, u8 *pBuf, u16 len); // 对指定器件的寄存器值的读取

u8 iic_write_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 data); // 写入8位寄存器的位（1或多位）
u8 iic_read_bits(u8 devAddr, u8 addr, u8 start, u8 *pBuf); // 读取多位
