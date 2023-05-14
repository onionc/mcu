
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart_dma.h"

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
    
    // ����ʹ��DMA
    if(DMA_Cfg()==ERROR){
        printf("��ʼ��ʧ��");
    }else{
        printf("��ʼ���ɹ�");
    }
    
    // USART1 �� DMA����TX����
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
    
    /* todo: rxδ���
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    */
    
    

    while(1){
        // �����������񣬸ķ��͵�ֵ
        LED1_TOGGLE;
        Delay(0xffffff);
        
        SendBuf[3]=0x0a;
        
    }
    
    return 0;
}
