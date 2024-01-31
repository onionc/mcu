#include "list.h"

// 链表节点初始化
void vListInitialseItem(ListItem_t * const pxItem){
    pxItem->pvContainer = NULL; // 该节点所在链表为空
}

// 链表根节点初始化
void vListInitialise(List_t * const pxList){
    pxList->pxIndex = (ListItem_t *) &(pxList->xListEnd);
    
    pxList->xListEnd.xItemValue = portMAX_DELAY;
    
    pxList->xListEnd.pxNext = (ListItem_t *) &(pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *) &(pxList->xListEnd);
    
    pxList->uxNumberOfItems = 0;
}

// 节点插入到链表尾部
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem){
    ListItem_t * const pxIndex = pxList->pxIndex;
    
    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;
    
    pxNewListItem->pvContainer = pxList;
    
    (pxList->uxNumberOfItems)++;

}

// 节点按升序排列插入到链表
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem){
    ListItem_t * pxIterator;
    
    // 获取数值（排序用）
    const TickType_t xValueOfInseration = pxNewListItem->xItemValue;
    
    // 寻找节点插入位置
    if(xValueOfInseration == portMAX_DELAY){
        // 末尾
        pxIterator = pxList->xListEnd.pxPrevious;
    }else{
        // 找到插入的位置
        for(pxIterator = (ListItem_t *) &(pxList->xListEnd); pxIterator->pxNext->xItemValue <= xValueOfInseration; pxIterator = pxIterator->pxNext);
    }
    
    // 插入节点
    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;
    
    // 记住该节点所在的链表
    pxNewListItem->pvContainer = pxList;
    
    // 节点计数器
    (pxList->uxNumberOfItems)++;
}

// 从链表删除节点
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove){
    // 获取链表
    List_t * const pxList = (List_t *) pxItemToRemove->pvContainer;
    
    // 将节点删除
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
    
    // 调整链表的节点索引指针
    if(pxList->pxIndex == pxItemToRemove){
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }
    
    // 设置该节点所在链表为空
    pxItemToRemove->pvContainer = NULL;
    
    // 链表节点计数器-1
    (pxList->uxNumberOfItems)--;
    
    // 返回链表节点个数
    return pxList->uxNumberOfItems;
}
