#include <AI8051U.H>
#include <intrins.h>


// led
#define LED7    P10
#define LED8    P11
#define LED9    P12

// 按键
#define SW3     P13
#define SW4     P16
#define SW5     P17


void Delay10ms(unsigned int times)	//@11.0592MHz
{
    unsigned char data i, j;
    while(times-->0){
        
        i = 108;
        j = 145;
        do
        {
            while (--j);
        } while (--i);
    }
}



void Delay500ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

// 按键检测，0无按键，3 4 5 对应按键3 4 5
unsigned char keyCheck(){
    if(SW3==1){
        Delay10ms(2);
        while(SW3==1);
        Delay10ms(1);
        return 3;
    }
    if(SW4==1){
        Delay10ms(2);
        while(SW4==1);
        Delay10ms(1);
        return 4;
    }
    if(SW5==1){
        Delay10ms(2);
        while(SW5==1);
        Delay10ms(1);
        return 5;
    }
    return 0;
    
}


int main(){
    unsigned char keyIndex=0;
    // 配置P1口模式：准双向，弱上拉
    P1M0 = 0x00;
    P1M1 = 0x00;
    
    // 初始值
    LED7 = 0;
    LED8 = 0;
    LED9 = 0;
    SW3=0;
    SW4=0;
    SW5=0;
    
    
        
    while(1){
        keyIndex = keyCheck();
        switch(keyIndex){
            case 3:
                LED7 ^= 1; 
            break;
            case 4:
                LED8 ^= 1; 
            break;
            case 5:
                LED9 ^= 1; 
            break;
        }

    }
    return 0;
}

