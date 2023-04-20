#include <REGX52.H>

#include "../public/common.h"
#include "../public/lcd1602/lcd1602.h"


int main(){
    init();
    show(0,0, "1234567890ABCDEFGH");
    delay1ms(2000); 
   
    show(0,3, "---111");
    delay1ms(2000);
    clear();
    show(1,3, "HELLO");              

    while(1){
        
        

    };
    return 0;
}