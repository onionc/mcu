#ifndef FREE_RTOS_H
#define FREE_RTOS_H

#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "list.h"


// ������ƿ�����
typedef struct tskTaskControlBlock{
    volatile StackType_t *pxTopOfStack; // ջ��
    ListItem_t xStateListItem;          // ����ڵ�
    StackType_t *pxStack;               // ����ջ��ʼ��ַ
    char pcTaskName[configMAX_TASK_NAME_LEN]; // ��������
    TickType_t xTicksToDelay; // ��ʱ
}TCB_t;



#endif
