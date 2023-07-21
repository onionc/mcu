// 1.�ֶ����մ���
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./tim/bsp_basic_tim.h"
#include "./adc/bsp_adc.h"

float carAdcValue=0;


RCC_ClocksTypeDef get_rcc;
int main(){
    char chTemp;
    u8 rbuf[256];
    SystemCoreClockUpdate();
    
    RCC_GetClocksFreq(&get_rcc);
    // LED
    LED_GPIO_Config();
    LED1_OFF;LED2_OFF;LED3_OFF; // led1 ��ʱ������
    
    
    // ��ʼ��usart
    Usart3_Cfg(115200);
    printf("��ʼ���ɹ�");
    
    // ��ʱ��
    TIMx_NVIC_Config();
    TIM_Mode_Config(10); // ���ö�ʱ��
    
    // adc
    
    ADC_GPIO_Config();
    ADC_Mode_Config();
    ADC_NVIC_Config();
    
    while(1){

    }
    
    return 0;
}

