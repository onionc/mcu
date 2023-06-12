
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"


//�򵥵���ʱ����
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}



int main(){

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\n");

    while(1){
        LED3_TOGGLE;
        Delay(0xffffff);
    }
    
    return 0;
}
