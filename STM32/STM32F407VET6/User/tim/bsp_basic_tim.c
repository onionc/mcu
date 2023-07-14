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

// 小车adc值
extern float carAdcValue;
unsigned char data[3]={0x88, 0x77};

void BASIC_TIM_IRQHandler(void){
    
    if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET){
        
        // 判断adc采集的电压。本来有左右遥控，高电平时3.3v，正常是0.9v。现在发现可以检测一路就可判断两路，正常情况0.9v都没按, 3.3v按了L，0v按了R。
        if(carAdcValue<0.5){
            // L
            data[2] = 'L';
        }else if(carAdcValue>2.5){
            // R
            data[2] = 'R';
        }else{
            // 未按任何按钮
            data[2] = '0';
        }
        
        Usart_SendStr(PRINT_USART, data, 3);
        
        LED1_TOGGLE;
        TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
    }
    
}

void TIM_Mode_Config(unsigned int freq){
    TIM_TimeBaseInitTypeDef timerInit;
    
    if(freq>=10000){
        return;
    }
    
    // 开启 clk
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);
    
    // 预分频器 
    timerInit.TIM_Prescaler = 8400-1; // timxClk = 84MHz，TIM_Prescaler=8400-1时，定时器频率为 timxClk/(TIM_Prescaler+1)=10000Hz
    
    // 如果上面是10000hz，要设定2hz(0.5s)，数值设定5000次一个定时周期
    timerInit.TIM_Period = (10000/freq)-1;
    
    // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &timerInit);
    
    // 开启定时器更新中断
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    
    // 使能定时器
    TIM_Cmd(BASIC_TIM, ENABLE);
}