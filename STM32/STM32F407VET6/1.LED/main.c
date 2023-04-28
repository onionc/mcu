#include "stm32f4xx.h"

// ��� hello world��ʹ�üĴ������

#define LED1 13
#define LED2 14
#define LED3 15

int main(){
    
    // ����GPIOE��ʱ��
    RCC_AHB1ENR |= (1<<4);
    
    
    /* ����LED1 */
    
    // ���� GPIO ģʽ
    // gpioe moder
    GPIOE_MODER &= ~(0x03 << 2*LED1); // �����λ��3��11�������������λ
    GPIOE_MODER |= (1 << 2*LED1); // 0b01 ���ģʽ
    
    // ���� �������
    GPIOE_OTYPER &= ~(1<<LED1); // ���
    GPIOE_OTYPER |= (0<<LED1); // 0 �������
    
    // ���� ����ٶȣ��˴���ʡ��
    GPIOE_OSPEEDR &= ~(0x03 << 2*LED1); // ���
    GPIOE_OSPEEDR &= (0 << 2*LED1); // 0 ����2MHz
    
    // ���� ������ģʽ����С���������������������˴���ʡ��
    GPIOE_PUPDR &= ~(0x03 << 2*LED1); // ���
    GPIOE_PUPDR &= (1 << 2*LED1); //0b01 ����ģʽ
    
    
    // �������������ƽ
    GPIOE_BSRR |= (1<<16<<LED1); // �õ͵�ƽ
    //GPIOE_BSRR |= (1<<LED1); // �øߵ�ƽ

    while(1);
    
    return 0;
}

// STM32 ��ʼ��ϵͳʱ�ӵĺ���
void SystemInit(){
    
}