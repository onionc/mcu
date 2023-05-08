#include "./key/bsp_exti_key.h"

/**
 * @brief 配置嵌套向量中断控制器 NVIC
 * 
 */
static void NVIC_Cfg(void){
    NVIC_InitTypeDef nvicInit;
    
    // 配置NVIC优先级分组为1(1位主优先级，3位子优先级)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // 配置中断源为按键1 
    nvicInit.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
    // 配置主优先级（抢占优先级）
    nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
    // 配置子优先级
    nvicInit.NVIC_IRQChannelSubPriority = 1;
    // 使能中断
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    // 初始化
    NVIC_Init(&nvicInit);
    
    // 配置按键2
    //nvicInit.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    //NVIC_Init(&nvicInit);
  
}

/**
 * @brief 配置 EXTI 中断
- 配置 NVIC
- 开启按键GPIO时钟和SYSCFG时钟
- 配置按键GPIO为输入模式
- 将按键GPIO连接到EXTI输入源
- 配置按键EXTI中断
 */
void EXTI_Key_Cfg(void){
    GPIO_InitTypeDef gpioInit;
    EXTI_InitTypeDef extiInit;
    
    // 配置 NVIC
    NVIC_Cfg();
    
    // 开启按键 GPIO口的时钟
    RCC_AHB1PeriphClockCmd(KEY1_INT_CLK, ENABLE);
    
    // 使能 SYSCFG 时钟，使用外部GPIO中断必须使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    // 配置按键 GPIO
    gpioInit.GPIO_Pin = KEY1_INT_PIN; // 按键1引脚
    gpioInit.GPIO_Mode = GPIO_Mode_IN; // 引脚为输入模式
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不上拉也不下拉
    GPIO_Init(KEY1_INT_PORT, &gpioInit);
    
    // 将按键GPIO连接到EXTI输入源
    SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    
    // 配置 exti 中断
    extiInit.EXTI_Line = KEY1_INT_EXTI_LINE; // 选择中断源
    extiInit.EXTI_Mode = EXTI_Mode_Interrupt; // 模式为中断模式
    extiInit.EXTI_Trigger = EXTI_Trigger_Rising; // 下降沿触发
    extiInit.EXTI_LineCmd = ENABLE; // 使能
    EXTI_Init(&extiInit);
    
    
    
    
    
    
    
    
    
}





