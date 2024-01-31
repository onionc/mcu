#include "list.h"


// 链表根节点
List_t List_Test;

// 节点
ListItem_t List_Item1;
ListItem_t List_Item2;
ListItem_t List_Item3;



int main(void){
    // 链表根节点初始化
    vListInitialise(&List_Test);
    
    // 节点1初始化
    vListInitialseItem(&List_Item1);
    List_Item1.xItemValue = 1;
    
    vListInitialseItem(&List_Item2);
    List_Item2.xItemValue = 2;
    
    vListInitialseItem(&List_Item3);
    List_Item3.xItemValue = 3;
    
    // 将节点插入链表，升序排序
    vListInsert(&List_Test, &List_Item2);
    vListInsert(&List_Test, &List_Item1);
    vListInsert(&List_Test, &List_Item3);
    
    for(;;){
    
    }
}
