
#include "stm32f4xx.h"
#include "led/bsp_led.h"

int main(){
    LED_GPIO_Config();
    
    // ʹ�ÿ⺯����
    //LED1(ON);
    //LED2(OFF);
    
    // ʹ�üĴ�����
    LED1_OFF;
    LED2_ON;
    
    // ��ת
    LED1_TOGGLE;

    while(1);
    return 0;
}
