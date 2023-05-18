
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./i2c/bsp_i2c_eeprom_h.h"
#include "./usart/bsp_usart.h"

//�򵥵���ʱ����
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}

int main(){

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�");

    
    // I2C ��ʼ��
    I2C_EE_INIT();

    while(1){
        // �����������񣬸ķ��͵�ֵ
        LED1_TOGGLE;
        Delay(0xffffff);
    }
    
    return 0;
}
