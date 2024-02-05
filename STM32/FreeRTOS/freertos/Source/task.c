#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"
#include "port.h"

// ��������ջ
StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

// ������ƿ�
TCB_t Task1TCB;
TCB_t Task2TCB;
TCB_t IdleTaskTCB; // ����������ƿ�

// ָ��ǰ�����ָ��
TCB_t * pxCurrentTCB; 

// ��������б�
List_t pxReadyTasksLists[configMAX_PRIORITIES];

// ʱ��������
static volatile TickType_t xTickCount = 0;

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
    /************ ������������ start ************/
    TCB_t *pxIdleTaskTCBBUffer = NULL;
    StackType_t *pxIdleTaskStackBuffer = NULL;
    uint32_t ulIdleTaskStackSize;
    
    // ��ȡ����������ڴ�
    vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBUffer, &pxIdleTaskStackBuffer, &ulIdleTaskStackSize);
    
    // ������������
    TaskIdle_Handle = xTaskCreateStatic( 
        (TaskFunction_t) prvIdleTask,
        (char *) "IDLE",
        (uint32_t) ulIdleTaskStackSize,
        NULL,
        (StackType_t *) pxIdleTaskStackBuffer,
        (TCB_t *) pxIdleTaskTCBBUffer);
    // ������ӵ������б�
    vListInsertEnd( &(pxReadyTasksLists[0]), &(((TCB_t*)pxIdleTaskTCBBUffer)->xStateListItem));
                        
    
    /************ ������������ end   ************/
    
    // �ֶ�ָ����һ�����е�����
    pxCurrentTCB = &Task1TCB;
    
    // ����������
    if(xPortStartScheduler()!=pdFALSE){
    
    }
}

// �����л�
#if 0
void vTaskSwitchContext(void){
    if(pxCurrentTCB == &Task1TCB){
        pxCurrentTCB = &Task2TCB;
    }else{
        pxCurrentTCB = &Task1TCB;
    }
}
#else
void vTaskSwitchContext(void){
    // ���Ϊ�������񣬼������1������2����ʱʱ�䣬���û�е��������ִ�п�������
    if(pxCurrentTCB == &IdleTaskTCB){
        if(Task1TCB.xTicksToDelay == 0){
            pxCurrentTCB = &Task1TCB;
        }else if(Task2TCB.xTicksToDelay == 0){
            pxCurrentTCB = &Task2TCB;
        }else{
            return;
        }
    }else{
        // �����ǰ������1������2�������һ�����񣬺��߲�����ʱ�о��л����񡣷����жϵ�ǰ�����Ƿ������ʱ������Ǿ��л�������
        if(pxCurrentTCB == &Task1TCB){
            if(Task2TCB.xTicksToDelay == 0){
                pxCurrentTCB = &Task2TCB;
            }else if(pxCurrentTCB->xTicksToDelay != 0){
                pxCurrentTCB = &IdleTaskTCB;
            }else{
                return ;
            }
        }else if(pxCurrentTCB == &Task2TCB){
            if(Task1TCB.xTicksToDelay == 0){
                pxCurrentTCB = &Task1TCB;
            }else if(pxCurrentTCB->xTicksToDelay != 0){
                pxCurrentTCB = &IdleTaskTCB;
            }else{
                return ;
            }
        }
    }

}

#endif

// ��ȡ���������ڴ�(ջ������TCB��
void vApplicationGetIdleTaskMemory(TCB_t ** ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize){
    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

// ������ʱ����
void vTaskDelay(const TickType_t xTicksToDelay){
    TCB_t *pxTCB = NULL;
    pxTCB = pxCurrentTCB; // ��ȡ��ǰ����
    pxTCB->xTicksToDelay = xTicksToDelay; // ������ʱʱ��
    
    taskYIELD(); // �����л�
}

// ����ϵͳʱ��
void xTaskIncrementTick(void){
    TCB_t *pxTCB = NULL;
    BaseType_t i=0;
    
    // ����ϵͳʱ�������� xTickCount
    const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;
    
    // ɨ������б������������ ��ʱʱ�䣬�����Ϊ0����1
    for(i=0; i<configMAX_PRIORITIES; i++){
        pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY((&pxReadyTasksLists[i]));
        if(pxTCB->xTicksToDelay > 0){
            pxTCB->xTicksToDelay--;
        }
    }
    
    // �����л�
    portYIELD();
}

#endif
