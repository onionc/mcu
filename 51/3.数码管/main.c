#include <REGX52.H>
#include "../public/common.h"
//共阴极数码管显示 0~F 的段码数据
u8 gsmg_code[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};



/**
 * 设置 LED 公共端
 * @param i 从右至左的第几个LED (1~8)
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
            // 公共端设置
            setLedCom(i);
            // 段选端
            P0 = gsmg_code[i]; 

            // 消隐
            delay1ms(1);
            P0 = 0; 
        }

    };
    return 0;
}