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
 * @param errorCode �Զ���Ĵ�����
 * @return ErrorStatus
 */
ErrorStatus CheckEvent(uint32_t eventState, uint32_t errorCode){
    // ��ʱʱ�丳ֵ
    __IO u32 timeout = I2CT_FLAG_TIMEOUT;
    
    // ����¼��������־
    while(ERROR == I2C_CheckEvent(EEPROM_I2C, eventState)){
        if((timeout--)==0) return I2C_TIMEOUT_MSG(errorCode);
    }
    
    return SUCCESS;
}



/**
 * @brief дһ���ֽڵ�EEPROM
 * 
 * @param writeAddr д��ַ
 * @param pBuf ������ָ��
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteByte(u8 writeAddr, u8 *pBuf){
    
    // ��ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // �ж� EV5 �¼�����ʼλ�������
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){

        return ERROR;
    }
    
    // ���� EEPROM �豸��ַ
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter); // #3 ����������
    // �ж� EV6 �¼�����ַ�������
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){

        return ERROR;
    }
    
    // ����EEPROM�ڲ���ַ������EEPROM������˵�ǵ�ַ������I2CЭ����˵�����ݵ�һ���֣����Ժ�����һ����EV8�ж�
    I2C_SendData(EEPROM_I2C, writeAddr);
    // �ж� EV8 �¼������ݷ������
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, ErrCodeEv8_Addr)==ERROR){
        return ERROR;
    }
    
    
    // ����һ�ֽ�����
    I2C_SendData(EEPROM_I2C, *pBuf);
    // �ж� EV8 �¼������ݷ������
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, ErrCodeEv8_Data)==ERROR){
        return ERROR;
    }
    
    // ֹͣ�ź�
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    // �ж� EV8_2 �¼������ݷ������
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, ErrCodeEv8_Data)==ERROR){
        return ERROR;
    }
    
    return SUCCESS;
    
}

// �ȴ� EEPROM ��׼��״̬
ErrorStatus I2C_EE_WaitEepromStandbyState(void){
     /*
    // �ȴ��ӻ�Ӧ��
    do{
        // ��ʼ�ź�
        I2C_GenerateSTART(EEPROM_I2C, ENABLE);

        // ���� EEPROM �豸��ַ
        I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
        
        // �ȴ�ADDR��־
    }while(!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1) & I2C_SR1_ADDR));
    
    // ��� af
    I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
    // ֹͣ�ź�
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    */
    __IO u32 I2CTimeout = I2CT_FLAG_TIMEOUT;

    __IO uint16_t tmpSR1 = 0;
    __IO uint32_t EETrials = 0;

    /*����æʱ�ȴ� */
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0) return ERROR;
    }

    /* �ȴ��ӻ�Ӧ�����ȴ�300�� */
    while (1)
    {
        /*��ʼ�ź� */
        I2C_GenerateSTART(EEPROM_I2C, ENABLE);

        /* ��� EV5 �¼��������־*/
        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        {
            if ((I2CTimeout--) == 0) return ERROR;
        }

        /* ����EEPROM�豸��ַ */
        I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);

        /* �ȴ�ADDR��־ */
        I2CTimeout = I2CT_LONG_TIMEOUT;
        do
        {
            /* ��ȡSR1�Ĵ���״̬ */
            tmpSR1 = EEPROM_I2C->SR1;

            if ((I2CTimeout--) == 0) return ERROR;
        }
        /* һֱ�ȴ�ֱ��addr��af��־Ϊ1 */
        while ((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);

        /*���addr��־�Ƿ�Ϊ1 */
        if (tmpSR1 & I2C_SR1_ADDR)
        {
            /* ���addr��־�ñ�־ͨ����SR1��SR2��� */
            (void)EEPROM_I2C->SR2;

            /*����ֹͣ�ź� */
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

            /* �˳����� */
            return 1;
        }
        else
        {
            /*���af��־ */
            I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
        }

        /*���ȴ�����*/
        if (EETrials++ == 300)
        {
            /* �ȴ�MAX_TRIAL_NUMBER�ζ���û׼���ã��˳��ȴ� */
            return ERROR;
        }
    }
    return SUCCESS;
}


/**
 * @brief д����ֽ�
 * 
 * @param pBuf ������ָ��
 * @param writeAddr д��ַ
 * @param len д����ֽ���
 * @return ErrorStatus 
 */

ErrorStatus I2C_EE_WriteBytes(u8 *pBuf, u8 writeAddr, u16 len){
    while(len-->0){
        // �ȴ� EEPROM ׼����ϣ�ÿдһ���ֽڵ���һ��
        if(I2C_EE_WaitEepromStandbyState()==ERROR){
            return ERROR;
        }
        if(I2C_EE_WriteByte(writeAddr++, pBuf++)==ERROR){
            return ERROR;
        }
    }
    
    return SUCCESS;
}

/**
 * @brief ��ȡ����
 * 
 * @param readAddr ��ȡ���ݵĵ�ַ
 * @param pBuf �������ݵĻ�����ָ��
 * @param len ��ȡ���ֽ���
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_ReadBytes(u8 readAddr, u8 *pBuf, u16 len){
    // �ȴ� EEPROM ׼�����
    if(I2C_EE_WaitEepromStandbyState()==ERROR){
        return ERROR;
    }
    
    // ��ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // �ж� EV5 �¼�����ʼλ�������
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){
        return ERROR;
    }
    
    // ���� EEPROM �豸��ַ
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter); // #3 ����������
    // �ж� EV6 �¼�����ַ�������
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){
        return ERROR;
    }
    
    // ����EEPROM�ڲ���ַ������EEPROM������˵�ǵ�ַ������I2CЭ����˵�����ݵ�һ���֣����Ժ�����һ����EV8�ж�
    I2C_SendData(EEPROM_I2C, readAddr);
    // �ж� EV8 �¼������ݷ������
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, ErrCodeEv8_Addr)==ERROR){
        return ERROR;
    }
    
    /* �ڶ��β����ź� */
    // ��ʼ�ź�
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // �ж� EV5 �¼�����ʼλ�������
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){
        return ERROR;
    }
    
    // ���� EEPROM �豸��ַ
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Receiver); // #3 �������������Ϊ��
    // �ж� EV6 �¼�����ַ�������
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){
        return ERROR;
    }
    printf("`");
    /* ��ȡlen������ */
    while(len--){
        // ���������ж� 
        if(CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED, ErrCodeRecv)==ERROR){
            return ERROR;
        }
        
        // ��ȡһ���ֽڵ�����
        *pBuf++ = I2C_ReceiveData(EEPROM_I2C);
        
        if(!len){
            /* ֹͣ */
            
            // ���� nack
            I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
            // ����ֹͣ�ź�
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
        }else{
            
            // ���� ack��׼��������һ��
            I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
        }
    }

    return SUCCESS;
}
