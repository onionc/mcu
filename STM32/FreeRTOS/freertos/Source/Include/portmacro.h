#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"
#include "FreeRTOSConfig.h"


/* ���������ض��� */
#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
    typedef uint16_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffff
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif


#define portINLINE __inline

#ifndef portFORCE_INLINE
    #define portFORCE_INLINE __forceinline
#endif



/******* �����л� *******/
// �жϿ���״̬�Ĵ���
// BIT 28: PendSV ����λ
#define portNVIC_INT_CTRL_REG       (*((volatile uint32_t *) 0xE000ED04)) 
#define portNVIC_PENDSVSET_BIT      (1UL<<28UL)
#define portSY_FULL_READ_WRITE      (15)
#define portYIELD()                                 \
{                                                   \
    /* ����PendSV�������������л� */                  \
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
    __dsb( portSY_FULL_READ_WRITE );                \
    __dsb( portSY_FULL_READ_WRITE );                \
}
/******* �����л�end *******/




/************* �ٽ����� start *************/
    
// �����ٽ�Ρ������жϱ����汾������Ƕ��
#define portENTER_CRITICAL()    vPortEnterCritical()

// �����ٽ�Ρ����жϱ����汾������Ƕ��
#define taskENTER_CRITICAL_FROM_ISR()   portSET_INTERRUPT_MASK_FROM_ISR()

// ��������ֵ�Ĺ��жϺ���������Ƕ�ף��������ж���ʹ��
#define portDISABLE_INTERRUPTS()    vPortRaiseBASEPRI()
static portFORCE_INLINE void vPortRaiseBASEPRI(void){
    uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    __asm
    {
        msr basepri, ulNewBASEPRI
        dsb
        isb
    }
}

// ������ֵ�Ĺ��жϺ���������Ƕ�ף������ж���ʹ��
#define portSET_INTERRUPT_MASK_FROM_ISR() ulPortRaiseBASEPRI()
static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI(void){
    uint32_t ulReturn, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    __asm
    {
        mrs ulReturn, basepri
        msr basepri, ulNewBASEPRI
        dsb
        isb
    }
    return ulReturn;
}

// �˳��ٽ�Ρ������жϱ����汾������Ƕ��
#define portEXIT_CRITICAL() vPortExitCritical()

// �˳��ٽ�Ρ����жϱ����汾������Ƕ��
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortSetBASEPRI(x)

// ���жϺ�����ͨ�������ж��Ƿ���жϱ�������0=��������else������
#define portENABLE_INTERRUPTS() vPortSetBASEPRI(0)
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortSetBASEPRI(x)
static portFORCE_INLINE void vPortSetBASEPRI(uint32_t ulBASEPRI){
    __asm
    {
        msr basepri, ulBASEPRI
    }
}


/************* �ٽ����� end   *************/

#endif /* PORTMACRO_H */
