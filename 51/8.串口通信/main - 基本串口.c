

// 基本的串口
#include <regx52.h>
void uart_init(unsigned char baud){
	TMOD |= 0x20; // 设置定时器模式2，8位自动重装定时器
	SCON=0x50; // 设置串口模式1，0b01010000 从高位开始 01是串口模式1, 0是多机位否，1是允许接收
	// PCON=0x80;// 加倍，这里不需要加倍
	TH1 = baud;
	TL1 = baud;
	TR1 = 1; // 打开定时/计数器
	ES = 1; // 打开串口中断
	EA = 1; // 打开总中断
}

// 中断函数, 4为串口中断
void uart() interrupt 4
{
	unsigned char r;
	RI = 0; // 清除接收中断
	r = SBUF; // 获取接收到的数据
	SBUF = r+1; // 数据+1放到发送寄存器
	while(!TI); // 等待发送数据完成
	TI=0; // 清除发送标志位
}


void main(){
	uart_init();
	while(1);
}
