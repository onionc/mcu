/**
 * 直流电机
 * 运行五秒后停止
 *
 */

#include <REGX51.H>
#include "../public/common.h"

sbit DC_MOTOR = P1^0;

int main(){
    
    while(1){
        DC_MOTOR = 1; // 开启电机
        delay1ms(1000);
        DC_MOTOR = 0; // 关闭电机
        delay1ms(2000);
    }

    
    return 0;
}