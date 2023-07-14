#include "./adc/bsp_adc.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

void ADC_GPIO_Config(void){
    GPIO_InitTypeDef gi;
    
    // ʹ�� GPIO ʱ��
    RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK1, ENABLE);
    
    // ���� IO
    gi.GPIO_Pin = ADC_GPIO_PIN1;
    gi.GPIO_Mode = GPIO_Mode_AIN; // ģ������
    gi.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��������������
    
    GPIO_Init(ADC_GPIO_PORT1, &gi);
}

void ADC_Mode_Config(void){
    ADC_InitTypeDef ai;
    ADC_CommonInitTypeDef aci;

    // ���� ADC ʱ��
    RCC_APB2PeriphClockCmd(ADC_CLK, ENABLE);
    
    /* -------------ADC Common-------------- */
    
    aci.ADC_Mode = ADC_Mode_Independent; // ����ADCģʽ
    aci.ADC_Prescaler = ADC_Prescaler_Div8; // fpclk��Ƶ
    aci.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // ��ֹDMA
    aci.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles; // ����ʱ����
    ADC_CommonInit(&aci);
    
    /* -------------ADC init �ṹ�� -------- */
    ai.ADC_Resolution = ADC_Resolution_12b; // ADC �ֱ���
    ai.ADC_ScanConvMode = DISABLE;  // ��ֹɨ��ģʽ
    ai.ADC_ContinuousConvMode = ENABLE; // ����ת��
    ai.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ֹ�ⲿ���ش���
    ai.ADC_DataAlign = ADC_DataAlign_Right; // �����Ҷ���
    ai.ADC_NbrOfConversion = 1; // ת��ͨ��1
    ADC_Init(ADC_NUM, &ai);
    
    // ���� ADC ͨ��ת��˳��Ͳ�������
    ADC_RegularChannelConfig(ADC_NUM, ADC_CHANNEL1, 1, ADC_SampleTime_56Cycles);
    
    // ADC ת�����������ж�
    ADC_ITConfig(ADC_NUM, ADC_IT_EOC, ENABLE);
    // ʹ�� ADC
    ADC_Cmd(ADC_NUM, ENABLE);
    // ��ʼ ADC ת�����������
    ADC_SoftwareStartConv(ADC_NUM);
    
}

void ADC_NVIC_Config(void){
    NVIC_InitTypeDef NVIC_init;
    // �ж���0 ֻ��Ҫִ��һ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // �ж���Դ
    NVIC_init.NVIC_IRQChannel = ADC_IRQ;
    
    // ��ռ���ȼ�
    NVIC_init.NVIC_IRQChannelPreemptionPriority = 1;
    
    // �����ȼ�
    NVIC_init.NVIC_IRQChannelSubPriority = 1;
    NVIC_init.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_init);
}

void ADC_IRQHandler(void){
    int a;
    if(ADC_GetITStatus(ADC_NUM, ADC_IT_EOC)==SET){
        // ��ȡadc��ת��ֵ
        a = ADC_GetConversionValue(ADC_NUM);
        printf("adc = %f\n", a*3.3/4096);
        LED3_ON;
    }
    ADC_ClearITPendingBit(ADC_NUM, ADC_IT_EOC);
}