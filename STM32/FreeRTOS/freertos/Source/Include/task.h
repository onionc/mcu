#ifndef TASK_H
#define TASK_H


#include "portmacro.h"
#include "task.h"
#include "FreeRTOS.h"
#include "projdefs.h"

typedef void * TaskHandle_t;

// ��������ջ
#define TASK1_STACK_SIZE    128
extern StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE    128
extern StackType_t Task2Stack[TASK2_STACK_SIZE];

// ������ƿ�
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;

// ָ��ǰ�����ָ��
extern TCB_t * pxCurrentTCB; 

// �����л�����
#define taskYIELD()     portYIELD()

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
);
// ����������
static void prvInitialiseNewTask(
    TaskFunction_t pxTaskCode,
    const char* const pcName,
    const uint32_t ulStackDepth,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask,
    TCB_t * pxNewTCB
);


    
// ��������б�
extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

// �����б��ʼ��
void prvInitialiseTaskLists(void);
    
// ������
void vTaskStartScheduler(void);

// �����л�
void vTaskSwitchContext(void);
    
#endif
