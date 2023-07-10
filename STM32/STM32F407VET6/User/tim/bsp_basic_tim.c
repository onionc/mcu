#include "bsp_basic_tim.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

void TIMx_NVIC_Config(void){
    NVIC_InitTypeDef NVIC_init;
    // 中断组0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    // 中断来源
    NVIC_init.NVIC_IRQChannel = BASIC_TIM_IRQn;
    
    // 抢占优先级
    NVIC_init.NVIC_IRQChannelPreemptionPriority = 0;
    
    // 子优先级
    NVIC_init.NVIC_IRQChannelSubPriority = 3;
    NVIC_init.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_init);
    
}

void BASIC_TIM_IRQHandler(void){
    if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET){
        LED1_TOGGLE;
        Usart_SendByte(PRINT_USART, 0xAA);
        TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
    }
}

void TIM_Mode_Config(void){
    TIM_TimeBaseInitTypeDef timerInit;
    
    // 开启 clk
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);
    
    // 预分频器 
    timerInit.TIM_Prescaler = 8400-1; // timxClk = 84MHz，TIM_Prescaler=8400-1时，定时器频率为 timxClk/(TIM_Prescaler+1)=10000Hz
    
    // 如果上面是10000hz，设定5000次一个定时周期，这就是0.5s（2hz)
    timerInit.TIM_Period = 50-1;
    
    // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &timerInit);
    
    // 开启定时器更新中断
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    
    // 使能定时器
    TIM_Cmd(BASIC_TIM, ENABLE);
}