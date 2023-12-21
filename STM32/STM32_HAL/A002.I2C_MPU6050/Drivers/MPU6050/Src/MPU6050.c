#include "MPU6050.h"


// delay
void mpu_delay_ms(u16 ms){
    HAL_Delay(ms);
}

// iic 连续写入
u8 mpu_write_len(u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Write(&hi2c1, MPU_WRITE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, I2C_TIME_OUT) == HAL_OK;
}

// iic 连续读取
u8 mpu_read_len(u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Read(&hi2c1, MPU_READ_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, I2C_TIME_OUT) == HAL_OK;
}

// iic 写一个字节
u8 mpu_write_byte(u8 reg, u8 dat){
    return mpu_write_len(reg, 1, &dat);
}

// iic 读一个字节，返回数据或0（无效）
u8 mpu_read_byte(u8 reg){
    u8 data=0;
    mpu_read_len(reg, 1, &data);
    return data;
}

/**
 * @brief 设置 MPU6050 陀螺传感器满量程范围
 * 
 * @param fsr 0 ±250dps, 1 ±500dps, 2 ±1000dps, 3 ±2000dps
 * @return u8 0 success
 */
u8 mpu_set_gyro_fsr(u8 fsr){
    return mpu_write_byte(MPU_GYRO_CFG_REG, fsr<<3);
}

/**
 * @brief 设置 MPU6050 加速度传感器满量程范围
 * 
 * @param fsr 0 ±2g, 1 ±4g, 2 ±8g, 3 ±16g
 * @return u8 0 success
 */
u8 mpu_set_accel_fsr(u8 fsr){
    return mpu_write_byte(MPU_ACCEL_CFG_REG, fsr<<3);
}

/**
 * @brief 设置 MPU6050 的数字低通滤波器
 * 
 * @param lpf 数字低通滤波器 Hz (260 184 94 44 21 10 5)
 * @return u8 0 success
 */
u8 mpu_set_lpf(u16 lpf){
    u8 dat = 0;

    // 决定采样率 加计的Fs=1Khz， 陀螺的Fs不定，当dat=0时为8Khz,其余为1Khz

    // hz: 260 184 94 44 21 10 5
    // 对应配置：0 1 2 3 4 5 6
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
 * @brief 设置 MPU6050 的采样率
 * 
 * @param rate 采样率 4~1000
 * @return u8 0 success
 */
u8 mpu_set_rate(u16 rate){
    u8 dat = 0;
    if(rate>1000) rate=1000;
    else if(rate<4) rate=4;

    dat = 1000 / rate -1; // 求出SAMPRT_DIV。其中1000为采样频率

    mpu_write_byte(MPU_SAMPLE_RATE_REG, dat);	//设置数字低通滤波器
	return mpu_set_lpf(rate / 2);	//自动设置LPF为采样率的一半
}

/**
 * @brief 获取加计值
 * 
 * @param gx x加计
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
 * @brief 获取陀螺值
 * 
 * @param gx x陀螺
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

// 获取温度值
float mpu_get_temperature(){
    u8 buf[2];
    short raw;
    
    mpu_read_len(MPU_TEMP_OUTH_REG, 2, buf);
    raw = (buf[0]<<8) | buf[1];
    return 36.5+ raw/340.0;
}


// mpu初始化，返回0成功
u8 mpu_init(){
    u8 r;
    
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x80); // 复位 MPU6050
    mpu_delay_ms(100);
    mpu_write_byte(MPU_PWR_MGMT1_REG, 0x00); // 唤醒 MPU6050
    mpu_set_gyro_fsr(3); // 陀螺传感器 ±2000dps
    mpu_set_accel_fsr(0); // 加速度传感器 ±2g
    mpu_set_rate(50); // 采样率 50Hz
    mpu_write_byte(MPU_INT_EN_REG, 0x01); // 数据就绪中断使能
	mpu_write_byte(MPU_USER_CTRL_REG, 0X00); //I2C主模式关闭
	mpu_write_byte(MPU_FIFO_EN_REG, 0X00); //关闭FIFO
	mpu_write_byte(MPU_INTBP_CFG_REG, 0X80); //INT引脚低电平有效
	r = mpu_read_byte(MPU_DEVICE_ID_REG);
    
	if (r == MPU_ADDR)//器件ID正确
	{
		mpu_write_byte(MPU_PWR_MGMT1_REG, 0X01); //设置CLKSEL,PLL X轴为参考
		mpu_write_byte(MPU_PWR_MGMT2_REG, 0X00); //加速度与陀螺仪都工作
		mpu_set_rate(200); //设置采样率为200Hz
        return 0;
	}
	return 1;

}
