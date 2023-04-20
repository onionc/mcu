#include "string.h"

/**
 * ����ת�ַ���
 */
void int2string(int num, char *s){
    
    char i=0;
    char j=0;
    char t=0;
    
    while(num%10 || num/10){
        s[i++] = '0' + num%10;
        num/=10;
    }
    
    s[i] = '\0';

    // ��ת����
    while(i>j){
        --i;
        t = s[i];
        s[i] = s[j];
        s[j] = t;
        j++;
    }
}