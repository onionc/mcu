#include "./usart/bsp_usart.h"
/**
 * @brief 配置嵌套向量中断控制器 NVIC
 * 
 */
static void NVIC_Cfg(void){
    NVIC_InitTypeDef nvicInit;
    
    // 配置NVIC优先级分组为1(1位主优先级，3位子优先级)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); // 只能有一次调用
    
    // 配置中断源为USART1
    nvicInit.NVIC_IRQChannel = USARTx_IRQ;
    // 配置主优先级（抢占优先级）
    nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
    // 配置子优先级
    nvicInit.NVIC_IRQChannelSubPriority = 1;
    // 使能中断
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    // 初始化
    NVIC_Init(&nvicInit);
}

// 中断处理函数
u8 RxFlag=0;
u8 chTemp=0;
void USARTx_IRQHandler(void){
    if(USART_GetITStatus(USARTx, USART_IT_RXNE)!=RESET){
        RxFlag = 1;
        chTemp = USART_ReceiveData(USARTx);
    }
}

/**
 * 串口配置函数
 * 1. 使能RX和TX引脚GPIO时钟和USART时钟；
 * 2. 初始化GPIO，并将GPIO复用到USART上；
 * 3. 配置USART参数；
 * 4. 配置中断控制器并使能USART接收中断；
 * 5. 使能USART；
 * 6. 在USART接收中断服务函数实现数据接收和发送。
 */
void Usart_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. 使能RX和TX引脚GPIO时钟和USART时钟；
    RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
    USARTx_CLOCK_CMD(USARTx_CLK, ENABLE); // 宏展开 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // 2. 初始化GPIO，并将GPIO复用到USART上；
    // 外设复用：连接 端口 到USARTx_Tx，USARTx_Rx
    GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
    GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
    // 配置引脚的复用功能
    gpioInit.GPIO_OType = GPIO_OType_PP; 
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP; 
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USARTx_TX_PIN;
    GPIO_Init(USARTx_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USARTx_RX_PIN;
    GPIO_Init(USARTx_RX_GPIO_PORT, &gpioInit);
    
    // 3. 配置USART参数；
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &usartInit);
    
    // 4. 配置中断控制器并使能USART接收中断；
    NVIC_Cfg();
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    
    // 5. 使能USART；
    USART_Cmd(USARTx, ENABLE);
}


// 发送一个字符
static void Usart_SendByte(USART_TypeDef* pUsart, u8 ch){
    // 发送一个字节到串口
    USART_SendData(pUsart, ch);
    // 等待发送完成
    while(USART_GetFlagStatus(pUsart, USART_FLAG_TXE) == RESET);
}

// 发送字符串函数, len==0时则发送str字符串（\0结尾）
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len){
    u32 k=0;
    do{
        Usart_SendByte(pUsart, *(str+k));
        k++;
    }while(len==0 ? (*(str+k)!=0) : (k<len) );
}

// 重定向 printf
int fputc(int ch, FILE *f){
    Usart_SendByte(USARTx, ch);
    return ch;
}

// 重定向 scanf
int fgetc(FILE *f){
    // 等待串口输入
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USARTx);
}