#include "stm32f4xx.h"

// 点灯 hello world：使用寄存器点灯

#define LED1 13
#define LED2 14
#define LED3 15

int main(){
    
    // 开启GPIOE的时钟
    RCC_AHB1ENR |= (1<<4);
    
    
    /* 配置LED1 */
    
    // 配置 GPIO 模式
    // gpioe moder
    GPIOE_MODER &= ~(0x03 << 2*LED1); // 清空两位，3是11，用来清空这两位
    GPIOE_MODER |= (1 << 2*LED1); // 0b01 输出模式
    
    // 配置 输出类型
    GPIOE_OTYPER &= ~(1<<LED1); // 清空
    GPIOE_OTYPER |= (0<<LED1); // 0 推挽输出
    
    // 配置 输出速度，此处可省略
    GPIOE_OSPEEDR &= ~(0x03 << 2*LED1); // 清空
    GPIOE_OSPEEDR &= (0 << 2*LED1); // 0 速率2MHz
    
    // 配置 上下拉模式，能小幅度提高输出电流能力，此处可省略
    GPIOE_PUPDR &= ~(0x03 << 2*LED1); // 清空
    GPIOE_PUPDR &= (1 << 2*LED1); //0b01 上拉模式
    
    
    // 控制引脚输出电平
    GPIOE_BSRR |= (1<<16<<LED1); // 置低电平
    //GPIOE_BSRR |= (1<<LED1); // 置高电平

    while(1);
    
    return 0;
}

// STM32 初始化系统时钟的函数
void SystemInit(){
    
}