#ifndef _MPU6050_H
#define _MPU6050_H

#include <regx52.h>
#include "../public/uart/uart.h"
#include "../public/iic/iic.h"

#define MPU_ADDR 0x68
#define MPU_PWR_MGMT1_REG		0X6B	//��Դ����Ĵ���1
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2

#include "MPU6050.h"

// --------------------- mpu6050 ��ȡ��д���װ -----------------------

// delay
void mpu_delay_ms(u16 ms);

// iic ����д��
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf);

// iic ������ȡ
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf);

// iic дһ���ֽ�
u8 mpu_write_byte(u8 reg, u8 dat);

// iic ��һ���ֽڣ��������ݻ�0����Ч��
u8 mpu_read_byte(u8 reg);

// --------------------- mpu6050 -----------------------

#endif