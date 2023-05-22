#include "./i2c/bsp_i2c_eeprom_h.h"


// I2C gpio ���� 
static void I2C_GPIO_Cfg(void){
    GPIO_InitTypeDef gpioInit;
    
    // ʹ��I2Cʱ��
    RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, ENABLE);
    
    // ʹ��GPIOʱ��
    RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
    
    /* ���Ÿ��� */
    // �������� �� I2C_SCL
    GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE, EEPROM_I2C_SCL_AF);
    // �������� �� I2C_SDA
    GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE, EEPROM_I2C_SDA_AF);
    
    // ����gpio����
    gpioInit.GPIO_Pin = EEPROM_I2C_SCL_PIN;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_OType = GPIO_OType_OD; // ��©
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &gpioInit);
    
    gpioInit.GPIO_Pin = EEPROM_I2C_SDA_PIN;
    GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &gpioInit);
    
}


// I2C ����ģʽ����
static void I2C_Mode_Cfg(void){
    I2C_InitTypeDef i2cInit;
    
    /* I2C ���� */
    i2cInit.I2C_Mode = I2C_Mode_I2C; // i2cģʽ
    i2cInit.I2C_DutyCycle = I2C_DutyCycle_2; // ռ�ձ�
    i2cInit.I2C_OwnAddress1 = I2C_OWN_ADDR; // i2c �����ַ
    i2cInit.I2C_Ack = I2C_Ack_Enable; // ʹ����Ӧ
    i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // Ѱַ7λ
    i2cInit.I2C_ClockSpeed = I2C_Speed; // ͨ������
    
    I2C_Init(EEPROM_I2C, &i2cInit); // д������
    
    I2C_Cmd(EEPROM_I2C, ENABLE); // ʹ�� I2C
}

// I2C �����ʼ��
void I2C_EE_INIT(void){
    I2C_GPIO_Cfg();
    I2C_Mode_Cfg();
}

// I2C��ʱ���
static ErrorStatus I2C_TIMEOUT_MSG(u8 errorCode){
    printf("I2C�ȴ���ʱ errorCode=%d", errorCode);
    return ERROR; 
}

/**
 * @brief ����¼�
 * 
 * @param eventState �¼�
 * @return ErrorStatus
 */
ErrorStatus CheckEvent(uint32_t eventState, uint32_t errorCode){
    // ��ʱʱ�丳ֵ
    __IO u32 timeout = I2CT_FLAG_TIMEOUT;
    
    // ����¼��������־
    while(!I2C_CheckEvent(EEPROM_I2C, eventState)){
        if((timeout--)==0) return I2C_TIMEOUT_MSG(errorCode);
    }
    
    return SUCCESS;
}

/**
 * @brief дһ���ֽڵ�EEPROM
 * 
 * @param pBuf ������ָ��
 * @param writeAddr д��ַ
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteByte(u8 *pBuf, u8 writeAddr){
    // ��ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    
    // �ж� EV5 �¼�����ʼλ�������
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, 0)==ERROR){
        return ERROR;
    }
    
    // ���� EEPROM �豸��ַ
    I2C_Send7bitAddress(EEPROM_I2C, 0);
    
    
    
    // ֹͣ�ź�
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    
}


