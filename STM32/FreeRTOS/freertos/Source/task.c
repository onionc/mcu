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

// ȫ�������ʱ��
static volatile UBaseType_t uxCurrentNumberOfTasks = 0;

// ʱ��������
static volatile TickType_t xTickCount = 0;

// �����������ȼ�
static volatile UBaseType_t uxTopReadyPriority = taskIDLE_PRIORITY;

// ������ʱ�б�
static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2; // ����������ʱ�б�û����������ʱ�ֱ���һ���б�
static List_t * volatile pxDelayedTaskList; // ָ�� xTickCount û�����ʱʹ�õ��б�
static List_t * volatile pxOverflowDelayedTaskList; // ָ�� xTickCount ���ʱʹ�õ��б�

// ��һ���������ʱ��
static volatile TickType_t xNextTaskUnblockTime = portMAX_DELAY;

// �������
volatile BaseType_t xNumOfOverflows = 0;

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

// ��������ӵ������б�
static void prvAddNewTaskToReadyList(TCB_t *pxNewTCB){
    // �����ٽ��
    taskENTER_CRITICAL();
    
    {
        // ȫ�������ʱ���ۼ�
        uxCurrentNumberOfTasks++;
        
        // ���pxCurrentTCB Ϊ�գ�����ָ���´���������
        if(pxCurrentTCB == NULL){
            pxCurrentTCB = pxNewTCB;
            
            // ����ǵ�һ�δ�����������Ҫ��ʼ�������б�
            if(uxCurrentNumberOfTasks == 1){
                prvInitialiseTaskLists();
            }
        }else{
            // �����������ȼ��� pxCurrentTCB ָ��������ȼ������TCB
            if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority){
                pxCurrentTCB = pxNewTCB;
            }
        }
        
        // ��������ӵ������б�
        prvAddTaskToReadyList(pxNewTCB);
    }
    
    // �˳��ٽ��
    taskEXIT_CRITICAL();
}

/**
 * ���񴴽�
 * @param pxTaskCode ������ָ��
 * @param pcName ��������
 * @param ulStackDepth ����ջ��С
 * @param pvParameters �����β�
 * @param uxPriority �������ȼ�����ֵԽ�����ȼ�Խ��
 * @param puxStackBuffer ����ջ��ʼ��ַ
 * @param pxTaskBuffer ������ƿ�ָ��
 * @return 
 */
TaskHandle_t xTaskCreateStatic(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters, 
    UBaseType_t uxPriority,
    StackType_t * const puxStackBuffer,
    TCB_t * const pxTaskBuffer
){
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn;
    
    if( (pxTaskBuffer!=NULL) && (puxStackBuffer!=NULL) ){
        pxNewTCB = (TCB_t *) pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *) puxStackBuffer;
        
        // ����������
        prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, &xReturn, pxNewTCB);
        
        // ��������ӵ������б�
        prvAddNewTaskToReadyList(pxNewTCB);
        
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
 * @param uxPriority �������ȼ�����ֵԽ�����ȼ�Խ��
 * @param pxCreatedTask ������
 * @param pxNewTCB ������ƿ�ָ��
 */
static void prvInitialiseNewTask(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
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
    
    // ��ʼ�����ȼ�
    if(uxPriority >= configMAX_PRIORITIES){
        uxPriority = (UBaseType_t) configMAX_PRIORITIES - 1;
    }
    pxNewTCB->uxPriority = uxPriority;
    
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
    
    // ��ʼ�������б�
    for(uxPriority = 0; uxPriority<configMAX_PRIORITIES; uxPriority++){
        vListInitialise(&(pxReadyTasksLists[uxPriority]));
    }
    
    // ��ʼ��������ʱ�б�
    vListInitialise(&xDelayedTaskList1);
    vListInitialise(&xDelayedTaskList2);
    
    pxDelayedTaskList = &xDelayedTaskList1;
    pxOverflowDelayedTaskList = &xDelayedTaskList2;
}

// ��λ xNextTaskUnlockTime ��ֵ
static void prvResetNextTaskUnblockTime(void){
    TCB_t *pxTCB;
    if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE){
        // ��ǰ��ʱ�б�Ϊ�գ�������һ������ʱ��xNextTaskUnblockTimeΪ���ֵ
        xNextTaskUnblockTime = portMAX_DELAY;
    }else{
        // ��ǰ�б�Ϊ�գ���ȡ��ǰ�б���һ�ڵ������ֵ�������µ� �´ν���ʱ�� xNextTaskUnblockTime ��
        (pxTCB) = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY(pxDelayedTaskList);
        xNextTaskUnblockTime = listGET_LIST_ITEM_VALUE(&(pxTCB->xStateListItem));
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
        (UBaseType_t) taskIDLE_PRIORITY,
        (StackType_t *) pxIdleTaskStackBuffer,
        (TCB_t *) pxIdleTaskTCBBUffer);
    
    /************ ������������ end   ************/
    xNextTaskUnblockTime = portMAX_DELAY;
    xTickCount = 0;
    
    // ����������
    if(xPortStartScheduler()!=pdFALSE){
    
    }
}

