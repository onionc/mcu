#include "./led/bsp_led.h"


void LED_GPIO_Config(){
    // 定义GPIO_InitTypeDef结构体
    GPIO_InitTypeDef gpioInitStruct;
    
    // 开启LED的外设时钟
    RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK, ENABLE);
    
    // 要控制的引脚
    gpioInitStruct.GPIO_Pin = LED1_PIN;
    // 引脚为输出模式
    gpioInitStruct.GPIO_Mode = GPIO_Mode_OUT;
    // 输出模式为推挽
    gpioInitStruct.GPIO_OType  = GPIO_OType_PP;
    // 上拉模式
    gpioInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    // 引脚速率 2M
    gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    
    // 使用函数库初始化gpio
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
    
    // 初始化 led2
    gpioInitStruct.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
        
    // 初始化 led3
    gpioInitStruct.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED1_GPIO_PORT, &gpioInitStruct);
   
}
