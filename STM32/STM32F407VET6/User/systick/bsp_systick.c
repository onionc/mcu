#include "./systick/bsp_systick.h"

vu32 TimingDelay=0;

// 启动系统定时器 SysTick. 
ErrorStatus SysTick_Init(void){
    // SystemFreq/100 10ms 中断一次 (好像不能再大了）
    // SystemFreq/1000 1ms
    // SystemFreq/100000 10us
    // SystemFreq/1000000 1us
    
    if(SysTick_Config(SystemCoreClock/100000)){
        return ERROR;
    }
    return SUCCESS;
}

// 延时程序，（以SysTick_Init函数设定的时间为一个单位，现在是10us）
void Delay_10us(vu32 times){
    TimingDelay = times;
    while(TimingDelay!=0);
}
