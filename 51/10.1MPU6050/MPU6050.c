#include "MPU6050.h"

// delay
void mpu_delay_ms(u16 ms){
    delay1ms(ms);
}

// iic 连续写入
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf){
    return iic_write_mem(MPU_ADDR, reg, pBuf, len);
}

// iic 连续读取
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf){
    return iic_read_mem(MPU_ADDR, reg, pBuf, len);
}

// iic 写一个字节
u8 mpu_write_byte(u8 reg, u8 dat){
    return iic_write_mem(MPU_ADDR, reg, &dat, 1);
}

// iic 读一个字节，返回数据或0（无效）
u8 mpu_read_byte(u8 reg){
    u8 r;
    if(iic_read_mem(MPU_ADDR, reg, &r, 1)==0){
        return r;
    }
    return 0;
}

// mpu初始化，返回0成功
u8 mpu_init(){
    u8 r;
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x80); // 复位 MPU6050
    mpu_delay_ms(100);
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x00); // 唤醒 MPU6050

    return 1;
}
