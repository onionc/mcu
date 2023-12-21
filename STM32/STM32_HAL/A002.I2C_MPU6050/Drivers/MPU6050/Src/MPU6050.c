#include "MPU6050.h"


// delay
void mpu_delay_ms(u16 ms){
    HAL_Delay(ms);
}

// iic ����д��
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Write(&hi2c1, MPU_WRITE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, I2C_TIME_OUT) == HAL_OK;
}

// iic ������ȡ
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Read(&hi2c1, MPU_READ_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, I2C_TIME_OUT) == HAL_OK;
}

// iic дһ���ֽ�
u8 mpu_write_byte(u8 reg, u8 dat){
    return mpu_write_len(reg, 1, &dat);
}

// iic ��һ���ֽڣ��������ݻ�0����Ч��
u8 mpu_read_byte(u8 reg){
    u8 data=0;
    mpu_read_len(reg, 1, &data);
    return data;
}

/**
 * @brief ���� MPU6050 ���ݴ����������̷�Χ
 * 
 * @param fsr 0 ��250dps, 1 ��500dps, 2 ��1000dps, 3 ��2000dps
 * @return u8 0 success
 */
u8 mpu_set_gyro_fsr(u8 fsr){
    return mpu_write_byte(MPU_GYRO_CFG_REG, fsr<<3);
}

/**
 * @brief ���� MPU6050 ���ٶȴ����������̷�Χ
 * 
 * @param fsr 0 ��2g, 1 ��4g, 2 ��8g, 3 ��16g
 * @return u8 0 success
 */
u8 mpu_set_accel_fsr(u8 fsr){
    return mpu_write_byte(MPU_ACCEL_CFG_REG, fsr<<3);
}

/**
 * @brief ���� MPU6050 �����ֵ�ͨ�˲���
 * 
 * @param lpf ���ֵ�ͨ�˲��� Hz (260 184 94 44 21 10 5)
 * @return u8 0 success
 */
u8 mpu_set_lpf(u16 lpf){
    u8 dat = 0;

    // ���������� �ӼƵ�Fs=1Khz�� ���ݵ�Fs��������dat=0ʱΪ8Khz,����Ϊ1Khz

    // hz: 260 184 94 44 21 10 5
    // ��Ӧ���ã�0 1 2 3 4 5 6
    if(lpf>184) dat=0;
    else if(lpf>94) dat=1;
    else if(lpf>44) dat=2;
    else if(lpf>21) dat=3;
    else if(lpf>10) dat=4;
    else if(lpf>5) dat=5;
    else dat=6;
    return mpu_write_byte(MPU_CFG_REG, dat);
}

/**
 * @brief ���� MPU6050 �Ĳ�����
 * 
 * @param rate ������ 4~1000
 * @return u8 0 success
 */
u8 mpu_set_rate(u16 rate){
    u8 dat = 0;
    if(rate>1000) rate=1000;
    else if(rate<4) rate=4;

    dat = 1000 / rate -1; // ���SAMPRT_DIV������1000Ϊ����Ƶ��

    mpu_write_byte(MPU_SAMPLE_RATE_REG, dat);	//�������ֵ�ͨ�˲���
	return mpu_set_lpf(rate / 2);	//�Զ�����LPFΪ�����ʵ�һ��
}

/**
 * @brief ��ȡ�Ӽ�ֵ
 * 
 * @param gx x�Ӽ�
 * @param gy 
 * @param gz 
 * @return u8 0 success
 */
u8 mpu_get_acc(short *ax, short *ay, short *az){
    u8 buf[6], r;
    r = mpu_read_len(MPU_ACCEL_XOUTH_REG, 6, buf);
    if(r==1){
        *ax = (u16)buf[0]<<8 | buf[1];
        *ay = (u16)buf[2]<<8 | buf[3];
        *az = (u16)buf[4]<<8 | buf[5];
    }
    return r;
}

/**
 * @brief ��ȡ����ֵ
 * 
 * @param gx x����
 * @param gy 
 * @param gz 
 * @return u8 0 success
 */
u8 mpu_get_gyro(short *gx, short *gy, short *gz){
    u8 buf[6], r;
    r = mpu_read_len(MPU_GYRO_XOUTH_REG, 6, buf);
    if(r==1){
        *gx = (u16)buf[0]<<8 | buf[1];
        *gy = (u16)buf[2]<<8 | buf[3];
        *gz = (u16)buf[4]<<8 | buf[5];
    }
    return r;
}

// ��ȡ�¶�ֵ
float mpu_get_temperature(){
    u8 buf[2];
    short raw;
    
    mpu_read_len(MPU_TEMP_OUTH_REG, 2, buf);
    raw = (buf[0]<<8) | buf[1];
    return 36.5+ raw/340.0;
}


// mpu��ʼ��������0�ɹ�
u8 mpu_init(){
    u8 r;
    
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x80); // ��λ MPU6050
    mpu_delay_ms(100);
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x00); // ���� MPU6050
    mpu_set_gyro_fsr(3); // ���ݴ����� ��2000dps
    mpu_set_accel_fsr(0); // ���ٶȴ����� ��2g
    mpu_set_rate(50); // ������ 50Hz
    mpu_write_byte(MPU_INT_EN_REG, 0x01); // ���ݾ����ж�ʹ��
	mpu_write_byte(MPU_USER_CTRL_REG, 0X00); //I2C��ģʽ�ر�
	mpu_write_byte(MPU_FIFO_EN_REG, 0X00); //�ر�FIFO
	mpu_write_byte(MPU_INTBP_CFG_REG, 0X80); //INT���ŵ͵�ƽ��Ч
	r = mpu_read_byte(MPU_DEVICE_ID_REG);
    
	if (r == MPU_ADDR)//����ID��ȷ
	{
		mpu_write_byte(MPU_PWR_MGMT1_REG, 0X01); //����CLKSEL,PLL X��Ϊ�ο�
		mpu_write_byte(MPU_PWR_MGMT2_REG, 0X00); //���ٶ��������Ƕ�����
		mpu_set_rate(200); //���ò�����Ϊ200Hz
        return 0;
	}
	return 1;

}
