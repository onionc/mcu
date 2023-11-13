#include "./usart/bsp_usart.h"

/**
 * �������ú���
 * 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
 * 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
 * 3. ����USART������
 * ��4. �����жϿ��ƣ�
 * 5. ʹ��USART��
 */
void Usart1_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
    RCC_AHB1PeriphClockCmd(USART1_TX_GPIO_CLK | USART1_RX_GPIO_CLK, ENABLE);
    USART1_CLOCK_CMD(USART1_CLK, ENABLE); // ��չ�� RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
    // ���踴�ã����� �˿� ��USARTx_Tx��USARTx_Rx
    GPIO_PinAFConfig(USART1_TX_GPIO_PORT, USART1_TX_SOURCE, USART1_TX_AF);
    GPIO_PinAFConfig(USART1_RX_GPIO_PORT, USART1_RX_SOURCE, USART1_RX_AF);
    // �������ŵĸ��ù���
    gpioInit.GPIO_OType = GPIO_OType_PP; 
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP; 
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART1_TX_PIN;
    GPIO_Init(USART1_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART1_RX_PIN;
    GPIO_Init(USART1_RX_GPIO_PORT, &gpioInit);
    
    // 3. ����USART������
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usartInit);
    
    // 5. ʹ��USART��
    USART_Cmd(USART1, ENABLE);
}

void Usart2_Cfg(uint32_t baudrate, char tx){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    NVIC_InitTypeDef nvicInit;
    // 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
    RCC_AHB1PeriphClockCmd(USART2_TX_GPIO_CLK | USART2_RX_GPIO_CLK | RS485_RE_GPIO_CLK, ENABLE);
    USART2_CLOCK_CMD(USART2_CLK, ENABLE);
    
    // 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
    // ���踴�ã����� �˿� ��USARTx_Tx��USARTx_Rx
    GPIO_PinAFConfig(USART2_TX_GPIO_PORT, USART2_TX_SOURCE, USART2_TX_AF);
    GPIO_PinAFConfig(USART2_RX_GPIO_PORT, USART2_RX_SOURCE, USART2_RX_AF);
    // �������ŵĸ��ù���
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART2_TX_PIN;
    GPIO_Init(USART2_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART2_RX_PIN;
    GPIO_Init(USART2_RX_GPIO_PORT, &gpioInit);
    
    // 485�շ�����
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpioInit.GPIO_Mode = GPIO_Mode_OUT;
    gpioInit.GPIO_Pin = RS485_RE_PIN;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_RE_GPIO_PORT, &gpioInit);
    
    // 3. ����USART������
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
    
    
    // 4. �����жϲ�ʹ��
    {
        // �����ж�ԴΪUSART2
        nvicInit.NVIC_IRQChannel = USART2_IRQn;
        // ���������ȼ�����ռ���ȼ���
        nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
        // ���������ȼ�
        nvicInit.NVIC_IRQChannelSubPriority = 0;
        // ʹ���ж�
        nvicInit.NVIC_IRQChannelCmd = ENABLE;
        // ��ʼ��
        NVIC_Init(&nvicInit);
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // �����ж�
    }
    
    // 5. ʹ��USART��
    USART_Cmd(USART2, ENABLE);
    
    
    
    if(tx==1){
        // 485���뷢��ģʽ
        GPIO_SetBits(RS485_RE_GPIO_PORT, RS485_RE_PIN);
    }else{
        // 485�������ģʽ
        GPIO_ResetBits(RS485_RE_GPIO_PORT, RS485_RE_PIN);
    }
}

void Usart3_Cfg(uint32_t baudrate){
    GPIO_InitTypeDef gpioInit;
    USART_InitTypeDef usartInit;
    
    // 1. ʹ��RX��TX����GPIOʱ�Ӻ�USARTʱ�ӣ�
    RCC_AHB1PeriphClockCmd(USART3_TX_GPIO_CLK | USART3_RX_GPIO_CLK, ENABLE);
    USART3_CLOCK_CMD(USART3_CLK, ENABLE);
    
    // 2. ��ʼ��GPIO������GPIO���õ�USART�ϣ�
    // ���踴�ã����� �˿� ��USARTx_Tx��USARTx_Rx
    GPIO_PinAFConfig(USART3_TX_GPIO_PORT, USART3_TX_SOURCE, USART3_TX_AF);
    GPIO_PinAFConfig(USART3_RX_GPIO_PORT, USART3_RX_SOURCE, USART3_RX_AF);
    // �������ŵĸ��ù���
    gpioInit.GPIO_OType = GPIO_OType_PP;
    gpioInit.GPIO_PuPd = GPIO_PuPd_UP;
    gpioInit.GPIO_Mode = GPIO_Mode_AF;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    
    gpioInit.GPIO_Pin = USART3_TX_PIN;
    GPIO_Init(USART3_TX_GPIO_PORT, &gpioInit);
    gpioInit.GPIO_Pin = USART3_RX_PIN;
    GPIO_Init(USART3_RX_GPIO_PORT, &gpioInit);
    
    // 3. ����USART������
    usartInit.USART_BaudRate = baudrate;
    usartInit.USART_WordLength = USART_WordLength_8b;
    usartInit.USART_StopBits = USART_StopBits_1;
    usartInit.USART_Parity = USART_Parity_No;
    usartInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &usartInit);
    
    // 5. ʹ��USART��
    USART_Cmd(USART3, ENABLE);
}


// ����һ���ַ�
void Usart_SendByte(USART_TypeDef* pUsart, u8 ch){
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
    Usart_SendByte(PRINT_USART, ch);
    return ch;
}

// �ض��� scanf
int fgetc(FILE *f){
    // �ȴ���������
    while(USART_GetFlagStatus(PRINT_USART, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(PRINT_USART);
}
