#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"
#include "port.h"

// 定义任务栈
StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];

// 任务控制块
TCB_t Task1TCB;
TCB_t Task2TCB;

// 指向当前任务的指针
TCB_t * pxCurrentTCB; 

// 任务就绪列表
List_t pxReadyTasksLists[configMAX_PRIORITIES];


#if( configSUPPORT_STATIC_ALLOCATION == 1 )
/**
 * 任务创建
 * @param pxTaskCode 任务函数指针
 * @param pcName 任务名称
 * @param ulStackDepth 任务栈大小
 * @parma pvParameters 任务形参
 * @param puxStackBuffer 任务栈起始地址
 * @param pxTaskBuffer 任务控制块指针
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
        
        // 创建新任务
        prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, &xReturn, pxNewTCB);
        
        
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
 * @param pxCreatedTask 任务句柄
 * @param pxNewTCB 任务控制块指针
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
    // 手动指定第一个运行的任务
    pxCurrentTCB = &Task1TCB;
    
    // 启动调度器
    if(xPortStartScheduler()!=pdFALSE){
    
    }
}

// 任务切换
void vTaskSwitchContext(void){
    if(pxCurrentTCB == &Task1TCB){
        pxCurrentTCB = &Task2TCB;
    }else{
        pxCurrentTCB = &Task1TCB;
    }
}

#endif
