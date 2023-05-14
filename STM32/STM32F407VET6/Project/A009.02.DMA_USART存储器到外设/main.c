
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart_dma.h"

//简单的延时函数
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}

int main(){

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
   
    // 初始化usart
    Usart_Cfg(9600);
    
    // 配置使用DMA
    if(DMA_Cfg()==ERROR){
        printf("初始化失败");
    }else{
        printf("初始化成功");
    }
    
    // USART1 向 DMA发出TX请求
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);
    
    /* todo: rx未添加
    USART_DMACmd(USARTx, USART_DMAReq_Rx, ENABLE);
    */
    
    

    while(1){
        // 运行其他任务，改发送的值
        LED1_TOGGLE;
        Delay(0xffffff);
        
        SendBuf[3]=0x0a;
        
    }
    
    return 0;
}
