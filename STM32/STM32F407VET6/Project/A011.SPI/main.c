
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"



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
    
    SPI_FLASH_INIT();
    id = SPI_FLASH_ReadId();
    printf("id=%x\n", id);
    while(1){
        
        
    }
    
    return 0;
}
