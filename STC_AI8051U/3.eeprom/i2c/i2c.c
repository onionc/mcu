#include "i2c.h"
#include <intrins.h>

// 配置引脚
void iic_init(){

    // P20 P21 开漏模式
    P2M0 |= 0x03; P2M1 |= 0x03; 

    // 开启内部上拉
    P2PU |= 0x03; 

}


void delay10us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 25;
	while (--i);
}


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
    IIC_SDA = 1;
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