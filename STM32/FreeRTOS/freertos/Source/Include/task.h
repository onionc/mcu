#ifndef TASK_H
#define TASK_H


#include "portmacro.h"
#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"

typedef void * TaskHandle_t;

// 定义任务栈
#define TASK1_STACK_SIZE    128
extern StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE    128
extern StackType_t Task2Stack[TASK2_STACK_SIZE];

// 空闲任务栈
extern StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

// 任务控制块
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB; // 空闲任务控制块

extern TaskHandle_t TaskIdle_Handle;

// 指向当前任务的指针
extern TCB_t * pxCurrentTCB; 

// 溢出计数
extern volatile BaseType_t xNumOfOverflows;

// 任务切换方法
#define taskYIELD()     portYIELD()

/**
 * 任务创建
 * @param pxTaskCode 任务函数指针
 * @param pcName 任务名称
 * @param ulStackDepth 任务栈大小
 * @parma pvParameters 任务形参
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
);
// 创建新任务
static void prvInitialiseNewTask(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t * const pxCreatedTask,
    TCB_t * pxNewTCB
);


    
// 任务就绪列表
extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

// 就绪列表初始化
void prvInitialiseTaskLists(void);
    
// 任务开启
void vTaskStartScheduler(void);

// 任务切换
void vTaskSwitchContext(void);
    

/************* 临界段相关定义 start *************/
    
// 进入临界段。不带中断保护版本，不能嵌套
#define taskENTER_CRITICAL()    portENTER_CRITICAL()  
// 进入临界段。带中断保护版本，可以嵌套
#define taskENTER_CRITICAL_FROM_ISR()   portSET_INTERRUPT_MASK_FROM_ISR()
// 退出临界段。不带中断保护版本，不能嵌套
#define taskEXIT_CRITICAL()     portEXIT_CRITICAL()
// 退出临界段。带中断保护版本，可以嵌套
#define taskEXIT_CRITICAL_FROM_ISR(x)   portCLEAR_INTERRUPT_MASK_FROM_ISR(x)

/************* 临界段相关定义 end   *************/

// 获取空闲任务内存(栈和任务TCB）
void vApplicationGetIdleTaskMemory(TCB_t ** ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

// 阻塞延时函数
void vTaskDelay(const TickType_t xTicksToDelay);

// 时基计数器
static volatile TickType_t xTickCount;

// 更新系统时基
void xTaskIncrementTick(void);

// 空闲任务
void prvIdleTask(void);
// 空闲任务优先级
#define taskIDLE_PRIORITY (0)

/************* 多优先级相关定义 start *************/

// 查找最高优先级的就绪任务（通用方法）
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )

    // 获取并设置最高优先级，uxTopReadyPriority 存放的是最高优先级
    #define taskRECORD_READY_PRIORITY(uxPriority)       \
    {                                                   \
        if( (uxPriority) > uxTopReadyPriority )         \
        {                                               \
            uxTopReadyPriority = (uxPriority);          \
        }                                               \
    }
    
    // 选择最高优先级任务
    #define taskSELECT_HIGHEST_PRIORITY_TASK()          \
    {                                                   \
        UBaseType_t uxTopPriority = uxTopReadyPriority; \
        /* 寻找就绪列表中最高优先级的队列 */              \
        while(listLIST_IS_EMPTY(&(pxReadyTasksLists[uxTopPriority])))   \
        {                                               \
            --uxTopPriority;                            \
        }                                               \
        /* 获取优先级最高任务的TCB，更新到 pxCurrentTCB */         \
        listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists[uxTopPriority]));  \
        uxTopReadyPriority = uxTopPriority;             \
    }
    
    // 重置最高优先级，属于选择优化的宏定义，这里为空
    #define taskRESET_READY_PRIORITY(uxPriority) 

// 查找最高优先级的就绪任务（根据处理器架构优化后的方法）
#else
    // 设置最高优先级
    #define taskRECORD_READY_PRIORITY(uxPriority)       \
        portRECORD_READY_PRIORITY(uxPriority, uxTopReadyPriority)
    
    // 选择最高优先级任务
    #define taskSELECT_HIGHEST_PRIORITY_TASK()          \
    {                                                   \
        UBaseType_t uxTopPriority;                      \
        /* 寻找最高优先级 */                              \
        portGET_HIGHEST_PRIORITY(uxTopPriority, uxTopReadyPriority);    \
        /* 获取优先级最高任务的TCB，更新到 pxCurrentTCB */ \
        listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists[uxTopPriority])); \
    }
    
    // 重置最高优先级
    #if 1
        #define taskRESET_READY_PRIORITY(uxPriority)        \
        {   \
            if(listCURRENT_LIST_LENGTH(&(pxReadyTasksLists[(uxPriority)])) == 0){   \
                portRESET_READY_PRIORITY((uxPriority), uxTopReadyPriority);         \
            }                                                                       \
        }
    #else
        #define taskRESET_READY_PRIORITY(uxPriority)   \
        {                                              \
            portRESET_READY_PRIORITY((uxPriority), uxTopReadyPriority); \
        }
    
    #endif
#endif

// 将任务添加到就绪列表
#define prvAddTaskToReadyList(pxTCB)                    \
        taskRECORD_READY_PRIORITY((pxTCB)->uxPriority); \
        vListInsertEnd( &(pxReadyTasksLists[(pxTCB)->uxPriority]),  &( (pxTCB)->xStateListItem) );

// 切换延时列表
#define taskSWITCH_DELAYED_LISTS()\
{\
    List_t *pxTemp;                                     \
    pxTemp = pxDelayedTaskList;                         \
    pxDelayedTaskList = pxOverflowDelayedTaskList;      \
    pxOverflowDelayedTaskList = pxTemp;                 \
    xNumOfOverflows++;                                  \
    prvResetNextTaskUnblockTime();                      \
}
    
/************* 多优先级相关定义   end *************/


#endif
