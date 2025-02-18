
#include <AI8051U.H>
#include "i2c.h"
#include "oled.h"
int main(){

    iic_init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"1234", 12);
    
    while(1){
    
    }
    

}