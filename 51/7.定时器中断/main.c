#include <regx52.h>

sbit LED = P2^0;
sbit KEY3 = P3^2; // 外部中断和P3.2连接

#define TH_VALUE 0XFC
#define TL_VALUE 0X66

void time0_init(){
	TMOD |= 0x01; // 定时器0设置工作方式1（16位定时器）
	TH0 = TH_VALUE; // 填值
	TL0 = TL_VALUE;
	ET0 = 1; // 定时器0
	EA = 1; // 总中断
	TR0 = 1; // 打开定时器
}

// 中断函数, 1为timer0
void timer0() interrupt 1
{
	static int i=0;
	i++;
	TH0 = TH_VALUE; // 填值
	TL0 = TL_VALUE;
	
	
	if(i>=1000){
		i=0;
		LED = !LED;
	}
}

void main(){
	time0_init();
	while(1);
}