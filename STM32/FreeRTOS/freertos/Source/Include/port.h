#ifndef PORT_H
#define PORT_H

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOSConfig.h"

// ����ջ���
#define portINITIAL_XPSR        (0x01000000)
#define portSTART_ADDRESS_MASK  ( (StackType_t) 0xfffffffeUL )

// ���������  SCB_SHPR3�Ĵ���
#define portNVIC_SYSPRI2_REG    ( *((volatile uint32_t*)0xE000ED20) )   
#define portNVIC_PENDSV_PRI     ( ((uint32_t)configKERNEL_INTERRUPT_PRIORITY)<<16UL )  // Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
#define portNVIC_SYSTICK_PRI    ( ((uint32_t)configKERNEL_INTERRUPT_PRIORITY)<<24UL )  // Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception

static void prvTaskExitError(void);


/**
 * ��ʼ������ջ����
 * @param pxTopOfStack ջ����ַ
 * @param pxCode ������
 * @parma pvParameters �����β�
 * @return 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters);

//����������
BaseType_t xPortStartScheduler(void);

// ��ʼ���У���ʼ��һ������
void prvStartFirstTask(void);

// SVC�жϷ���
void vPortSVCHandler(void);

// PendSV�жϷ���
void xPortPendSVHandler(void);

#endif
