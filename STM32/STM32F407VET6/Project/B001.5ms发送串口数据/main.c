// 1.手动接收处理
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./tim/bsp_basic_tim.h"

int main(){
    char chTemp;
    u8 rbuf[256];
    
    // LED
    LED_GPIO_Config();
    LED1_OFF;LED2_OFF;LED3_OFF;
    
    // 初始化usart
    Usart3_Cfg(9600);
    LED2_ON;
    printf("初始化成功");
    
    // 定时器
    TIMx_NVIC_Config();
    TIM_Mode_Config();

    while(1){

    }
    
    return 0;
}

