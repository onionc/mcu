#include "./key/bsp_key.h"

// ����GPIO����
void Key_GPIO_Config(){
    GPIO_InitTypeDef gpioInitStruct;
    
    // ��������GPIO��ʱ��
    RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK, ENABLE);
    
    
    // ����Ϊ����ģʽ
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
    // ������������������
    gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    // �������Ų���ʼ������1
    gpioInitStruct.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    // ��ʼ������2
    gpioInitStruct.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    // ��ʼ������3
    gpioInitStruct.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    
    
}



/**
  * @brief ����Ƿ��а�������
  * @param gpiox �����GPIO
  * @param pin �������һλ��0~15��
  * @return ����״̬ KEY_ON ���£�KEY_OFF δ����
*/
uint8_t Key_Scan(GPIO_TypeDef* gpiox, uint16_t pin){
    // ��ⰴ���Ƿ���
    if(GPIO_ReadInputDataBit(gpiox, pin) == KEY_ON){
        // �����������Ҫ��������Ҫ����delay�������е��ݣ���ֱ�Ӷ�ȡ��������
        
        // �ȴ������ͷ�
        while(GPIO_ReadInputDataBit(gpiox, pin) == KEY_ON);
        return KEY_ON;
    }
    return KEY_OFF;
}
