
#ifndef _IIC_H
#define _IIC_H

#include <AI8051U.H>

typedef unsigned char u8;


// IIC 控制引脚
#define IIC_SCL P20
#define IIC_SDA P21

void iic_init(); // 配置引脚

void iic_delay(); // iic的间隔

// IIC 操作函数
void iic_start(); // 发送开始信号
void iic_stop(); // 发送停止信号
void iic_ack(); // 发送 ACK 信号
void iic_nack(); // 发送 no Ack 信号
u8 iic_wait_ack(); // 等待ACK信号，0成功，1失败

void iic_write_byte(u8 txd); // 发送一个字节
u8 iic_read_byte(u8 ack); // 读取一个字节

#endif