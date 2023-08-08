#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"

// ��ȡ FLASH ID
u32 SPI_FLASH_ReadId(void){
    u8 t[3]={0};
    u32 r;
    
    // ��ʼͨѶ��CS��
    SPI_FLASH_CS_LOW();
    
    // ���� JEDEC ָ�Ϊ�˶�ȡID
    SPI_FLASH_sendByte(W25X_JedecDeviceID);
    
    // ��ȡ3���ֽ�����
    t[0]=SPI_FLASH_sendByte(Dummy_Byte); // ����
    t[1]=SPI_FLASH_sendByte(Dummy_Byte); // �洢������
    t[2]=SPI_FLASH_sendByte(Dummy_Byte); // ����
    
    // ֹͣͨѶ��CS��
    SPI_FLASH_CS_HIGH();
    
    r = (t[0]<<16) | (t[1]<<8) | t[2];
    return r;

}