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

// 任务控制块
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;

// 指向当前任务的指针
extern TCB_t * pxCurrentTCB; 

// 任务切换方法
#define taskYIELD()     portYIELD()

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
);
// 创建新任务
static void prvInitialiseNewTask(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
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
    
#endif
