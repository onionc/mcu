#include "./adc/bsp_adc.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

void ADC_GPIO_Config(void){
    GPIO_InitTypeDef gi;
    
    // 使能 GPIO 时钟
    RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK1, ENABLE);
    
    // 配置 IO
    gi.GPIO_Pin = ADC_GPIO_PIN1;
    gi.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
    gi.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上拉，不下拉
    
    GPIO_Init(ADC_GPIO_PORT1, &gi);
}

void ADC_Mode_Config(void){
    ADC_InitTypeDef ai;
    ADC_CommonInitTypeDef aci;

    // 开启 ADC 时钟
    RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
    
    /* -------------ADC Common-------------- */
    
    aci.ADC_Mode = ADC_Mode_Independent; // 独立ADC模式
    aci.ADC_Prescaler = ADC_Prescaler_Div8; // fpclk分频
    aci.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // 禁止DMA
    aci.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; // 采样时间间隔
    ADC_CommonInit(&aci);
    
    /* -------------ADC init 结构体 -------- */
    ai.ADC_Resolution = ADC_Resolution_12b; // ADC 分辨率
    ai.ADC_ScanConvMode = DISABLE;  // 禁止扫描模式
    ai.ADC_ContinuousConvMode = ENABLE; // 连续转换
    ai.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止外部边沿触发
    ai.ADC_DataAlign = ADC_DataAlign_Right; // 数据右对齐
    ai.ADC_NbrOfConversion = 1; // 转换通道1
    ADC_Init(ADC_NUM, &ai);
    
    // 配置 ADC 通道转换顺序和采样周期
    ADC_RegularChannelConfig(ADC_NUM, ADC_CHANNEL1, 1, ADC_SampleTime_56Cycles);
    
    // ADC 转换结束产生中断
    ADC_ITConfig(ADC_NUM, ADC_IT_EOC, ENABLE);
    // 使能 ADC
    ADC_Cmd(ADC_NUM, ENABLE);
    // 开始 ADC 转换，软件触发
    ADC_SoftwareStartConv(ADC_NUM);
    
}

void ADC_NVIC_Config(void){
    NVIC_InitTypeDef NVIC_init;
    // 中断组0 只需要执行一次
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // 中断来源
    NVIC_init.NVIC_IRQChannel = ADC_IRQ;
    
    // 抢占优先级
    NVIC_init.NVIC_IRQChannelPreemptionPriority = 1;
    
    // 子优先级
    NVIC_init.NVIC_IRQChannelSubPriority = 1;
    NVIC_init.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_init);
}

void ADC_IRQHandler(void){
    int a;
    if(ADC_GetITStatus(ADC_NUM, ADC_IT_EOC)==SET){
        // 读取adc的转换值
        a = ADC_GetConversionValue(ADC_NUM);
        printf("adc = %f\n", a*3.3/4096);
        LED3_ON;
    }
    ADC_ClearITPendingBit(ADC_NUM, ADC_IT_EOC);
}