#include "./key/bsp_exti_key.h"

/**
 * @brief ����Ƕ�������жϿ����� NVIC
 * 
 */
static void NVIC_Cfg(void){
    NVIC_InitTypeDef nvicInit;
    
    // ����NVIC���ȼ�����Ϊ1(1λ�����ȼ���3λ�����ȼ�)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // �����ж�ԴΪ����1 
    nvicInit.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
    // ���������ȼ�����ռ���ȼ���
    nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
    // ���������ȼ�
    nvicInit.NVIC_IRQChannelSubPriority = 1;
    // ʹ���ж�
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    // ��ʼ��
    NVIC_Init(&nvicInit);
    
    // ���ð���2
    //nvicInit.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    //NVIC_Init(&nvicInit);
  
}

/**
 * @brief ���� EXTI �ж�
- ���� NVIC
- ��������GPIOʱ�Ӻ�SYSCFGʱ��
- ���ð���GPIOΪ����ģʽ
- ������GPIO���ӵ�EXTI����Դ
- ���ð���EXTI�ж�
 */
void EXTI_Key_Cfg(void){
    GPIO_InitTypeDef gpioInit;
    EXTI_InitTypeDef extiInit;
    
    // ���� NVIC
    NVIC_Cfg();
    
    // �������� GPIO�ڵ�ʱ��
    RCC_AHB1PeriphClockCmd(KEY1_INT_CLK, ENABLE);
    
    // ʹ�� SYSCFG ʱ�ӣ�ʹ���ⲿGPIO�жϱ���ʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // ���ð��� GPIO
    gpioInit.GPIO_Pin = KEY1_INT_PIN; // ����1����
    gpioInit.GPIO_Mode = GPIO_Mode_IN; // ����Ϊ����ģʽ
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL; // ������Ҳ������
    GPIO_Init(KEY1_INT_PORT, &gpioInit);
    
    // ������GPIO���ӵ�EXTI����Դ
    SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    
    // ���� exti �ж�
    extiInit.EXTI_Line = KEY1_INT_EXTI_LINE; // ѡ���ж�Դ
    extiInit.EXTI_Mode = EXTI_Mode_Interrupt; // ģʽΪ�ж�ģʽ
    extiInit.EXTI_Trigger = EXTI_Trigger_Rising; // �½��ش���
    extiInit.EXTI_LineCmd = ENABLE; // ʹ��
    EXTI_Init(&extiInit);
    
    
    
    
    
    
    
    
    
}





