#ifndef _IIC_H
#define _IIC_H

#include <regx52.h>

// IIC ��������
sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void iic_delay(); // iic�ļ��

// IIC ��������
void iic_start(); // ���Ϳ�ʼ�ź�
void iic_stop(); // ����ֹͣ�ź�
void iic_ack(); // ���� ACK �ź�
void iic_nack(); // ���� no Ack �ź�
u8 iic_wait_ack(); // �ȴ�ACK�ź�

void iic_write_byte(u8 txd); // ����һ���ֽ�
u8 iic_read_byte(u8 ack); // ��ȡһ���ֽ�

u8 iic_write_mem(u8 devAddr, u8 addr, u8 *pData, u16 len); // ��ָ�������ļĴ���ֵ��д��
u8 iic_read_mem(u8 devAddr, u8 addr, u8 *pBuf, u16 len); // ��ָ�������ļĴ���ֵ�Ķ�ȡ
u8 iic_write_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 data); // д��8λ�Ĵ�����λ��1���λ��
u8 iic_read_bits(u8 devAddr, u8 addr, u8 start, u8 *pBuf); // ��ȡ��λ



#endif