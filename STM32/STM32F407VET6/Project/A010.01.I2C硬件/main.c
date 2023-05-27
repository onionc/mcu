
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./i2c/bsp_i2c_eeprom_h.h"


//�򵥵���ʱ����
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}

// i2c ��д����
#define BUF_SIZE 256
#define FirstPage 0x00 // ��ʼ��ַ
uint8_t wBuf[BUF_SIZE];
uint8_t rBuf[BUF_SIZE];
ErrorStatus I2C_Test(void){
    u16 i=0;
    printf("��ʼд������\n");
    
    // ׼������
    for(i=0; i<BUF_SIZE; i++){
        wBuf[i] = i%9; // %3��%9��Ϊ�˱��������ظ�д��� 256һ�ֻأ����������졣
        //printf("%02x", wBuf[i]);
    }
    printf("\n");
    // д����
    if(I2C_EE_WriteBytes(wBuf, FirstPage, BUF_SIZE)==ERROR){
        printf("д����ʧ��");
        return ERROR;
    }
    printf("д�������\n");
    //return SUCCESS;
    // ������
    if(I2C_EE_ReadBytes(FirstPage, rBuf, BUF_SIZE)==ERROR){
        printf("������ʧ��\n");
        return ERROR;
    }
    
    printf("�Ա�����\n");
    
    for(i=0; i<BUF_SIZE; i++){
        //printf("%02x", rBuf[i]);
        if(wBuf[i]!=rBuf[i]){
            printf("error:%d 0x%02X", rBuf[i]);
            return ERROR;
        }
    }
    
    printf("�������\n");
    
    return SUCCESS;
}

int main(){

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\n");

    
    // I2C ��ʼ��
    I2C_EE_INIT();
    
    if(I2C_Test()==SUCCESS){
        LED1_ON;
    }else{
        LED2_ON;
    }

    while(1){
        LED3_TOGGLE;
        Delay(0xffffff);
    }
    
    return 0;
}
