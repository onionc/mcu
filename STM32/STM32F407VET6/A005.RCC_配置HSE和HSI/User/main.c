
#include "stm32f4xx.h"
#include "led/bsp_led.h"
#include "rcc/bsp_clkcfg.h"

void delay(__IO uint32_t count){
    for(; count!=0; count--);
}

int main(){
    
    // led 初始化
    LED_GPIO_Config();
    
    // 默认全灭
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 配置系统时钟为 168M
    // 使用 HSE
    if(HSE_SetSysClk(25, 336, 2, 7)==ERROR){
        LED1_ON; LED2_ON; LED3_ON;
    }
    
    
    /*
    // 使用HSI 
    if(HSI_SetSysClk(16, 336, 2, 7)==ERROR){
        LED1_ON; LED2_ON; LED3_ON;
    }
    */

    while(1){
        LED1_TOGGLE;
        delay(0xffffff);
    }
    return 0;
}
