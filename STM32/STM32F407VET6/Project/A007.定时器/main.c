
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./key/bsp_exti_key.h"

void delay(__IO uint32_t count){
    for(; count!=0; count--);
}

void KEY1_IRQHandler(void){
    // ȷ�ϲ������ж�
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET){
        LED1_TOGGLE;
        
        // ����жϱ�־λ
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

int main(){
    
    // led ��ʼ��
    LED_GPIO_Config();
    
    // Ĭ��ȫ��
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��EXTI�ж�
    EXTI_Key_Cfg();


    while(1){
        
    }
    
    return 0;
}
