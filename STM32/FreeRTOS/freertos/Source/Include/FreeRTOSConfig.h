#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdio.h>

// 指定TickType_t类型，=1为16位，=0为32位
#define configUSE_16_BIT_TICKS  0
// 任务名称的长度
#define configMAX_TASK_NAME_LEN 16
// 创建任务标志，=1静态创建
#define configSUPPORT_STATIC_ALLOCATION 1
// 任务就绪列表大小，默认5，max=256
#define configMAX_PRIORITIES 5 

// 空闲任务栈大小
#define configMINIMAL_STACK_SIZE    ((unsigned short)128)

// 配置内核优先级
#define configKERNEL_INTERRUPT_PRIORITY 255 // 0xff 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191   /* 高四位有效，即等于0xb0，或者是11 */

// 中断服务函数
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler
#define vPortSVCHandler         SVC_Handler

// 断言
#define vAssertCalled(file, line) printf("Error: %s, %d\n", file, line)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__, __LINE__)
    
// 时钟
#define configCPU_CLOCK_HZ  (25000000UL)  // 系统时钟 25MHz
#define configTICK_RATE_HZ  ((TickType_t) 100) // SysTick每秒中断（100Hz则为10ms/次）

// 任务就绪且就绪任务优先级高时，执行任务切换
#define configUSE_PREEMPTION 1
// 时间片
#ifndef configUSE_TIME_SLICING
    #define configUSE_TIME_SLICING 1
#endif
#endif
