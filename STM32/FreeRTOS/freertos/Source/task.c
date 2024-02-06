#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"
#include "port.h"

// 定义任务栈
StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

// 任务控制块
TCB_t Task1TCB;
TCB_t Task2TCB;
TCB_t IdleTaskTCB; // 空闲任务控制块

// 指向当前任务的指针
TCB_t * pxCurrentTCB; 

// 任务就绪列表
List_t pxReadyTasksLists[configMAX_PRIORITIES];

// 全局任务计时器
static volatile UBaseType_t uxCurrentNumberOfTasks = 0;

// 时基计数器
static volatile TickType_t xTickCount = 0;

// 任务的最高优先级
static volatile UBaseType_t uxTopReadyPriority = taskIDLE_PRIORITY;

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

// 将任务添加到就绪列表
static void prvAddNewTaskToReadyList(TCB_t *pxNewTCB){
    // 进入临界段
    taskENTER_CRITICAL();
    
    {
        // 全局任务计时器累加
        uxCurrentNumberOfTasks++;
        
        // 如果pxCurrentTCB 为空，则将它指向新创建的任务
        if(pxCurrentTCB == NULL){
            pxCurrentTCB = pxNewTCB;
            
            // 如果是第一次创建任务，则需要初始化就绪列表
            if(uxCurrentNumberOfTasks == 1){
                prvInitialiseTaskLists();
            }
        }else{
            // 根据任务优先级将 pxCurrentTCB 指向最高优先级任务的TCB
            if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority){
                pxCurrentTCB = pxNewTCB;
            }
        }
        
        // 将任务添加到就绪列表
        prvAddTaskToReadyList(pxNewTCB);
    }
    
    // 退出临界段
    taskEXIT_CRITICAL();
}

/**
 * 任务创建
 * @param pxTaskCode 任务函数指针
 * @param pcName 任务名称
 * @param ulStackDepth 任务栈大小
 * @param pvParameters 任务形参
 * @param uxPriority 任务优先级，数值越大，优先级越高
 * @param puxStackBuffer 任务栈起始地址
 * @param pxTaskBuffer 任务控制块指针
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
        
        // 创建新任务
        prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, &xReturn, pxNewTCB);
        
        // 将任务添加到就绪列表
        prvAddNewTaskToReadyList(pxNewTCB);
        
    }else{
        xReturn = NULL;
    }
    
    return xReturn;

}


/**
 * 创建新任务
 * @param pxTaskCode 任务函数
 * @param pcName 任务名称
 * @param ulStackDepth 任务栈大小
 * @parma pvParameters 任务形参
 * @param uxPriority 任务优先级，数值越大，优先级越高
 * @param pxCreatedTask 任务句柄
 * @param pxNewTCB 任务控制块指针
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
    
    // 获取栈顶地址
    pxTopStack = pxNewTCB->pxStack + (ulStackDepth -1);
    // 向下8字节对齐
    pxTopStack = (StackType_t*) ( (uint32_t)pxTopStack & (~(uint32_t)0x7) );
    // 将任务名存储到TCB中
    for(x = 0; x < configMAX_TASK_NAME_LEN; x++){
        pxNewTCB->pcTaskName[x] = pcName[x];
        if(pcName[x]==0) break;
    }
    
    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN-1] = 0;
    
    // 初始化TCB中的节点
    vListInitialseItem(&(pxNewTCB->xStateListItem));
    
    // 设置节点的拥有者
    listSET_LIST_ITEM_OWNDER(&(pxNewTCB->xStateListItem), pxNewTCB);
    
    // 初始化优先级
    if(uxPriority >= configMAX_PRIORITIES){
        uxPriority = (UBaseType_t) configMAX_PRIORITIES - 1;
    }
    pxNewTCB->uxPriority = uxPriority;
    
    // 初始化任务栈
    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopStack, pxTaskCode, pvParameters);
    
    // 让任务句柄指向任务控制块
    if((void *)pxCreatedTask != NULL){
        *pxCreatedTask = (TaskHandle_t) pxNewTCB;
    }
}

// 就绪列表初始化
void prvInitialiseTaskLists(void){
    UBaseType_t uxPriority;
    
    for(uxPriority = 0; uxPriority<configMAX_PRIORITIES; uxPriority++){
        vListInitialise(&(pxReadyTasksLists[uxPriority]));
    }
}

// 任务开启

void vTaskStartScheduler(void){
    /************ 创建空闲任务 start ************/
    TCB_t *pxIdleTaskTCBBUffer = NULL;
    StackType_t *pxIdleTaskStackBuffer = NULL;
    uint32_t ulIdleTaskStackSize;
    
    // 获取空闲任务的内存
    vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBUffer, &pxIdleTaskStackBuffer, &ulIdleTaskStackSize);
    
    // 创建空闲任务
    TaskIdle_Handle = xTaskCreateStatic( 
        (TaskFunction_t) prvIdleTask,
        (char *) "IDLE",
        (uint32_t) ulIdleTaskStackSize,
        NULL,
        (UBaseType_t) taskIDLE_PRIORITY,
        (StackType_t *) pxIdleTaskStackBuffer,
        (TCB_t *) pxIdleTaskTCBBUffer);
    
    /************ 创建空闲任务 end   ************/
    
    // 启动调度器
    if(xPortStartScheduler()!=pdFALSE){
    
    }
}

// 任务切换
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
    // 获取优先级最高的就绪任务的TCB，更新到pxCurrentTCB, 实现任务的切换
    taskSELECT_HIGHEST_PRIORITY_TASK();
}
#else
void vTaskSwitchContext(void){
    // 如果为空闲任务，检查任务1或任务2的延时时间，如果没有到期则继续执行空闲任务
    if(pxCurrentTCB == &IdleTaskTCB){
        if(Task1TCB.xTicksToDelay == 0){
            pxCurrentTCB = &Task1TCB;
        }else if(Task2TCB.xTicksToDelay == 0){
            pxCurrentTCB = &Task2TCB;
        }else{
            return;
        }
    }else{
        // 如果当前是任务1或任务2，检查另一个任务，后者不在延时中就切换任务。否则，判断当前任务是否进入延时，如果是就切换到空闲
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

// 获取空闲任务内存(栈和任务TCB）
void vApplicationGetIdleTaskMemory(TCB_t ** ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize){
    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = IdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

// 阻塞延时函数
void vTaskDelay(const TickType_t xTicksToDelay){
    TCB_t *pxTCB = NULL;
    pxTCB = pxCurrentTCB; // 获取当前任务
    pxTCB->xTicksToDelay = xTicksToDelay; // 设置延时时间
    
    // 将任务从就绪列表移除
    // todo: uxListRemove(&(pxTCB->xStateListItem));
    taskRESET_READY_PRIORITY(pxTCB->uxPriority);
    
    taskYIELD(); // 任务切换
}

// 更新系统时基
void xTaskIncrementTick(void){
    TCB_t *pxTCB = NULL;
    BaseType_t i=0;
    
    // 更新系统时基计数器 xTickCount
    const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;
    
    // 扫描就绪列表中所有任务的 延时时间，如果不为0，减1
    for(i=0; i<configMAX_PRIORITIES; i++){
        pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY((&pxReadyTasksLists[i]));
        if(pxTCB->xTicksToDelay > 0){
            pxTCB->xTicksToDelay--;
            
            // 延时时间到，将任务就绪
            if(pxTCB->xTicksToDelay == 0){
                taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
            }
        }
    }
    
    // 任务切换
    portYIELD();
}

#endif
