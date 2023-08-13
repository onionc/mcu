
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"



//简单的延时函数
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}



int main(){
    u32 id;
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\n");
    
    SPI_FLASH_INIT();
    id = SPI_FLASH_ReadId();
    printf("id=%x\n", id);
    while(1){
        
        
    }
    
    return 0;
}
