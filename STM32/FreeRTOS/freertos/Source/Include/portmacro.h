#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"
#include "FreeRTOSConfig.h"


/* 数据类型重定义 */
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



/******* 任务切换 *******/
// 中断控制状态寄存器
// BIT 28: PendSV 悬起位
#define portNVIC_INT_CTRL_REG       (*((volatile uint32_t *) 0xE000ED04)) 
#define portNVIC_PENDSVSET_BIT      (1UL<<28UL)
#define portSY_FULL_READ_WRITE      (15)
#define portYIELD()                                 \
{                                                   \
    /* 触发PendSV，产生上下文切换 */                  \
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
    __dsb( portSY_FULL_READ_WRITE );                \
    __dsb( portSY_FULL_READ_WRITE );                \
}
/******* 任务切换end *******/




/************* 临界段相关 start *************/
    
// 进入临界段。不带中断保护版本，不能嵌套
#define portENTER_CRITICAL()    vPortEnterCritical()

// 进入临界段。带中断保护版本，可以嵌套
#define taskENTER_CRITICAL_FROM_ISR()   portSET_INTERRUPT_MASK_FROM_ISR()

// 不带返回值的关中断函数，不能嵌套，不能在中断中使用
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

// 带返回值的关中断函数，可以嵌套，能在中断中使用
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

// 退出临界段。不带中断保护版本，不能嵌套
#define portEXIT_CRITICAL() vPortExitCritical()

// 退出临界段。带中断保护版本，可以嵌套
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortSetBASEPRI(x)

// 开中断函数，通过参数判断是否带中断保护，（0=不保护，else保护）
#define portENABLE_INTERRUPTS() vPortSetBASEPRI(0)
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortSetBASEPRI(x)
static portFORCE_INLINE void vPortSetBASEPRI(uint32_t ulBASEPRI){
    __asm
    {
        msr basepri, ulBASEPRI
    }
}


/************* 临界段相关 end   *************/

#endif /* PORTMACRO_H */
