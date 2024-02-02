#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// ָ��TickType_t���ͣ�=1Ϊ16λ��=0Ϊ32λ
#define configUSE_16_BIT_TICKS  0
// �������Ƶĳ���
#define configMAX_TASK_NAME_LEN 16
// ���������־��=1��̬����
#define configSUPPORT_STATIC_ALLOCATION 1
// ��������б��С��Ĭ��5��max=256
#define configMAX_PRIORITIES 5 

// �����ں����ȼ�
#define configKERNEL_INTERRUPT_PRIORITY 255 // 0xff 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191   /* ����λ��Ч��������0xb0��������11 */

// �жϷ�����
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler
#define vPortSVCHandler         SVC_Handler
#endif
