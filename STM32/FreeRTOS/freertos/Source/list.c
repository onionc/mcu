#include "list.h"

// ����ڵ��ʼ��
void vListInitialseItem(ListItem_t * const pxItem){
    pxItem->pvContainer = NULL; // �ýڵ���������Ϊ��
}

// ������ڵ��ʼ��
void vListInitialise(List_t * const pxList){
    pxList->pxIndex = (ListItem_t *) &(pxList->xListEnd);
    
    pxList->xListEnd.xItemValue = portMAX_DELAY;
    
    pxList->xListEnd.pxNext = (ListItem_t *) &(pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *) &(pxList->xListEnd);
    
    pxList->uxNumberOfItems = 0;
}

// �ڵ���뵽����β��
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem){
    ListItem_t * const pxIndex = pxList->pxIndex;
    
    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;
    
    pxNewListItem->pvContainer = pxList;
    
    (pxList->uxNumberOfItems)++;

}

// �ڵ㰴�������в��뵽����
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem){
    ListItem_t * pxIterator;
    
    // ��ȡ��ֵ�������ã�
    const TickType_t xValueOfInseration = pxNewListItem->xItemValue;
    
    // Ѱ�ҽڵ����λ��
    if(xValueOfInseration == portMAX_DELAY){
        // ĩβ
        pxIterator = pxList->xListEnd.pxPrevious;
    }else{
        // �ҵ������λ��
        for(pxIterator = (ListItem_t *) &(pxList->xListEnd); pxIterator->pxNext->xItemValue <= xValueOfInseration; pxIterator = pxIterator->pxNext);
    }
    
    // ����ڵ�
    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;
    
    // ��ס�ýڵ����ڵ�����
    pxNewListItem->pvContainer = pxList;
    
    // �ڵ������
    (pxList->uxNumberOfItems)++;
}

// ������ɾ���ڵ�
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove){
    // ��ȡ����
    List_t * const pxList = (List_t *) pxItemToRemove->pvContainer;
    
    // ���ڵ�ɾ��
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;
    
    // ��������Ľڵ�����ָ��
    if(pxList->pxIndex == pxItemToRemove){
        pxList->pxIndex = pxItemToRemove->pxPrevious;
    }
    
    // ���øýڵ���������Ϊ��
    pxItemToRemove->pvContainer = NULL;
    
    // ����ڵ������-1
    (pxList->uxNumberOfItems)--;
    
    // ��������ڵ����
    return pxList->uxNumberOfItems;
}
