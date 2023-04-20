#include <regx52.h>

sbit LED = P2^0;
sbit KEY3 = P3^2; // �ⲿ�жϺ�P3.2����

#define TH_VALUE 0XFC
#define TL_VALUE 0X66

void time0_init(){
	TMOD |= 0x01; // ��ʱ��0���ù�����ʽ1��16λ��ʱ����
	TH0 = TH_VALUE; // ��ֵ
	TL0 = TL_VALUE;
	ET0 = 1; // ��ʱ��0
	EA = 1; // ���ж�
	TR0 = 1; // �򿪶�ʱ��
}

// �жϺ���, 1Ϊtimer0
void timer0() interrupt 1
{
	static int i=0;
	i++;
	TH0 = TH_VALUE; // ��ֵ
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