// �����л�
/*
void vTaskSwitchContext(void){
    if(pxCurrentTCB == &Task1TCB){
        pxCurrentTCB = &Task2TCB;
    }else{
        pxCurrentTCB = &Task1TCB;
    }
}*/
#if 1
void vTaskSwitchContext(void){
    // ��ȡ���ȼ���ߵľ��������TCB�����µ�pxCurrentTCB, ʵ��������л�
    taskSELECT_HIGHEST_PRIORITY_TASK();
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


// ��������뵽��ʱ�б�
static void prvAddCurrentTaskToDelayedList(TickType_t xTicksToWait){
    TickType_t xTimeToWake;
    
    // ��ȡϵͳʱ��������
    const TickType_t xConstTickCount = xTickCount;
    
    // ������Ӿ����б����Ƴ�
    if(uxListRemove(&(pxCurrentTCB->xStateListItem)) == 0){
        // ���������ȼ�λͼ�Ķ�Ӧλ���
        portRESET_READY_PRIORITY(pxCurrentTCB->uxPriority, uxTopReadyPriority);
    }
    
    // ����������ʱ����ʱ��ϵͳʱ����������ֵ
    xTimeToWake = xConstTickCount + xTicksToWait;
    
    // ����ʱ���ڵ�ֵ����Ϊ����ֵ
    listSET_LIST_ITEM_VALUE( &(pxCurrentTCB->xStateListItem), xTimeToWake );
    
    // ���
    if(xTimeToWake < xConstTickCount){
        vListInsert(pxOverflowDelayedTaskList, &(pxCurrentTCB->xStateListItem));
    }else{
        vListInsert(pxDelayedTaskList, &(pxCurrentTCB->xStateListItem));
        // ������һ���������ʱ�̱��� xNextTaskUnblockTime ֵ
        if(xTimeToWake < xNextTaskUnblockTime){
            xNextTaskUnblockTime = xTimeToWake;
        }
    }
}

// ������ʱ����
void vTaskDelay(const TickType_t xTicksToDelay){
    // ��������뵽��ʱ�б�
    prvAddCurrentTaskToDelayedList(xTicksToDelay);

    
    taskYIELD(); // �����л�
}

// ����ϵͳʱ��
void xTaskIncrementTick(void){
    TCB_t *pxTCB = NULL;
    TickType_t xItemValue;
    
    // ����ϵͳʱ�������� xTickCount
    const TickType_t xConstTickCount = ++xTickCount;
    // xTickCount = xConstTickCount;
    
    // ���������л�����ʱ�б���Ϊ��������񶼵���һ�ֵ��������ˣ��л���ʱ����б����ʱ�б�
    if(xConstTickCount == 0){
        taskSWITCH_DELAYED_LISTS();
    }
    
    // ������ʱ����,����ʱ���ڵ�����һһ����ʱ�б������
    if(xConstTickCount >= xNextTaskUnblockTime){
        for(;;){
            if(listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE){
                // ��ʱ�б�Ϊ�գ�����Ϊ���ֵ
                xNextTaskUnblockTime = portMAX_DELAY;
                break;
            }else{
                // ��ʱ�б�Ϊ��
                
                // ȡ����ʱ�б��һ���ڵ������ֵ
                pxTCB = (TCB_t *) listGET_OWNER_OF_HEAD_ENTRY(pxDelayedTaskList);
                xItemValue = listGET_LIST_ITEM_VALUE(&(pxTCB->xStateListItem));
                
                // ����ʱ�б���������ʱ���ڵ������Ƴ�������
                if(xConstTickCount < xItemValue){
                    xNextTaskUnblockTime = xItemValue;
                    break;
                }
                // ���������ʱ�б��Ƴ��������ȴ�״̬
                (void) uxListRemove(&(pxTCB->xStateListItem));
                
                // ������ȴ���������ӵ������б�
                prvAddTaskToReadyList(pxTCB);
            }
        }
    }
    
    // �����л�
    portYIELD();
}


#endif
