#include <REGX52.H>
#include "../public/common.h"
//�������������ʾ 0~F �Ķ�������
u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};



/**
 * ���� LED ������
 * @param i ��������ĵڼ���LED (1~8)
 */
void setLedCom(u8 i){
    if(i>=1 && i<=8){
        i--;
        P2_2 = (i & 1<<0) > 0;
        P2_3 = (i & 1<<1) > 0;
        P2_4 = (i & 1<<2) > 0;
    }
}

int main(){
    int i=1;

    while(1){
        
        for(i=1; i<=8; i++){
            // ����������
            setLedCom(i);
            // ��ѡ��
            P0 = gsmg_code[i]; 

            // ����
            delay1ms(1);
            P0 = 0; 
        }

    };
    return 0;
}