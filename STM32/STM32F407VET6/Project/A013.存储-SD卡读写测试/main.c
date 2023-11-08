
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"
#include "./sdio/sdio_test.h"



int main(){
    u32 id;

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF; // 灯亮代表有问题
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\r\n");
    
    SD_TEST();
    
    while(1){
        
        
    }
    
    return 0;
}
