/**
 * ֱ�����
 * ���������ֹͣ
 *
 */

#include <REGX51.H>
#include "../public/common.h"

sbit DC_MOTOR = P1^0;

int main(){
    
    while(1){
        DC_MOTOR = 1; // �������
        delay1ms(1000);
        DC_MOTOR = 0; // �رյ��
        delay1ms(2000);
    }

    
    return 0;
}