#ifndef LIST_H
#define LIST_H

/*
双向链表
*/

#include "portmacro.h"


/************* 结构体定义 *************/
/* 链表节点数据结构 */
struct xLIST_ITEM
{
    TickType_t          xItemValue;     /* 帮助节点排列顺序 */
    struct xLIST_ITEM   *pxNext;        /* 指向后一个节点 */
    struct xLIST_ITEM   *pxPrevious;    /* 指向前一个节点 */
    void                *pvOwner;       /* 拥有该节点的内核对象 */
    void                *pvContainer;   /* 指向该节点所在的链表 */
};
typedef struct xLIST_ITEM ListItem_t;   /* 节点数据类型重定义 */


/* 链表精简节点 */
struct xMINI_LIST_ITEM{
    TickType_t          xItemValue;
    struct xLIST_ITEM   *pxNext;
    struct xLIST_ITEM   *pxPrevious;
};
typedef struct xMINI_LIST_ITEM  MiniListItem_t;


/* 链表根节点 */
typedef struct xLIST{
    UBaseType_t     uxNumberOfItems;    /* 链表节点计数器 */
    ListItem_t      *pxIndex;           /* 链表节点索引指针 */
    MiniListItem_t  xListEnd;           /* 最后一个节点 */
} List_t;


/************* 宏函数 *************/
// 初始化节点拥有者
#define listSET_LIST_ITEM_OWNDER( pxListItem, pxOwner)\
    ( (pxListItem)->pvOwner = (void *) (pxOwner) )

// 获取节点拥有者
#define listGet_LIST_ITEM_OWNER( pxListItem )\
    ( (pxListItem)->pvOwner )

// 初始化节点排序值
#define listSET_LIST_ITEM_VALUE( pxListItem, xValue )\
    ( (pxListItem)->xItemValue = (xValue) )

// 获取节点排序值
#define listGET_LIST_ITEM_VALUE( pxListItem )\
    ( (pxListItem)->xItemValue )

// 获取链表根节点的节点计数器的值
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY( pxList )\
    ( ((pxList)->xListEnd).pxNext->xItemValue )
    
// 获取链表的入口节点
#define listGET_HEAD_ENTRY( pxList )\
    ( ((pxList)->xListEnd).pxNext )

// 获取节点的下一个节点
#define listGET_NEXT( pxListItem )\
    ( (pxListItem)->pxNext )

// 获取链表的最后一个节点
#define listGET_END_MARKER( pxList )\
    ( (ListItem_t const *) ( &((pxList)->xListEnd)))

// 判断链表是否为空
#define listLIST_IS_EMPTY( pxList )\
    ( (BaseType_t)((pxList)->uxNumberOfItems == 0) )

// 获取链表节点数
#define listCURRENT_LIST_LENGTH( pxList )\
    ( (pxList)->uxNumberOfItems )

// 获取链表下一个节点的拥有者，即TCB
#define listGET_OWNER_OF_NEXT_ENTRY( pxTCB, pxList )\
{\
    List_t * const pxConstList = (pxList);          \
    /*节点索引指向链表第一个节点*/                   \
    (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;    \
    if((void*)(pxConstList)->pxIndex == (void *)&((pxConstList)->xListEnd)){   \
        (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;                \
    }\
    (pxTCB) = (pxConstList)->pxIndex->pvOwner; \
}
// 获取链表头节点的拥有者
#define listGET_OWNER_OF_HEAD_ENTRY(pxList) ( (&((pxList)->xListEnd))->pxNext->pvOwner )

    
    
/************* 方法 *************/
// 链表节点初始化
void vListInitialseItem(ListItem_t * const pxItem);
// 链表根节点初始化
void vListInitialise(List_t * const pxList);
// 节点插入到链表尾部
void vListInsertEnd(List_t * const pxList, ListItem_t * const pxNewListItem);
// 节点按升序排列插入到链表
void vListInsert(List_t * const pxList, ListItem_t * const pxNewListItem);
// 从链表删除节点
UBaseType_t uxListRemove(ListItem_t * const pxItemToRemove);
#endif
