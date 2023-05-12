
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"



int main(){
    
    u8 rxBuf[256]={0};
    u32 rxCount = 0;
    
    
    // 初始化usart
    Usart_Cfg(9600);
    
    printf("初始化成功");


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
        //直接接收数据，不使用中断的时候（4. 配置中断控制器并使能USART接收中断；）使用，但是好像丢数，暂不做深究 
        chTemp = getchar();
        printf("-%c",chTemp);
        */
    }
    
    return 0;
}
