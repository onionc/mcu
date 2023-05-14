#include "./usart/bsp_usart_dma.h"

u8 SendBuf[SEND_BUF_SIZE] = {1,2};


/**
 * �������ú���
 * 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
 * 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
 * 3. ����USART������
 * 5. ʹ��USART��
 * 6. �������ط���ʹ��ʵ�����ݽ��պͷ��͡�
 */
void Usart_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
    RCC_AHB1PeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
    USARTx_CLOCK_CMD(USARTx_CLK, ENABLE); // ��չ�� RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
    // ���踴�ã����� �˿� ��USARTx_Tx��USARTx_Rx
    GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
    GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
    // �������ŵĸ��ù���
    gpioInit.GPIO_OType = GPIO_OType_PP; 
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP; 
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USARTx_TX_PIN;
    GPIO_Init(USARTx_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USARTx_RX_PIN;
    GPIO_Init(USARTx_RX_GPIO_PORT, &gpioInit);
    
    // 3. ����USART������
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &usartInit);

    // 5. ʹ��USART��
    USART_Cmd(USARTx, ENABLE);
}

// DMA���ú������洢�������裨USART1.DR)
ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    
    // ʹ�� DMAʱ��
    RCC_AHB1PeriphClockCmd(USARTx_DMA_CLK, ENABLE);
    
    // ��λDMA������
    DMA_DeInit(USARTx_DMA_STREAM);
    
    // �ȴ���λ���
    while(DMA_GetCmdStatus(USARTx_DMA_STREAM) != DISABLE);
    
    // ���� dma��usart1.tx��Ӧdma1,ͨ��4��������7
    dmaInit.DMA_Channel = USARTx_DMA_CHANNEL; // DMA������ͨ��
    dmaInit.DMA_PeripheralBaseAddr = (u32)USARTx_DR_BASE; // Դ����Ϊ��UART1�����ݼĴ�����ַ
    dmaInit.DMA_Memory0BaseAddr = (u32)SendBuf; // Ŀ���ַ���ڴ��ַ
    dmaInit.DMA_DIR = DMA_DIR_MemoryToPeripheral; // ģʽ���洢��������
    dmaInit.DMA_BufferSize = (u32)SEND_BUF_SIZE; // ���ݴ�С
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // ʹ�������ַ���Զ�����
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // ʹ�ܴ洢���Զ�����
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // Դ���ݿ�ȣ��ֽڴ�С 8bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // Ŀ�����ݿ�ȣ��ֽڴ�С
    dmaInit.DMA_Mode = DMA_Mode_Circular; // ģʽ��ѭ��
    dmaInit.DMA_Priority = DMA_Priority_Medium; // ���������ȼ�����
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFOģʽ��ʹ�ܣ�FIFOThreshold��ֵ��Ч
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // �洢��ͻ��ģʽ������
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // ����ͻ��ģʽ������
    // ����DMA2��������7
    DMA_Init(USARTx_DMA_STREAM, &dmaInit);
    
    // ʹ��DMA������
    DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
    
    // ����������Ƿ���Ч
    while((DMA_GetCmdStatus(USARTx_DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // �ж��Ƿ�ʱ
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
}


// ����һ���ַ�
static void Usart_SendByte(USART_TypeDef* pUsart, u8 ch){
    // ����һ���ֽڵ�����
    USART_SendData(pUsart, ch);
    // �ȴ��������
    while(USART_GetFlagStatus(pUsart, USART_FLAG_TXE) == RESET);
}

// �����ַ�������, len==0ʱ����str�ַ�����\0��β��
void Usart_SendStr(USART_TypeDef* pUsart, u8 *str, u32 len){
    u32 k=0;
    do{
        Usart_SendByte(pUsart, *(str+k));
        k++;
    }while(len==0 ? (*(str+k)!=0) : (k<len) );
}

// �ض��� printf
int fputc(int ch, FILE *f){
    Usart_SendByte(USARTx, ch);
    return ch;
}

// �ض��� scanf
int fgetc(FILE *f){
    // �ȴ���������
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(USARTx);
}
