#include "uart.h"


unsigned char  xdata RX_Buffer[RX_BUF_MAX]={0};
bit Flag_Receive_OK=0, Flag_Send_Busy=0;

void Uart_Init(unsigned char baud){
	TMOD |= 0x20; // ���ö�ʱ��ģʽ2��8λ�Զ���װ��ʱ��
	SCON=0x50; // ���ô���ģʽ1��0b01010000 �Ӹ�λ��ʼ 01�Ǵ���ģʽ1, 0�Ƕ��λ��1 ��REN�������
	 PCON=0x80;// �ӱ������ﲻ��Ҫ�ӱ�
	TH1 = baud;
	TL1 = baud;
	
	ES = 1; // �򿪴����ж�
	EA = 1; // �����ж�
	TR1 = 1; // �򿪶�ʱ/������
}

void Send_String(unsigned char *p)				//�����ַ���
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

char putchar(char c)				//����һ���ַ�
{   
	while(Flag_Send_Busy); // �ȴ��������
	Flag_Send_Busy=1;
	SBUF=c; 
	// ���ж����жϺ����TI
	//while(!TI); // �ȴ������������
	//TI=0; // ������ͱ�־λ
	return c;
}



// �жϺ���, 4Ϊ�����ж�
void uart() interrupt 4
{
	static unsigned char  rIndex=0;
	if(RI){
		RI = 0; // ��������ж�
		if(Flag_Receive_OK==0){ // ��������ʱ�Ž��գ����û��ʱ������ᶪ������
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
		TI = 0; // ������ͱ�־λ
		Flag_Send_Busy = 0; // ���send busy��־��������
	}
	

}




















