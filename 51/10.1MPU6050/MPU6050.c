#include "MPU6050.h"

// delay
void mpu_delay_ms(u16 ms){
    delay1ms(ms);
}

// iic ����д��
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf){
    return iic_write_mem(MPU_ADDR, reg, pBuf, len);
}

// iic ������ȡ
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf){
    return iic_read_mem(MPU_ADDR, reg, pBuf, len);
}

// iic дһ���ֽ�
u8 mpu_write_byte(u8 reg, u8 dat){
    return iic_write_mem(MPU_ADDR, reg, &dat, 1);
}

// iic ��һ���ֽڣ��������ݻ�0����Ч��
u8 mpu_read_byte(u8 reg){
    u8 r;
    if(iic_read_mem(MPU_ADDR, reg, &r, 1)==0){
        return r;
    }
    return 0;
}

// mpu��ʼ��������0�ɹ�
u8 mpu_init(){
    u8 r;
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x80); // ��λ MPU6050
    mpu_delay_ms(100);
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x00); // ���� MPU6050

    return 1;
}
