
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"

#define ADDR 0x0000
u8 data[100] = "�����׾���ؽ��޷�";
u8 data2[100] = {0};

//�򵥵���ʱ����
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}



int main(){
    u32 id;
    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart1_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\n");
    
    // ��ʼ��FLASH����ȡID
    SPI_FLASH_INIT();
    Delay(200);
    id = SPI_FLASH_ReadId();
    printf("id=%x\n", id);

    // ��������
    if(SPI_FLASH_SectorErase(ADDR)==ERROR){
        printf("���ݲ���ʧ��\n");
    }
    
    // ��������
    if(SPI_FLASH_PageWrite(data, ADDR, sizeof(data))==ERROR){
        printf("����д��ʧ��\n");
    }else{
        printf("����д��ɹ���len=%d\n", sizeof(data));
    }
    
    // ��ȡ����
    if(SPI_FLASH_ReadData(data2, ADDR, 100)==ERROR){
        printf("����д��ʧ��\n");
    }else{
        printf("���ݶ�ȡ�ɹ���data=%s\n", data2);
    }
        
    
    while(1){
        
        
    }
    
    return 0;
}
