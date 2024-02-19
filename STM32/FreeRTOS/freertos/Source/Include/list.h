#ifndef LIST_H
#define LIST_H

/*
˫������
*/

#include "portmacro.h"


/************* �ṹ�嶨�� *************/
/* ����ڵ����ݽṹ */
struct xLIST_ITEM
{
    TickType_t          xItemValue;     /* �����ڵ�����˳�� */
    struct xLIST_ITEM   *pxNext;        /* ָ���һ���ڵ� */
    struct xLIST_ITEM   *pxPrevious;    /* ָ��ǰһ���ڵ� */
    void                *pvOwner;       /* ӵ�иýڵ���ں˶��� */
    void                *pvContainer;   /* ָ��ýڵ����ڵ����� */
};
typedef struct xLIST_ITEM ListItem_t;   /* �ڵ����������ض��� */


/* ������ڵ� */
struct xMINI_LIST_ITEM{
    TickType_t          xItemValue;
    struct xLIST_ITEM   *pxNext;
    struct xLIST_ITEM   *pxPrevious;
};
typedef struct xMINI_LIST_ITEM  MiniListItem_t;


/* ������ڵ� */
typedef struct xLIST{
    UBaseType_t     uxNumberOfItems;    /* ����ڵ������ */
    ListItem_t      *pxIndex;           /* ����ڵ�����ָ�� */
    MiniListItem_t  xListEnd;           /* ���һ���ڵ� */
} List_t;


/************* �꺯�� *************/
// ��ʼ���ڵ�ӵ����
#define listSET_LIST_ITEM_OWNDER( pxListItem, pxOwner)\
    ( (pxListItem)->pvOwner = (void *) (pxOwner) )

// ��ȡ�ڵ�ӵ����
#define listGet_LIST_ITEM_OWNER( pxListItem )\
    ( (pxListItem)->pvOwner )

// ��ʼ���ڵ�����ֵ
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )\
    ( (pxListItem)->xItemValue = (xValue) )

// ��ȡ�ڵ�����ֵ
#define listGET_LIST_ITEM_VALUE( pxListItem )\
    ( (pxListItem)->xItemValue )

// ��ȡ������ڵ�Ľڵ��������ֵ
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )\
    ( ((pxList)->xListEnd).pxNext->xItemValue )
    
// ��ȡ�������ڽڵ�
#define listGET_HEAD_ENTRY( pxList )\
    ( ((pxList)->xListEnd).pxNext )

// ��ȡ�ڵ����һ���ڵ�
#define listGET_NEXT( pxListItem )\
    ( (pxListItem)->pxNext )

// ��ȡ��������һ���ڵ�
#define listGET_END_MARKER( pxList )\
    ( (ListItem_t const *) ( &((pxList)->xListEnd)))

// �ж������Ƿ�Ϊ��
#define listLIST_IS_EMPTY( pxList )\
    ( (BaseType_t)((pxList)->uxNumberOfItems == 0) )

// ��ȡ����ڵ���
#define listCURRENT_LIST_LENGTH( pxList )\
    ( (pxList)->uxNumberOfItems )

// ��ȡ������һ���ڵ��ӵ���ߣ���TCB
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )\
{\
    List_t * const pxConstList = (pxList);          \
    /*�ڵ�����ָ�������һ���ڵ�*/                   \
    (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;    \
    if((void*)(pxConstList)->pxIndex == (void *)&((pxConstList)->xListEnd)){   \
        (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;                \
    }\
    (pxTCB) = (pxConstList)->pxIndex->pvOwner; \
}
// ��ȡ����ͷ�ڵ��ӵ����
#define listGET_OWNER_OF_HEAD_ENTRY(pxList) ( (&((pxList)->xListEnd))->pxNext->pvOwner )

    
    
/************* ���� *************/
// ����ڵ��ʼ��
void vListInitialseItem(ListItem_t * const pxItem);
// ������ڵ��ʼ��
void vListInitialise(List_t * const pxList);
// �ڵ���뵽����β��
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem);
// �ڵ㰴�������в��뵽����
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
// ������ɾ���ڵ�
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove);
#endif
