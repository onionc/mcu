
#include "port.h"
#include "task.h"
#include "ARMCM4.h"
#include "portmacro.h"

// �ٽ�μ���ֵ
static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

// �����쳣�˳�����
static void prvTaskExitError(void){
    for(;;);
}


/**
 * ��ʼ������ջ����
 * @param pxTopOfStack ջ����ַ
 * @param pxCode ������
 * @parma pvParameters �����β�
 * @return 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters){
    // �쳣����ʱ���Զ����ص�CPU�Ĵ���������
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ( (StackType_t) pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t) prvTaskExitError;
    pxTopOfStack -= 5; // R12, R3, R2 and R1 Ĭ�ϲ�����ʼ��Ϊ0
    // R0 �����������ַ
    *pxTopOfStack = (StackType_t) pvParameters;
    
    // �쳣����ʱ���ֶ����ص�CPU�Ĵ���������
    pxTopOfStack -= 8;
    
    // ����ջ��ָ�룬��ʱָ�����ջ
    return pxTopOfStack;
}


//����������
BaseType_t xPortStartScheduler(void){
    // ���� PendSV �� SysTick ���ж����ȼ����
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;
    
    // ��ʼ��SysTick
    vPortSetupTimerInterrupt();
    
    uxCriticalNesting = 0;
    
    // ������һ�����񣬲��ٷ���
    prvStartFirstTask();
    
    return 0;
}

// ��ʼ���У���ʼ��һ������
__asm void prvStartFirstTask(){
    // 8�ֽڶ���
    PRESERVE8
    
    // SCB_VTOR��ַ���������������ʼ��ַ����MSP��ַ
    ldr r0, =0xE000ED08
    ldr r0, [r0]
    ldr r0, [r0]
    
    // ��������ջmsp��ֵ
    msr msp, r0
    // ʹ��ȫ���ж�
    cpsie i //���ж�
    cpsie f //���쳣
    dsb
    isb
    // ����SVCȥ������һ������
    svc 0
    nop
    nop
}

// SVC�жϷ���
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

// PendSV�жϷ���
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

// �����ٽ��
void vPortEnterCritical(void){
    portDISABLE_INTERRUPTS();
    uxCriticalNesting++;
    
    if(uxCriticalNesting == 1){
        configASSERT( (portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0 );
    }

}


// �˳��ٽ��
void vPortExitCritical(void){
    configASSERT(uxCriticalNesting);
    uxCriticalNesting--;
    if(uxCriticalNesting==0){
        portENABLE_INTERRUPTS();
    }
}


// SysTick �жϷ���
void xPortSysTickHandler(void){
    // ���ж�
    portDISABLE_INTERRUPTS();
    
    // ����ϵͳʱ��
    xTaskIncrementTick();
    
    // ���ж�
    portENABLE_INTERRUPTS();
}

// SysTick ��ʼ������
void vPortSetupTimerInterrupt(void){
    // ����װ�ؼĴ�����ֵ
    portNVIC_SYSTICK_LOAD_REG = (configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ) - 1; 
    
    // ʹ�ܶ�ʱ���жϣ�ʹ�ܶ�ʱ��
    portNVIC_SYSTICK_CTRL_REG = (
        portNVIC_SYSTICK_CLK_BIT |
        portNVIC_SYSTICK_INT_BIT | 
        portNVIC_SYSTICK_ENABLE_BIT
    );
}
