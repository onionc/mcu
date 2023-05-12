
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"



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
        
        
        /* 
        //ֱ�ӽ������ݣ���ʹ���жϵ�ʱ��4. �����жϿ�������ʹ��USART�����жϣ���ʹ�ã����Ǻ��������ݲ���� 
        chTemp = getchar();
        printf("-%c",chTemp);
        */
    }
    
    return 0;
}
