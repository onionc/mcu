#ifndef _MPU6050_H
#define _MPU6050_H

#include <regx52.h>
#include "../public/uart/uart.h"
#include "../public/iic/iic.h"

#define MPU_ADDR 0x68
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2

#include "MPU6050.h"

// --------------------- mpu6050 读取和写入封装 -----------------------

// delay
void mpu_delay_ms(u16 ms);

// iic 连续写入
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf);

// iic 连续读取
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf);

// iic 写一个字节
u8 mpu_write_byte(u8 reg, u8 dat);

// iic 读一个字节，返回数据或0（无效）
u8 mpu_read_byte(u8 reg);

// --------------------- mpu6050 -----------------------

#endif