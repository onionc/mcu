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
u8 dvlData[]="\
:SA,-5.89,+4.33,0.00\r\n\n\
:TS,23030218083269,35.0, +21.0,0.2,1524.2,  0\r\n\n\
:BI,-32768,-32768,-32768,-32768,V\r\n\n\
:BS,-32768,-32768,-32768,V\r\n\n\
:BE,-32768,-32768,-32768,V\r\n\n\
:BD,+0.00,+0.00,+0.00,0.00,0.00\r\n\n\
:HM,G,D,0e8b,0ffe,*10.842,*1.974,*5.493\r\n\n\
";

u8 dvlData2[]="\
:SA,  -5.91,   +4.35,  0.00\r\n\n\
:TS,23030218083390,35.0, +14.6,   0.2,1505.4,  0\r\n\n\
:BI,    +0,    +0,    +0,-32768,A\r\n\n\
:BS,    +0,    +0,    +0,A\r\n\n\
:BE,    +0,    +0,    +0,A\r\n\n\
 :BD,        +0.00,        +0.00,        +0.00,   0.55,  0.00\r\n\n\
:HM,G,D,0e89,0ffe, 10.842, 1.974, 5.493\r\n\n\
";

void BASIC_TIM_IRQHandler(void){
    
    if(TIM_GetITStatus(BASIC_TIM, TIM_IT_Update) != RESET){
        
        Usart_SendStr(PRINT_USART, dvlData, 0);
        
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