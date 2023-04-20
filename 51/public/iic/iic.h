#ifndef _IIC_H
#define _IIC_H

#include <regx52.h>

// IIC 控制引脚
sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void iic_delay(); // iic的间隔

// IIC 操作函数
void iic_start(); // 发送开始信号
void iic_stop(); // 发送停止信号
void iic_ack(); // 发送 ACK 信号
void iic_nack(); // 发送 no Ack 信号
u8 iic_wait_ack(); // 等待ACK信号

void iic_write_byte(u8 txd); // 发送一个字节
u8 iic_read_byte(u8 ack); // 读取一个字节

u8 iic_write_mem(u8 devAddr, u8 addr, u8 *pData, u16 len); // 对指定器件的寄存器值的写入
u8 iic_read_mem(u8 devAddr, u8 addr, u8 *pBuf, u16 len); // 对指定器件的寄存器值的读取
u8 iic_write_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 data); // 写入8位寄存器的位（1或多位）
u8 iic_read_bits(u8 devAddr, u8 addr, u8 start, u8 *pBuf); // 读取多位



#endif