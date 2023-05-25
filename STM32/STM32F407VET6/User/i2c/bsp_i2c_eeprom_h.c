#include "./i2c/bsp_i2c_eeprom_h.h"


// I2C gpio 配置 
static void I2C_GPIO_Cfg(void){
    GPIO_InitTypeDef gpioInit;
    
    // 使能I2C时钟
    RCC_APB1PeriphClockCmd(EEPROM_I2C_CLK, ENABLE);
    
    // 使能GPIO时钟
    RCC_AHB1PeriphClockCmd(EEPROM_I2C_SCL_GPIO_CLK | EEPROM_I2C_SDA_GPIO_CLK, ENABLE);
    
    /* 引脚复用 */
    // 连接引脚 到 I2C_SCL
    GPIO_PinAFConfig(EEPROM_I2C_SCL_GPIO_PORT, EEPROM_I2C_SCL_SOURCE, EEPROM_I2C_SCL_AF);
    // 连接引脚 到 I2C_SDA
    GPIO_PinAFConfig(EEPROM_I2C_SDA_GPIO_PORT, EEPROM_I2C_SDA_SOURCE, EEPROM_I2C_SDA_AF);
    
    // 配置gpio引脚
    gpioInit.GPIO_Pin = EEPROM_I2C_SCL_PIN;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_OType = GPIO_OType_OD; // 开漏
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &gpioInit);
    
    gpioInit.GPIO_Pin = EEPROM_I2C_SDA_PIN;
    GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &gpioInit);
    
}


// I2C 工作模式配置
static void I2C_Mode_Cfg(void){
    I2C_InitTypeDef i2cInit;
    
    /* I2C 配置 */
    i2cInit.I2C_Mode = I2C_Mode_I2C; // i2c模式
    i2cInit.I2C_DutyCycle = I2C_DutyCycle_2; // 占空比
    i2cInit.I2C_OwnAddress1 = I2C_OWN_ADDR; // i2c 自身地址
    i2cInit.I2C_Ack = I2C_Ack_Enable; // 使能响应
    i2cInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 寻址7位
    i2cInit.I2C_ClockSpeed = I2C_Speed; // 通信速率
    
    I2C_Init(EEPROM_I2C, &i2cInit); // 写入配置
    
    I2C_Cmd(EEPROM_I2C, ENABLE); // 使能 I2C
}

// I2C 外设初始化
void I2C_EE_INIT(void){
    I2C_GPIO_Cfg();
    I2C_Mode_Cfg();
}

// I2C超时输出
static ErrorStatus I2C_TIMEOUT_MSG(u8 errorCode){
    printf("I2C等待超时 errorCode=%d", errorCode);
    return ERROR; 
}

/**
 * @brief 检测事件
 * 
 * @param eventState 事件
 * @param errorCode 自定义的错误码
 * @return ErrorStatus
 */
ErrorStatus CheckEvent(uint32_t eventState, uint32_t errorCode){
    // 超时时间赋值
    __IO u32 timeout = I2CT_FLAG_TIMEOUT;
    
    // 检测事件并清除标志
    while(ERROR == I2C_CheckEvent(EEPROM_I2C, eventState)){
        if((timeout--)==0) return I2C_TIMEOUT_MSG(errorCode);
    }
    
    return SUCCESS;
}



