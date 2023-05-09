
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./systick/bsp_systick.h"

void delay(vu32 count){
    for(; count!=0; count--);
}

// ϵͳ��ʱ���жϷ�������������Ŀ�У���Ӱ�칤�̵�stm32f4xx_it.c
void SysTick_Handler(void)
{
    if(TimingDelay != 0){ 
        TimingDelay--;
    }
}

int main(){
    
    // led ��ʼ��
    LED_GPIO_Config();
    
    // Ĭ��ȫ��
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    
    // ���� SysTick
    if(SysTick_Init()==ERROR){
        LED1_ON; LED2_ON; LED3_ON;
        return 1;
    }



    while(1){
        Delay_10us(100000); // 100000 * 10us = 1000ms
        LED1_TOGGLE;
    }
    
    return 0;
}
