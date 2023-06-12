
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"


//简单的延时函数
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}



int main(){

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart_Cfg(9600);
    printf("串口初始化成功\n");

    while(1){
        LED3_TOGGLE;
        Delay(0xffffff);
    }
    
    return 0;
}
