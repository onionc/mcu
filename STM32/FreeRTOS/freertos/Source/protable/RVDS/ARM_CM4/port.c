
#include "port.h"
#include "task.h"
#include "ARMCM4.h"
#include "portmacro.h"

// 临界段计数值
static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

// 任务异常退出函数
static void prvTaskExitError(void){
    for(;;);
}


/**
 * 初始化任务栈函数
 * @param pxTopOfStack 栈顶地址
 * @param pxCode 任务函数
 * @parma pvParameters 任务形参
 * @return 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters){
    // 异常发生时，自动加载到CPU寄存器的内容
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ( (StackType_t) pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) prvTaskExitError;
    pxTopOfStack -= 5; // R12, R3, R2 and R1 默认参数初始化为0
    // R0 是任务参数地址
    *pxTopOfStack = (StackType_t) pvParameters;
    
    // 异常发生时，手动加载到CPU寄存器的内容
    pxTopOfStack -= 8;
    
    // 返回栈顶指针，此时指向空闲栈
    return pxTopOfStack;
}


//启动调度器
BaseType_t xPortStartScheduler(void){
    // 配置 PendSV 和 SysTick 的中断优先级最低
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
    
    // 初始化SysTick
    vPortSetupTimerInterrupt();
    
    uxCriticalNesting = 0;
    
    // 启动第一个任务，不再返回
    prvStartFirstTask();
    
    return 0;
}

// 开始运行，开始第一个任务
__asm void prvStartFirstTask(){
    // 8字节对齐
    PRESERVE8
    
    // SCB_VTOR地址，存放向量表的起始地址，即MSP地址
    ldr r0, =0xE000ED08
    ldr r0, [r0]
    ldr r0, [r0]
    
    // 设置主堆栈msp的值
    msr msp, r0
    // 使能全局中断
    cpsie i //开中断
    cpsie f //开异常
    dsb
    isb
    // 调用SVC去启动第一个任务
    svc 0
    nop
    nop
}

// SVC中断服务
__asm void vPortSVCHandler(void){
    extern pxCurrentTCB;
    
    PRESERVE8
    
    ldr r3, =pxCurrentTCB
    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    mov r0, #0
    msr basepri, r0
    orr r14, #0xd
    
    bx r14
}

// PendSV中断服务
__asm void xPortPendSVHandler(void){
    extern pxCurrentTCB;
    extern vTaskSwitchContext;
    
    PRESERVE8
    
    mrs r0, psp
    isb
    
    ldr r3, =pxCurrentTCB
    ldr r2, [r3]
    
    stmdb r0!, {r4-r11}
    str r0, [r2]
    
    stmdb sp!, {r3, r14}
    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0
    dsb
    bl vTaskSwitchContext
    mov r0, #0
    msr basepri, r0
    ldmia sp!, {r3, r14}
    
    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0

    isb
    bx r14
    nop
}

// 进入临界段
void vPortEnterCritical(void){
    portDISABLE_INTERRUPTS();
    uxCriticalNesting++;
    
    if(uxCriticalNesting == 1){
        configASSERT( (portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0 );
    }

}


// 退出临界段
void vPortExitCritical(void){
    configASSERT(uxCriticalNesting);
    uxCriticalNesting--;
    if(uxCriticalNesting==0){
        portENABLE_INTERRUPTS();
    }
}


// SysTick 中断服务
void xPortSysTickHandler(void){
    // 关中断
    portDISABLE_INTERRUPTS();
    
    
    // 更新系统时基
    if(xTaskIncrementTick() != pdFALSE){
        // 任务切换
        taskYIELD();
    }
    
    // 开中断
    portENABLE_INTERRUPTS();
}

// SysTick 初始化函数
void vPortSetupTimerInterrupt(void){
    // 设置装载寄存器的值
    portNVIC_SYSTICK_LOAD_REG = (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ) - 1; 
    
    // 使能定时器中断，使能定时器
    portNVIC_SYSTICK_CTRL_REG = (
        portNVIC_SYSTICK_CLK_BIT |
        portNVIC_SYSTICK_INT_BIT | 
        portNVIC_SYSTICK_ENABLE_BIT
    );
}

