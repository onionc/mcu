// 使用封装的串口
#include <regx52.h>
#include "../public/uart/uart.h"
#include "../public/common.h"

void main(){
	
	int i=0;
	Uart_Init(0xFA); // 4800 倍频=9600
	delay1ms(500);
	printf("你好");
	for(;i<RX_BUF_MAX; i++){
		RX_Buffer[i]=1;
	}
	while(1){
		putchar('a');
		if(Flag_Receive_OK){
			printf("收到消息 %s", RX_Buffer);
			Flag_Receive_OK=0;
		}
		delay1ms(500);
	}
	
}