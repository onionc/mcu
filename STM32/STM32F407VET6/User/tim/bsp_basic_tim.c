#include "bsp_basic_tim.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

void TIMx_NVIC_Config(void){
    NVIC_InitTypeDef NVIC_init;
    // �ж���0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    // �ж���Դ
    NVIC_init.NVIC_IRQChannel = BASIC_TIM_IRQn;
    
    // ��ռ���ȼ�
    NVIC_init.NVIC_IRQChannelPreemptionPriority = 0;
    
    // �����ȼ�
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
    
    // ���� clk
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);
    
    // Ԥ��Ƶ�� 
    timerInit.TIM_Prescaler = 8400-1; // timxClk = 84MHz��TIM_Prescaler=8400-1ʱ����ʱ��Ƶ��Ϊ timxClk/(TIM_Prescaler+1)=10000Hz
    
    // ���������10000hz���趨5000��һ����ʱ���ڣ������0.5s��2hz)
    timerInit.TIM_Period = 50-1;
    
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &timerInit);
    
    // ������ʱ�������ж�
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    
    // ʹ�ܶ�ʱ��
    TIM_Cmd(BASIC_TIM, ENABLE);
}