#include "./usart/bsp_usart.h"

/**
 * 串口配置函数
 * 1. 使能RX和TX引脚GPIO时钟和USART时钟；
 * 2. 初始化GPIO，并将GPIO复用到USART上；
 * 3. 配置USART参数；
 * （4. 配置中断控制）
 * 5. 使能USART；
 */
void Usart1_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. 使能RX和TX引脚GPIO时钟和USART时钟；
    RCC_AHB1PeriphClockCmd(USART1_TX_GPIO_CLK | USART1_RX_GPIO_CLK, ENABLE);
    USART1_CLOCK_CMD(USART1_CLK, ENABLE); // 宏展开 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // 2. 初始化GPIO，并将GPIO复用到USART上；
    // 外设复用：连接 端口 到USARTx_Tx，USARTx_Rx
    GPIO_PinAFConfig(USART1_TX_GPIO_PORT, USART1_TX_SOURCE, USART1_TX_AF);
    GPIO_PinAFConfig(USART1_RX_GPIO_PORT, USART1_RX_SOURCE, USART1_RX_AF);
    // 配置引脚的复用功能
    gpioInit.GPIO_OType = GPIO_OType_PP; 
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP; 
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART1_TX_PIN;
    GPIO_Init(USART1_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART1_RX_PIN;
    GPIO_Init(USART1_RX_GPIO_PORT, &gpioInit);
    
    // 3. 配置USART参数；
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usartInit);
    
    // 5. 使能USART；
    USART_Cmd(USART1, ENABLE);
}

void Usart2_Cfg(uint32_t baudrate, char tx){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    NVIC_InitTypeDef nvicInit;
    // 1. 使能RX和TX引脚GPIO时钟和USART时钟；
    RCC_AHB1PeriphClockCmd(USART2_TX_GPIO_CLK | USART2_RX_GPIO_CLK | RS485_RE_GPIO_CLK, ENABLE);
    USART2_CLOCK_CMD(USART2_CLK, ENABLE);
    
    // 2. 初始化GPIO，并将GPIO复用到USART上；
    // 外设复用：连接 端口 到USARTx_Tx，USARTx_Rx
    GPIO_PinAFConfig(USART2_TX_GPIO_PORT, USART2_TX_SOURCE, USART2_TX_AF);
    GPIO_PinAFConfig(USART2_RX_GPIO_PORT, USART2_RX_SOURCE, USART2_RX_AF);
    // 配置引脚的复用功能
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART2_TX_PIN;
    GPIO_Init(USART2_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART2_RX_PIN;
    GPIO_Init(USART2_RX_GPIO_PORT, &gpioInit);
    
    // 485收发控制
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpioInit.GPIO_Mode = GPIO_Mode_OUT;
    gpioInit.GPIO_Pin = RS485_RE_PIN;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_RE_GPIO_PORT, &gpioInit);
    
    // 3. 配置USART参数；
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    if(tx==1){
        usartInit.USART_Mode = USART_Mode_Tx;
    }else{
        usartInit.USART_Mode = USART_Mode_Rx;
    }        
    USART_Init(USART2, &usartInit);
    
    
    // 4. 配置中断并使能
    {
        // 配置中断源为USART2
        nvicInit.NVIC_IRQChannel = USART2_IRQn;
        // 配置主优先级（抢占优先级）
        nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
        // 配置子优先级
        nvicInit.NVIC_IRQChannelSubPriority = 0;
        // 使能中断
        nvicInit.NVIC_IRQChannelCmd = ENABLE;
        // 初始化
        NVIC_Init(&nvicInit);
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 空闲中断
    }
    
    // 5. 使能USART；
    USART_Cmd(USART2, ENABLE);
    
    
    
    if(tx==1){
        // 485进入发送模式
        GPIO_SetBits(RS485_RE_GPIO_PORT, RS485_RE_PIN);
    }else{
        // 485进入接收模式
        GPIO_ResetBits(RS485_RE_GPIO_PORT, RS485_RE_PIN);
    }
}

void Usart3_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. 使能RX和TX引脚GPIO时钟和USART时钟；
    RCC_AHB1PeriphClockCmd(USART3_TX_GPIO_CLK | USART3_RX_GPIO_CLK, ENABLE);
    USART3_CLOCK_CMD(USART3_CLK, ENABLE);
    
    // 2. 初始化GPIO，并将GPIO复用到USART上；
    // 外设复用：连接 端口 到USARTx_Tx，USARTx_Rx
    GPIO_PinAFConfig(USART3_TX_GPIO_PORT, USART3_TX_SOURCE, USART3_TX_AF);
    GPIO_PinAFConfig(USART3_RX_GPIO_PORT, USART3_RX_SOURCE, USART3_RX_AF);
    // 配置引脚的复用功能
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART3_TX_PIN;
    GPIO_Init(USART3_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART3_RX_PIN;
    GPIO_Init(USART3_RX_GPIO_PORT, &gpioInit);
    
    // 3. 配置USART参数；
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &usartInit);
    
    // 5. 使能USART；
    USART_Cmd(USART3, ENABLE);
}


// 发送一个字符
void Usart_SendByte(USART_TypeDef* pUsart, u8 ch){
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
    Usart_SendByte(PRINT_USART, ch);
    return ch;
}

// 重定向 scanf
int fgetc(FILE *f){
    // 等待串口输入
    while(USART_GetFlagStatus(PRINT_USART, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(PRINT_USART);
}
