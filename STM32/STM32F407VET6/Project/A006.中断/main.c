
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./key/bsp_exti_key.h"

void delay(__IO uint32_t count){
    for(; count!=0; count--);
}

void KEY1_IRQHandler(void){
    // 确认产生了中断
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET){
        LED1_TOGGLE;
        
        // 清除中断标志位
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

int main(){
    
    // led 初始化
    LED_GPIO_Config();
    
    // 默认全灭
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化EXTI中断
    EXTI_Key_Cfg();


    while(1){
        
    }
    
    return 0;
}
