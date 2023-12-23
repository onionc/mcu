#include "MPU6050.h"


// delay
void mpu_delay_ms(unsigned long ms){
    HAL_Delay(ms);
}

// iic ����д��, slaveAddr ������ַ����Ҫ�������ֶ�дʹ��
u8 mpu_write_len(u8 slaveAddr, u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Write(&hi2c1, (slaveAddr<<1), reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, 0x100);
    //return HAL_I2C_Mem_Write(&hi2c1, (slaveAddr), reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, 0x100);
}

// iic ������ȡ
u8 mpu_read_len(u8 slaveAddr, u8 reg, u8 len, u8 *pBuf){
    return HAL_I2C_Mem_Read(&hi2c1, ((slaveAddr<<1) | 1), reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, 0x100);
    //return HAL_I2C_Mem_Read(&hi2c1, (slaveAddr), reg, I2C_MEMADD_SIZE_8BIT, pBuf, len, 0x100);
}



#ifdef EMPL
// ��ȡ������� MPL����
void get_tick_count(unsigned long *count)
{
	*count = HAL_GetTick();
}

// MPLģ���ʼ��
struct int_param_s int_param;
/* Private typedef -----------------------------------------------------------*/
struct hal_s {
    unsigned char lp_accel_mode;
    unsigned char lp_6axis_mode;
    unsigned char sensors;
    unsigned char watermark;
    //volatile unsigned char new_sensor;
    unsigned char motion_int_mode;
    unsigned long next_temp_tick;
    unsigned int report;
};

/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};


/* Private variables ---------------------------------------------------------*/
/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the driver(s).
 */
static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

static struct hal_s hal = {0};

void mpl_moudle_init(){
    inv_error_t  result = mpu_init(&int_param);
    unsigned short gyro_rate, gyro_fsr;
    unsigned char accel_fsr,  new_temp = 0;


    if (result) {
        MPL_LOGE("Could not initialize gyro.\n");
        return;
    } 

    result = inv_init_mpl();
  if (result) {
      MPL_LOGE("Could not initialize MPL.\n");
      return;
  }

    /* Compute 6-axis and 9-axis quaternions. */
    inv_enable_quaternion();
    inv_enable_9x_sensor_fusion();
    /* The MPL expects compass data at a constant rate (matching the rate
     * passed to inv_set_compass_sample_rate). If this is an issue for your
     * application, call this function, and the MPL will depend on the
     * timestamps passed to inv_build_compass instead.
     *
     * inv_9x_fusion_use_timestamps(1);
     */

    /* This function has been deprecated.
     * inv_enable_no_gyro_fusion();
     */

    /* Update gyro biases when not in motion. ���ݾ�ֹʱ����ƫ��
     * WARNING: These algorithms are mutually exclusive.
     */
    inv_enable_fast_nomot();
    /* inv_enable_motion_no_motion(); */
    /* inv_set_no_motion_time(1000); */

    /* Update gyro biases when temperature changes. �²� */
    inv_enable_gyro_tc();

   
    /* Allows use of the MPL APIs in read_from_mpl. */
    inv_enable_eMPL_outputs();

    result = inv_start_mpl();
    if (result == INV_ERROR_NOT_AUTHORIZED) {
        MPL_LOGE("Not authorized.\n");
        return;
    }
    if (result) {
        MPL_LOGE("Could not start the MPL.\n");
        return;
    }
      /* Get/set hardware configuration. Start gyro. */
      /* Wake up all sensors. */
      mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
      /* Push both gyro and accel data into the FIFO. */
      mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
      mpu_set_sample_rate(DEFAULT_MPU_HZ);
      /* Read back configuration in case it was set improperly. */
      mpu_get_sample_rate(&gyro_rate);
      mpu_get_gyro_fsr(&gyro_fsr);
      mpu_get_accel_fsr(&accel_fsr);
      /* Sync driver configuration with MPL. */
      /* Sample rate expected in microseconds. */
      inv_set_gyro_sample_rate(1000000L / gyro_rate);
      inv_set_accel_sample_rate(1000000L / gyro_rate);
      /* Set chip-to-body orientation matrix.
       * Set hardware units to dps/g's/degrees scaling factor.
       */
      inv_set_gyro_orientation_and_scale(
              inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
              (long)gyro_fsr<<15);
      inv_set_accel_orientation_and_scale(
              inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
              (long)accel_fsr<<15);
      /* Initialize HAL state variables. */
      hal.sensors = ACCEL_ON | GYRO_ON;
      hal.report = 0;
      hal.next_temp_tick = 0;
      printf("MPL INIT OK.");
      return;

}

/** 
 *  @brief ��MPU��FIFO�ж�ȡ���ݲ�����
           ����Ҳ������mpl����¶ȱ仯�Ĵ���
 *  @return ��
 *  @attention Ӧ������Ϊ10msִ��һ��
 */
int mpu_module_sampling()
{
  unsigned char sensors, more,new_temp = 0;
  static unsigned long ticktime = 0;
  unsigned long sensor_timestamp,cycletime = 0;
  
  int new_data = 0;
  
  ticktime++;
  
  /* �¶�û�б�Ҫÿ�ζ���ȡ������һ��ʱ������ȡ */
  if (ticktime > hal.next_temp_tick) {
      hal.next_temp_tick = ticktime + TEMP_READ_TICK;
      new_temp = 1; //start task temp;
  }
  
  /* û���κ�sensor��ʱ */
  if (!hal.sensors) return 0;
  
  do {
    short gyro[3], accel_short[3];
    long accel[3], temperature;
    
    cycletime++;
    
    mpu_read_fifo(gyro, accel_short, &sensor_timestamp,
        &sensors, &more);
    if (sensors & INV_XYZ_GYRO) {
        /* ����������MPL. */
        inv_build_gyro(gyro, sensor_timestamp);
        new_data = 1;
        if (new_temp) {
            new_temp = 0;
            /* Temperature only used for gyro temp comp. */
            mpu_get_temperature(&temperature, &sensor_timestamp);
            inv_build_temp(temperature, sensor_timestamp);
        }
    }
    if (sensors & INV_XYZ_ACCEL) {
        accel[0] = (long)accel_short[0];
        accel[1] = (long)accel_short[1];
        accel[2] = (long)accel_short[2];
        inv_build_accel(accel, 0, sensor_timestamp);
        new_data = 1;
    }
    
    if (new_data) {
        if(inv_execute_on_data()) {
            MPL_LOGE("ERROR execute on data\n");
        }
        #ifdef MPU_DEBUG
          read_from_mpl(); 
        #endif
    }
  }
  while(more);
  return cycletime;
}


#else
// ��ͨmpu6050���ƣ�ʹ����EMPLʱ�з������ͻ

// iic дһ���ֽ�
u8 mpu_write_byte(u8 reg, u8 dat){
    return mpu_write_len(MPU_ADDR, reg, 1, &dat);
}

// iic ��һ���ֽڣ��������ݻ�0����Ч��
u8 mpu_read_byte(u8 reg){
    u8 data=0;
    mpu_read_len(MPU_ADDR, reg, 1, &data);
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
    r = mpu_read_len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
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
    r = mpu_read_len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
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
    
    mpu_read_len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
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
#endif

