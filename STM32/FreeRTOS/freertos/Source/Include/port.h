#ifndef PORT_H
#define PORT_H

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOSConfig.h"

// 任务栈相关
#define portINITIAL_XPSR        (0x01000000)
#define portSTART_ADDRESS_MASK  ( (StackType_t) 0xfffffffeUL )

// 调度器相关  SCB_SHPR3寄存器
#define portNVIC_SYSPRI2_REG    ( *((volatile uint32_t*)0xE000ED20) )   
#define portNVIC_PENDSV_PRI     ( ((uint32_t)configKERNEL_INTERRUPT_PRIORITY)<<16UL )  // Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
#define portNVIC_SYSTICK_PRI    ( ((uint32_t)configKERNEL_INTERRUPT_PRIORITY)<<24UL )  // Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception

static void prvTaskExitError(void);


/**
 * 初始化任务栈函数
 * @param pxTopOfStack 栈顶地址
 * @param pxCode 任务函数
 * @parma pvParameters 任务形参
 * @return 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters);

//启动调度器
BaseType_t xPortStartScheduler(void);

// 开始运行，开始第一个任务
void prvStartFirstTask(void);

// SVC中断服务
void vPortSVCHandler(void);

// PendSV中断服务
void xPortPendSVHandler(void);

#endif
