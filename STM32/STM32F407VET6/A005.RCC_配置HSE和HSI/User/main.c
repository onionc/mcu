
#include "stm32f4xx.h"
#include "led/bsp_led.h"
#include "rcc/bsp_clkcfg.h"

void delay(__IO uint32_t count){
    for(; count!=0; count--);
}

int main(){
    
    // led ��ʼ��
    LED_GPIO_Config();
    
    // Ĭ��ȫ��
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ����ϵͳʱ��Ϊ 168M
    // ʹ�� HSE
    if(HSE_SetSysClk(25, 336, 2, 7)==ERROR){
        LED1_ON; LED2_ON; LED3_ON;
    }
    
    
    /*
    // ʹ��HSI 
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
