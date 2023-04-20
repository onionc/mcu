#include <regx52.h>
#include "../public/common.h"
sbit LED = P2^0;
sbit KEY3 = P3^2; // 外部中断和P3.2连接

void init(){
	IT0 = 1; // 下降沿触发
	EX0 = 1; // INT0 
	EA = 1; // 总中断
}

// 中断函数, 0为外部中断0
void int0() interrupt 0
{
	
	delay1ms(10);
	if(!KEY3){
		LED = !LED;
	}
}

void main(){
	init();
	while(1);
}