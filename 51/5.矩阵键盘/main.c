#include "../public/keys/matrix.h"
#include "../public/lcd1602/lcd1602.h"
#include "../public/utils/string.h"

int main(){
    int k = 0;
    char s[16]="";
    init();
    show(0,2, "key: ");

    while(1){
        k=key_matrix_flip_scan();
        if(k){
            P2 = k;
            int2string(k, s);
            show(0, 7, "   ");
            show(0, 7, s);
        }
    
    }
    
    return 0;
}