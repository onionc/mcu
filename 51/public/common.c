#include "common.h"


#if hz==12
/**
 * 12.000MHz 下的延迟1毫秒函数
 */
void delay1ms(unsigned int nums)
{
    unsigned char i, j;
    while(nums--){
        i = 2;
        j = 239;
        do
        {
            while (--j);
        } while (--i);
    }
}
void delay10us() //@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 2;
	while (--i);
}


#elif hz==110592

void delay1ms(unsigned int nums)	//@11.0592MHz
{
	unsigned char i, j;
    while(nums--){
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j);
        } while (--i);
    }
}


void delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

#endif






/**
 * 12.000MHz 下的延迟40微秒函数
 */
void delay40us(){
    unsigned char i;
    _nop_();
    i = 17;
    while (--i);
}




