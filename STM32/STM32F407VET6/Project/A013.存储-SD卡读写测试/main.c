
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"
#include "./sdio/sdio_test.h"



int main(){
    u32 id;

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF; // ��������������
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart1_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\r\n");
    
    SD_TEST();
    
    while(1){
        
        
    }
    
    return 0;
}
