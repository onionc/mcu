#include <regx52.h>
#include "../public/common.h"
sbit LED = P2^0;
sbit KEY3 = P3^2; // �ⲿ�жϺ�P3.2����

void init(){
	IT0 = 1; // �½��ش���
	EX0 = 1; // INT0 
	EA = 1; // ���ж�
}

// �жϺ���, 0Ϊ�ⲿ�ж�0
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