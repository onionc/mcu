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

// ��������ջ
extern StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];

// ������ƿ�
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB; // ����������ƿ�

extern TaskHandle_t TaskIdle_Handle;

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
    

/************* �ٽ����ض��� start *************/
    
// �����ٽ�Ρ������жϱ����汾������Ƕ��
#define taskENTER_CRITICAL()    portENTER_CRITICAL()  
// �����ٽ�Ρ����жϱ����汾������Ƕ��
#define taskENTER_CRITICAL_FROM_ISR()   portSET_INTERRUPT_MASK_FROM_ISR()
// �˳��ٽ�Ρ������жϱ����汾������Ƕ��
#define taskEXIT_CRITICAL()     portEXIT_CRITICAL()
// �˳��ٽ�Ρ����жϱ����汾������Ƕ��
#define taskEXIT_CRITICAL_FROM_ISR(x)   portCLEAR_INTERRUPT_MASK_FROM_ISR(x)
/************* �ٽ����ض��� end   *************/

// ��ȡ���������ڴ�(ջ������TCB��
void vApplicationGetIdleTaskMemory(TCB_t ** ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

// ������ʱ����
void vTaskDelay(const TickType_t xTicksToDelay);

// ʱ��������
static volatile TickType_t xTickCount;

// ����ϵͳʱ��
void xTaskIncrementTick(void);

// ��������
void prvIdleTask(void);

#endif
