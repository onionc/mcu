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

// С��adcֵ
extern float carAdcValue;
unsigned char data[3]={0x88, 0x77};

void BASIC_TIM_IRQHandler(void){
    
    if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET){
        
        // �ж�adc�ɼ��ĵ�ѹ������������ң�أ��ߵ�ƽʱ3.3v��������0.9v�����ڷ��ֿ��Լ��һ·�Ϳ��ж���·���������0.9v��û��, 3.3v����L��0v����R��
        if(carAdcValue<0.5){
            // L
            data[2] = 'L';
        }else if(carAdcValue>2.5){
            // R
            data[2] = 'R';
        }else{
            // δ���κΰ�ť
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
    
    // ���� clk
    RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE);
    
    // Ԥ��Ƶ�� 
    timerInit.TIM_Prescaler = 8400-1; // timxClk = 84MHz��TIM_Prescaler=8400-1ʱ����ʱ��Ƶ��Ϊ timxClk/(TIM_Prescaler+1)=10000Hz
    
    // ���������10000hz��Ҫ�趨2hz(0.5s)����ֵ�趨5000��һ����ʱ����
    timerInit.TIM_Period = (10000/freq)-1;
    
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &timerInit);
    
    // ������ʱ�������ж�
    TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);
    
    // ʹ�ܶ�ʱ��
    TIM_Cmd(BASIC_TIM, ENABLE);
}