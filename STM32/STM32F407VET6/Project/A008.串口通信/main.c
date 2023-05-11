
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"



int main(){
    
    u8 rxBuf[256]={0};
    u32 rxCount = 0;
    
    
    // ≥ı ºªØusart
    Usart_Cfg(9600);
    
    printf("123");


    while(1){

    }
    
    return 0;
}
