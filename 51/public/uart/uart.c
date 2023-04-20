#include "uart.h"


unsigned char  xdata RX_Buffer[RX_BUF_MAX]={0};
bit Flag_Receive_OK=0, Flag_Send_Busy=0;

void Uart_Init(unsigned char baud){
	TMOD |= 0x20; // 设置定时器模式2，8位自动重装定时器
	SCON=0x50; // 设置串口模式1，0b01010000 从高位开始 01是串口模式1, 0是多机位否，1 是REN允许接收
	 PCON=0x80;// 加倍，这里不需要加倍
	TH1 = baud;
	TL1 = baud;
	
	ES = 1; // 打开串口中断
	EA = 1; // 打开总中断
	TR1 = 1; // 打开定时/计数器
}

void Send_String(unsigned char *p)				//发送字符串
{	
 	while(*p)
 	{
	 	SBUF=*(p++);
		while(TI==0)
		{
			;
			;
			;
		};
		TI=0;
	}   
}

char putchar(char c)				//发送一个字符
{   
	while(Flag_Send_Busy); // 等待接收完毕
	Flag_Send_Busy=1;
	SBUF=c; 
	// 在中断中判断和清除TI
	//while(!TI); // 等待发送数据完成
	//TI=0; // 清除发送标志位
	return c;
}



// 中断函数, 4为串口中断
void uart() interrupt 4
{
	static unsigned char  rIndex=0;
	if(RI){
		RI = 0; // 清除接收中断
		if(Flag_Receive_OK==0){ // 接收允许时才接收，如果没计时处理，则会丢弃数据
			RX_Buffer[rIndex++] = SBUF;
			if(RX_Buffer[rIndex-1]=='\n' || rIndex>=RX_BUF_MAX-1){
				RX_Buffer[rIndex]=0;
				rIndex=0;
				Flag_Receive_OK = 1;
			}
		}
	}

	if(TI);
	{
		TI = 0; // 清除发送标志位
		Flag_Send_Busy = 0; // 清除send busy标志，允许发送
	}
	

}




