/**
 * @brief 写一个字节到EEPROM
 * 
 * @param writeAddr 写地址
 * @param pBuf 缓冲区指针
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteByte(u8 writeAddr, u8 *pBuf){
    
    // 起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // 判断 EV5 事件，起始位发送完成
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){

        return ERROR;
    }
    
    // 发送 EEPROM 设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter); // #3 参数代表方向
    // 判断 EV6 事件，地址发送完成
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){

        return ERROR;
    }
    
    // 发送EEPROM内部地址，对于EEPROM外设来说是地址，对于I2C协议来说是数据的一部分，所以和数据一样用EV8判断
    I2C_SendData(EEPROM_I2C, writeAddr);
    // 判断 EV8 事件，数据发送完成
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, ErrCodeEv8_Addr)==ERROR){
        return ERROR;
    }
    
    
    // 发送一字节数据
    I2C_SendData(EEPROM_I2C, *pBuf);
    // 判断 EV8 事件，数据发送完成
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, ErrCodeEv8_Data)==ERROR){
        return ERROR;
    }
    
    // 停止信号
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    // 判断 EV8_2 事件，数据发送完成
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, ErrCodeEv8_Data)==ERROR){
        return ERROR;
    }
    
    return SUCCESS;
    
}

// 等待 EEPROM 到准备状态
ErrorStatus I2C_EE_WaitEepromStandbyState(void){
     /*
    // 等待从机应答
    do{
        // 开始信号
        I2C_GenerateSTART(EEPROM_I2C, ENABLE);

        // 发送 EEPROM 设备地址
        I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
        
        // 等待ADDR标志
    }while(!(I2C_ReadRegister(EEPROM_I2C, I2C_Register_SR1) & I2C_SR1_ADDR));
    
    // 清除 af
    I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
    // 停止信号
    I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
    */
    __IO u32 I2CTimeout = I2CT_FLAG_TIMEOUT;

    __IO uint16_t tmpSR1 = 0;
    __IO uint32_t EETrials = 0;

    /*总线忙时等待 */
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0) return ERROR;
    }

    /* 等待从机应答，最多等待300次 */
    while (1)
    {
        /*开始信号 */
        I2C_GenerateSTART(EEPROM_I2C, ENABLE);

        /* 检测 EV5 事件并清除标志*/
        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while (!I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
        {
            if ((I2CTimeout--) == 0) return ERROR;
        }

        /* 发送EEPROM设备地址 */
        I2C_Send7bitAddress(EEPROM_I2C,EEPROM_ADDR,I2C_Direction_Transmitter);

        /* 等待ADDR标志 */
        I2CTimeout = I2CT_LONG_TIMEOUT;
        do
        {
            /* 获取SR1寄存器状态 */
            tmpSR1 = EEPROM_I2C->SR1;

            if ((I2CTimeout--) == 0) return ERROR;
        }
        /* 一直等待直到addr及af标志为1 */
        while ((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);

        /*检查addr标志是否为1 */
        if (tmpSR1 & I2C_SR1_ADDR)
        {
            /* 清除addr标志该标志通过读SR1及SR2清除 */
            (void)EEPROM_I2C->SR2;

            /*产生停止信号 */
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);

            /* 退出函数 */
            return 1;
        }
        else
        {
            /*清除af标志 */
            I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
        }

        /*检查等待次数*/
        if (EETrials++ == 300)
        {
            /* 等待MAX_TRIAL_NUMBER次都还没准备好，退出等待 */
            return ERROR;
        }
    }
    return SUCCESS;
}


/**
 * @brief 写多个字节
 * 
 * @param pBuf 缓冲区指针
 * @param writeAddr 写地址
 * @param len 写入的字节数
 * @return ErrorStatus 
 */

ErrorStatus I2C_EE_WriteBytes(u8 *pBuf, u8 writeAddr, u16 len){
    while(len-->0){
        // 等待 EEPROM 准备完毕，每写一个字节调用一次
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
 * @brief 读取数据
 * 
 * @param readAddr 读取数据的地址
 * @param pBuf 接收数据的缓冲区指针
 * @param len 读取的字节数
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_ReadBytes(u8 readAddr, u8 *pBuf, u16 len){
    // 等待 EEPROM 准备完毕
    if(I2C_EE_WaitEepromStandbyState()==ERROR){
        return ERROR;
    }
    
    // 起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // 判断 EV5 事件，起始位发送完成
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){
        return ERROR;
    }
    
    // 发送 EEPROM 设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter); // #3 参数代表方向
    // 判断 EV6 事件，地址发送完成
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){
        return ERROR;
    }
    
    // 发送EEPROM内部地址，对于EEPROM外设来说是地址，对于I2C协议来说是数据的一部分，所以和数据一样用EV8判断
    I2C_SendData(EEPROM_I2C, readAddr);
    // 判断 EV8 事件，数据发送完成
    if(CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, ErrCodeEv8_Addr)==ERROR){
        return ERROR;
    }
    
    /* 第二次产生信号 */
    // 起始信号
    I2C_GenerateSTART(EEPROM_I2C, ENABLE);
    // 判断 EV5 事件，起始位发送完成
    if(CheckEvent(I2C_EVENT_MASTER_MODE_SELECT, ErrCodeEv5)==ERROR){
        return ERROR;
    }
    
    // 发送 EEPROM 设备地址
    I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Receiver); // #3 参数代表方向，这次为读
    // 判断 EV6 事件，地址发送完成
    if(CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, ErrCodeEv6)==ERROR){
        return ERROR;
    }
    printf("`");
    /* 读取len个数据 */
    while(len--){
        // 接收数据判断 
        if(CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED, ErrCodeRecv)==ERROR){
            return ERROR;
        }
        
        // 读取一个字节的数据
        *pBuf++ = I2C_ReceiveData(EEPROM_I2C);
        
        if(!len){
            /* 停止 */
            
            // 发送 nack
            I2C_AcknowledgeConfig(EEPROM_I2C, DISABLE);
            // 发送停止信号
            I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
        }else{
            
            // 发送 ack，准备接收下一组
            I2C_AcknowledgeConfig(EEPROM_I2C, ENABLE);
        }
    }

    return SUCCESS;
}
