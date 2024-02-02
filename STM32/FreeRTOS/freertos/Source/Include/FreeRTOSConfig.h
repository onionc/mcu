#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// 指定TickType_t类型，=1为16位，=0为32位
#define configUSE_16_BIT_TICKS  0
// 任务名称的长度
#define configMAX_TASK_NAME_LEN 16
// 创建任务标志，=1静态创建
#define configSUPPORT_STATIC_ALLOCATION 1
// 任务就绪列表大小，默认5，max=256
#define configMAX_PRIORITIES 5 

// 配置内核优先级
#define configKERNEL_INTERRUPT_PRIORITY 255 // 0xff 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191   /* 高四位有效，即等于0xb0，或者是11 */

// 中断服务函数
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler
#define vPortSVCHandler         SVC_Handler
#endif
