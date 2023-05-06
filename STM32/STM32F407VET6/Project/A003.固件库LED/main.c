
#include "stm32f4xx.h"
#include "led/bsp_led.h"

int main(){
    LED_GPIO_Config();
    
    // 使用库函数的
    //LED1(ON);
    //LED2(OFF);
    //LED3(OFF);
    
    // 使用寄存器的
    LED1_OFF;
    LED2_ON;
    LED3_OFF;
    
    // 反转
    LED1_TOGGLE;

    while(1);
    return 0;
}
