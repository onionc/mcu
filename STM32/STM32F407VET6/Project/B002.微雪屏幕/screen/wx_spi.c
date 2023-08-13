#include "./wx_spi.h"

// SPI FLASH ��ʼ��
void SPI_FLASH_INIT(){
    GPIO_InitTypeDef GPIO_init;
    SPI_InitTypeDef SPI_init;
    
    /* SPI �������� */
    
    // ʹ�� FLASH SPI GPIO ʱ��
    RCC_AHB1PeriphClockCmd(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MOSI_GPIO_CLK | FLASH_CS_GPIO_CLK, ENABLE);
    
    // SPI ʱ��
    FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
    
    // ���Ÿ���
    GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT , FLASH_SPI_SCK_PINSOURCE , FLASH_SPI_SCK_AF );
    GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT, FLASH_SPI_MOSI_PINSOURCE, FLASH_SPI_MOSI_AF);
    
    // ��������
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
    
    /* SPI ģʽ���� */
    SPI_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_init.SPI_Mode = SPI_Mode_Master;
    SPI_init.SPI_DataSize = SPI_DataSize_8b;
    SPI_init.SPI_CPOL = SPI_CPOL_Low;
    SPI_init.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_init.SPI_NSS = SPI_NSS_Soft;
    SPI_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPI_init.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_init.SPI_CRCPolynomial = 7; // CRC ��Ч����Ϊ��û��ʹ��SPI��CRC����
    
    SPI_Init(FLASH_SPI, &SPI_init);
    // ʹ�� FLASH SPI
    SPI_Cmd(FLASH_SPI, ENABLE);
}

// spi ��ʱ���
static ErrorStatus SPI_TIMEOUT_MSG(char *msg){
    printf("SPI�ȴ���ʱ errorMsg��%s", msg);
    return ERROR; 
}

/**
 * @brief ����¼�
 * 
 * @param eventState �¼�
 * @param errorCode �Զ���Ĵ�����
 * @return ErrorStatus
 */
ErrorStatus SPI_CheckEvent(uint32_t eventState, char *errorMsg){
    // ��ʱʱ�丳ֵ
    __IO u32 timeout = SPIT_FLAG_TIMEOUT;
    
    // ����¼��������־
    while(RESET == SPI_I2S_GetFlagStatus(FLASH_SPI, eventState)){
        if((timeout--)==0) return SPI_TIMEOUT_MSG(errorMsg);
    }
    
    
    return SUCCESS;
}

// ʹ��SPI����һ���ֽ�����
u8 SPI_FLASH_sendByte(u8 byte){
    // �ȴ����ͻ�����Ϊ�գ�TXE�¼�
    if(SPI_CheckEvent(SPI_I2S_FLAG_TXE, "txe flag")==ERROR){
        return 0;
    }
    
    // д�����ݼĴ�����������д�뷢�ͻ�����
    SPI_I2S_SendData(FLASH_SPI, byte);
    
    // �ȴ����ͻ�����Ϊ�գ�RXNE�¼�
    if(SPI_CheckEvent(SPI_I2S_FLAG_RXNE, "rxne flag")==ERROR){
        return 0;
    }
    // ��ȡ���ݼĴ���
    return SPI_I2S_ReceiveData(FLASH_SPI);
    
}

// ʹ��SPI��ȡһ���ֽ�����
u8 SPI_FLASH_readByte(){
    return SPI_FLASH_sendByte(0xff);
}















