#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"
#include "port.h"

// ��������ջ
StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];

// ������ƿ�
TCB_t Task1TCB;
TCB_t Task2TCB;

// ָ��ǰ�����ָ��
TCB_t * pxCurrentTCB; 

// ��������б�
List_t pxReadyTasksLists[configMAX_PRIORITIES];


#if( configSUPPORT_STATIC_ALLOCATION == 1 )
/**
 * ���񴴽�
 * @param pxTaskCode ������ָ��
 * @param pcName ��������
 * @param ulStackDepth ����ջ��С
 * @parma pvParameters �����β�
 * @param puxStackBuffer ����ջ��ʼ��ַ
 * @param pxTaskBuffer ������ƿ�ָ��
 * @return 
 */
TaskHandle_t xTaskCreateStatic(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters, 
    StackType_t * const puxStackBuffer,
    TCB_t * const pxTaskBuffer
){
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn;
    
    if( (pxTaskBuffer!=NULL) && (puxStackBuffer!=NULL) ){
        pxNewTCB = (TCB_t *) pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *) puxStackBuffer;
        
        // ����������
        prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, &xReturn, pxNewTCB);
        
        
    }else{
        xReturn = NULL;
    }
    
    return xReturn;

}

/**
 * ����������
 * @param pxTaskCode ������
 * @param pcName ��������
 * @param ulStackDepth ����ջ��С
 * @parma pvParameters �����β�
 * @param pxCreatedTask ������
 * @param pxNewTCB ������ƿ�ָ��
 */
static void prvInitialiseNewTask(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask,
    TCB_t * pxNewTCB
){
    StackType_t * pxTopStack;
    UBaseType_t x;
    
    // ��ȡջ����ַ
    pxTopStack = pxNewTCB->pxStack + (ulStackDepth -1);
    // ����8�ֽڶ���
    pxTopStack = (StackType_t*) ( (uint32_t)pxTopStack & (~(uint32_t)0x7) );
    // ���������洢��TCB��
    for(x = 0; x < configMAX_TASK_NAME_LEN; x++){
        pxNewTCB->pcTaskName[x] = pcName[x];
        if(pcName[x]==0) break;
    }
    
    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1] = 0;
    
    // ��ʼ��TCB�еĽڵ�
    vListInitialseItem(&(pxNewTCB->xStateListItem));
    
    // ���ýڵ��ӵ����
    listSET_LIST_ITEM_OWNDER(&(pxNewTCB->xStateListItem), pxNewTCB);
    
    // ��ʼ������ջ
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopStack, pxTaskCode, pvParameters);
    
    // ��������ָ��������ƿ�
    if((void *)pxCreatedTask != NULL){
        *pxCreatedTask = (TaskHandle_t) pxNewTCB;
    }
}


// �����б��ʼ��
void prvInitialiseTaskLists(void){
    UBaseType_t uxPriority;
    
    for(uxPriority = 0; uxPriority<configMAX_PRIORITIES; uxPriority++){
        vListInitialise(&(pxReadyTasksLists[uxPriority]));
    }
}

// ������
void vTaskStartScheduler(void){
    // �ֶ�ָ����һ�����е�����
    pxCurrentTCB = &Task1TCB;
    
    // ����������
    if(xPortStartScheduler()!=pdFALSE){
    
    }
}

// �����л�
void vTaskSwitchContext(void){
    if(pxCurrentTCB == &Task1TCB){
        pxCurrentTCB = &Task2TCB;
    }else{
        pxCurrentTCB = &Task1TCB;
    }
}

#endif
