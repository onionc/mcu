// 1.�ֶ����մ���
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

int main(){
    char chTemp;
    u8 rbuf[256];
    
    // ��ʼ��usart
    Usart_Cfg(9600);
    
    printf("��ʼ���ɹ�");


    while(1){
        //ֱ�ӽ������ݣ���ʹ���ж�Դ����
        
        if(scanf("%s", rbuf)){
            printf("%s", rbuf);
        }
        
        //chTemp = getchar();
        //printf("%c",chTemp);
    }
    
    return 0;
}

/*
// 2.ʹ���жϽ��մ���
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart_interrupt.h"



int main(){
    
    u8 rxBuf[256]={0};
    u32 rxCount = 0;
    
    
    // ��ʼ��usart
    Usart_Cfg(9600);
    
    printf("��ʼ���ɹ�");


    while(1){
        
        if(RxFlag){
            if(rxCount < sizeof(rxBuf)){
                rxBuf[rxCount++] = chTemp;
            }else{
                rxCount = 0;
            }
            
            if(chTemp==0x0a){
                Usart_SendStr(USARTx, "received: ", 0);
                Usart_SendStr(USARTx, rxBuf, rxCount);
                
                rxCount = 0;
            }
            RxFlag = 0;
        }
        
        
       
    }
    
    return 0;
}
*/


 

        
