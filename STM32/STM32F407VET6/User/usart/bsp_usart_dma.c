#include "./usart/bsp_usart_dma.h"

u8 SendBuf[SEND_BUF_SIZE] = {1,2};


/**
 * 串口配置函数
 * 1. 使能RX和TX引脚GPIO时钟和USART时钟；
 * 2. 初始化GPIO，并将GPIO复用到USART上；
 * 3. 配置USART参数；
 * 5. 使能USART；
 * 6. （其他地方）使用实现数据接收和发送。
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

    // 5. 使能USART；
    USART_Cmd(USARTx, ENABLE);
}

// DMA配置函数，存储器到外设（USART1.DR)
ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    
    // 使能 DMA时钟
    RCC_AHB1PeriphClockCmd(USARTx_DMA_CLK, ENABLE);
    
    // 复位DMA数据流
    DMA_DeInit(USARTx_DMA_STREAM);
    
    // 等待复位完成
    while(DMA_GetCmdStatus(USARTx_DMA_STREAM) != DISABLE);
    
    // 配置 dma。usart1.tx对应dma1,通道4，数据流7
    dmaInit.DMA_Channel = USARTx_DMA_CHANNEL; // DMA数据流通道
    dmaInit.DMA_PeripheralBaseAddr = (u32)USARTx_DR_BASE; // 源数据为：UART1的数据寄存器地址
    dmaInit.DMA_Memory0BaseAddr = (u32)SendBuf; // 目标地址：内存地址
    dmaInit.DMA_DIR = DMA_DIR_MemoryToPeripheral; // 模式：存储器到外设
    dmaInit.DMA_BufferSize = (u32)SEND_BUF_SIZE; // 数据大小
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 使能外设地址不自动递增
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // 使能存储器自动递增
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 源数据宽度：字节大小 8bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 目标数据宽度：字节大小
    dmaInit.DMA_Mode = DMA_Mode_Circular; // 模式：循环
    dmaInit.DMA_Priority = DMA_Priority_Medium; // 数据流优先级：中
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFO模式不使能，FIFOThreshold阈值无效
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // 存储器突发模式：单次
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // 外设突发模式：单次
    // 配置DMA2的数据流7
    DMA_Init(USARTx_DMA_STREAM, &dmaInit);
    
    // 使能DMA数据流
    DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
    
    // 检测数据流是否有效
    while((DMA_GetCmdStatus(USARTx_DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // 判断是否超时
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
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
