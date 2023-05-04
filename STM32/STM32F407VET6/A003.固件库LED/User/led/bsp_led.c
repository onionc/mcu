#include "./led/bsp_led.h"


void LED_GPIO_Config(){
    // ����GPIO_InitTypeDef�ṹ��
    GPIO_InitTypeDef gpioInitStruct;
    
    // ����LED������ʱ��
    RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
    
    // Ҫ���Ƶ�����
    gpioInitStruct.GPIO_Pin = LED1_PIN;
    // ����Ϊ���ģʽ
    gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
    // ���ģʽΪ����
    gpioInitStruct.GPIO_OType  = GPIO_OType_PP;
    // ����ģʽ
    gpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    // �������� 2M
    gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    
    // ʹ�ú������ʼ��gpio
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
    
    // ��ʼ�� led2
    gpioInitStruct.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
        
    // ��ʼ�� led3
    gpioInitStruct.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
   
}
