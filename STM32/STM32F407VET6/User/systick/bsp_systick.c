#include "./systick/bsp_systick.h"

vu32 TimingDelay=0;

// ����ϵͳ��ʱ�� SysTick. 
ErrorStatus SysTick_Init(void){
    // SystemFreq/100 10ms �ж�һ�� (�������ٴ��ˣ�
    // SystemFreq/1000 1ms
    // SystemFreq/100000 10us
    // SystemFreq/1000000 1us
    
    if(SysTick_Config(SystemCoreClock/100000)){
        return ERROR;
    }
    return SUCCESS;
}

// ��ʱ���򣬣���SysTick_Init�����趨��ʱ��Ϊһ����λ��������10us��
void Delay_10us(vu32 times){
    TimingDelay = times;
    while(TimingDelay!=0);
}
