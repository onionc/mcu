#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"

// 读取 FLASH ID
u32 SPI_FLASH_ReadId(void){
    u8 t[3]={0};
    u32 r;
    
    // 开始通讯：CS低
    SPI_FLASH_CS_LOW();
    
    // 发送 JEDEC 指令，为了读取ID
    SPI_FLASH_sendByte(W25X_JedecDeviceID);
    
    // 读取3个字节数据
    t[0]=SPI_FLASH_sendByte(Dummy_Byte); // 厂商
    t[1]=SPI_FLASH_sendByte(Dummy_Byte); // 存储器类型
    t[2]=SPI_FLASH_sendByte(Dummy_Byte); // 容量
    
    // 停止通讯：CS高
    SPI_FLASH_CS_HIGH();
    
    r = (t[0]<<16) | (t[1]<<8) | t[2];
    return r;

}