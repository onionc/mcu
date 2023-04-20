#include <REGX52.H>

typedef unsigned char u8;
sbit K1 = P3^0;
sbit K2 = P3^1;
sbit K3 = P3^2;
sbit K4 = P3^3;



void Delay1ms(unsigned int nums)
{
    while(nums--){
        unsigned char i, j;

        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}



bit down(u8 index){
    bit i;
    switch(index){
        case 1:
            // 判断第一个按键是否按下
            //i = (P3_0==1?( (Delay1ms(10); (P3_0==1?1:0)) ):0);
            break;
        case 2:
            //i = CHECK(P3_1);
            break;
        case 3:
           // i = CHECK(P3_2);
            break;
        case 4:
           // i = CHECK(P3_3);
            break;
        default: break;
    }


    return i;
}





int main(){
	while(1){
         if(P3_1==0){
            Delay1ms(10); // 消抖
            while(P3_1==0); // 等待抬起
            Delay1ms(10);
            P2_0=~P2_0;
         }
    }
    return 0;
}