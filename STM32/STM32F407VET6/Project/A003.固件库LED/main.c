
#include "stm32f4xx.h"
#include "led/bsp_led.h"

int main(){
    LED_GPIO_Config();
    
    // ʹ�ÿ⺯����
    //LED1(ON);
    //LED2(OFF);
    //LED3(OFF);
    
    // ʹ�üĴ�����
    LED1_OFF;
    LED2_ON;
    LED3_OFF;
    
    // ��ת
    LED1_TOGGLE;

    while(1);
    return 0;
}
