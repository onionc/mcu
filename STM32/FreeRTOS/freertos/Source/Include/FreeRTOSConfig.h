#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdio.h>

// ָ��TickType_t���ͣ�=1Ϊ16λ��=0Ϊ32λ
#define configUSE_16_BIT_TICKS  0
// �������Ƶĳ���
#define configMAX_TASK_NAME_LEN 16
// ���������־��=1��̬����
#define configSUPPORT_STATIC_ALLOCATION 1
// ��������б��С��Ĭ��5��max=256
#define configMAX_PRIORITIES 5 

// ��������ջ��С
#define configMINIMAL_STACK_SIZE    ((unsigned short)128)

// �����ں����ȼ�
#define configKERNEL_INTERRUPT_PRIORITY 255 // 0xff 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191   /* ����λ��Ч��������0xb0��������11 */

// �жϷ�����
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler
#define vPortSVCHandler         SVC_Handler

// ����
#define vAssertCalled(file, line) printf("Error: %s, %d\n", file, line)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__, __LINE__)
    
// ʱ��
#define configCPU_CLOCK_HZ  (25000000UL)  // ϵͳʱ�� 25MHz
#define configTICK_RATE_HZ  ((TickType_t) 100) // SysTickÿ���жϣ�100Hz��Ϊ10ms/�Σ�

// ��������Ҿ����������ȼ���ʱ��ִ�������л�
#define configUSE_PREEMPTION 1
// ʱ��Ƭ
#ifndef configUSE_TIME_SLICING
    #define configUSE_TIME_SLICING 1
#endif
#endif
