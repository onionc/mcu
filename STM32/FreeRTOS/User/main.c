#include "list.h"


// ������ڵ�
List_t List_Test;

// �ڵ�
ListItem_t List_Item1;
ListItem_t List_Item2;
ListItem_t List_Item3;



int main(void){
    // ������ڵ��ʼ��
    vListInitialise(&List_Test);
    
    // �ڵ�1��ʼ��
    vListInitialseItem(&List_Item1);
    List_Item1.xItemValue = 1;
    
    vListInitialseItem(&List_Item2);
    List_Item2.xItemValue = 2;
    
    vListInitialseItem(&List_Item3);
    List_Item3.xItemValue = 3;
    
    // ���ڵ����������������
    vListInsert(&List_Test, &List_Item2);
    vListInsert(&List_Test, &List_Item1);
    vListInsert(&List_Test, &List_Item3);
    
    for(;;){
    
    }
}
