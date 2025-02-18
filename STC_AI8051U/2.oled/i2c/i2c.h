
#ifndef _IIC_H
#define _IIC_H

#include <AI8051U.H>

typedef unsigned char u8;


// IIC ��������
#define IIC_SCL P20
#define IIC_SDA P21

void iic_init(); // ��������

void iic_delay(); // iic�ļ��

// IIC ��������
void iic_start(); // ���Ϳ�ʼ�ź�
void iic_stop(); // ����ֹͣ�ź�
void iic_ack(); // ���� ACK �ź�
void iic_nack(); // ���� no Ack �ź�
u8 iic_wait_ack(); // �ȴ�ACK�źţ�0�ɹ���1ʧ��

void iic_write_byte(u8 txd); // ����һ���ֽ�
u8 iic_read_byte(u8 ack); // ��ȡһ���ֽ�

#endif