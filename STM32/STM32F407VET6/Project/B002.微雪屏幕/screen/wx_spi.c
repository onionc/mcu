#include "./wx_spi.h"

// SPI FLASH 初始化
void SPI_FLASH_INIT(){
    GPIO_InitTypeDef GPIO_init;
    SPI_InitTypeDef SPI_init;
    
    /* SPI 引脚配置 */
    
    // 使能 FLASH SPI GPIO 时钟
    RCC_AHB1PeriphClockCmd(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MOSI_GPIO_CLK | FLASH_CS_GPIO_CLK, ENABLE);
    
    // SPI 时钟
    FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
    
    // 引脚复用
    GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT , FLASH_SPI_SCK_PINSOURCE , FLASH_SPI_SCK_AF );
    GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT, FLASH_SPI_MOSI_PINSOURCE, FLASH_SPI_MOSI_AF);
    
    // 引脚配置
    GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_init.GPIO_Mode = GPIO_Mode_AF;
    GPIO_init.GPIO_OType = GPIO_OType_PP;
    GPIO_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_init.GPIO_Pin = FLASH_SPI_SCK_PIN;
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_init);
    
    GPIO_init.GPIO_Pin = FLASH_SPI_MOSI_PIN;
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_init);
    
    GPIO_init.GPIO_Pin = FLASH_CS_PIN;
    GPIO_init.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_init);
    
    /* SPI 模式配置 */
    SPI_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_init.SPI_Mode = SPI_Mode_Master;
    SPI_init.SPI_DataSize = SPI_DataSize_8b;
    SPI_init.SPI_CPOL = SPI_CPOL_Low;
    SPI_init.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_init.SPI_NSS = SPI_NSS_Soft;
    SPI_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_init.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_init.SPI_CRCPolynomial = 7; // CRC 无效，因为并没有使能SPI的CRC功能
    
    SPI_Init(FLASH_SPI, &SPI_init);
    // 使能 FLASH SPI
    SPI_Cmd(FLASH_SPI, ENABLE);
}

// spi 超时输出
static ErrorStatus SPI_TIMEOUT_MSG(char *msg){
    printf("SPI等待超时 errorMsg：%s", msg);
    return ERROR; 
}

/**
 * @brief 检测事件
 * 
 * @param eventState 事件
 * @param errorCode 自定义的错误码
 * @return ErrorStatus
 */
ErrorStatus SPI_CheckEvent(uint32_t eventState, char *errorMsg){
    // 超时时间赋值
    __IO u32 timeout = SPIT_FLAG_TIMEOUT;
    
    // 检测事件并清除标志
    while(RESET == SPI_I2S_GetFlagStatus(FLASH_SPI, eventState)){
        if((timeout--)==0) return SPI_TIMEOUT_MSG(errorMsg);
    }
    
    
    return SUCCESS;
}

// 使用SPI发送一个字节数据
u8 SPI_FLASH_sendByte(u8 byte){
    // 等待发送缓冲区为空，TXE事件
    if(SPI_CheckEvent(SPI_I2S_FLAG_TXE, "txe flag")==ERROR){
        return 0;
    }
    
    // 写入数据寄存器，把数据写入发送缓冲区
    SPI_I2S_SendData(FLASH_SPI, byte);
    
    // 等待发送缓冲区为空，RXNE事件
    if(SPI_CheckEvent(SPI_I2S_FLAG_RXNE, "rxne flag")==ERROR){
        return 0;
    }
    // 读取数据寄存器
    return SPI_I2S_ReceiveData(FLASH_SPI);
    
}

// 使用SPI读取一个字节数据
u8 SPI_FLASH_readByte(){
    return SPI_FLASH_sendByte(0xff);
}















