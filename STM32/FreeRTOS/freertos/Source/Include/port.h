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

// �����쳣�˳�����
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



// �ٽ�μ���ֵ
static UBaseType_t uxCriticalNesting;

/* Masks off all bits but the VECTACTIVE bits in the ICSR register. */
#define portVECTACTIVE_MASK     ( 0xFFUL )


// ϵͳʱ�����
// SysTick ���ƼĴ���
#define portNVIC_SYSTICK_CTRL_REG (*((volatile uint32_t *) 0xE000E010))
// SysTick ��װ�ؼĴ���
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t *) 0xE000E014))
// SysTick ʱ��Դѡ��
#ifndef configSYSTICK_CLOCK_HZ
    #define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ

    #define portNVIC_SYSTICK_CLK_BIT (1<<2)
#else
    #define portNVIC_SYSTICK_CLK_BIT (0)
#endif
#define portNVIC_SYSTICK_INT_BIT    (1<<1)
#define portNVIC_SYSTICK_ENABLE_BIT (1<<0)
// SysTick ��ʼ������
void vPortSetupTimerInterrupt(void);

#endif
