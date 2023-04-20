
#include <REGX52.H>
#include <intrins.h>

/**
 * 带参数的延时函数，延时100ms的倍数
 * @param num 指定倍数
 */
void delay100Ms(unsigned char num)		//@12.000MHz
{
    while(num--){
        unsigned char i, j;
    
    	i = 195;
    	j = 138;
    	do
    	{
    		while (--j);
    	} while (--i);
    }
}


int main(){
    unsigned char r = 0;
    unsigned p2 = 0xFE;
    
    while(1){
        P2 = p2;
        if(r==1){  
            p2 = _cror_(p2, 1);
        }else{
            p2 = _crol_(p2, 1);
        }

        if(p2==0xFE) r=0;
        if(p2==0x7F) r=1;
        delay100Ms(2);
    }
    return 0;
}
