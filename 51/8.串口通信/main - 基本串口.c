

// �����Ĵ���
#include <regx52.h>
void uart_init(unsigned char baud){
	TMOD |= 0x20; // ���ö�ʱ��ģʽ2��8λ�Զ���װ��ʱ��
	SCON=0x50; // ���ô���ģʽ1��0b01010000 �Ӹ�λ��ʼ 01�Ǵ���ģʽ1, 0�Ƕ��λ��1���������
	// PCON=0x80;// �ӱ������ﲻ��Ҫ�ӱ�
	TH1 = baud;
	TL1 = baud;
	TR1 = 1; // �򿪶�ʱ/������
	ES = 1; // �򿪴����ж�
	EA = 1; // �����ж�
}

// �жϺ���, 4Ϊ�����ж�
void uart() interrupt 4
{
	unsigned char r;
	RI = 0; // ��������ж�
	r = SBUF; // ��ȡ���յ�������
	SBUF = r+1; // ����+1�ŵ����ͼĴ���
	while(!TI); // �ȴ������������
	TI=0; // ������ͱ�־λ
}


void main(){
	uart_init();
	while(1);
}
