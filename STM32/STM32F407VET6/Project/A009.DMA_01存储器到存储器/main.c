
#include "stm32f4xx.h"
#include "./led/bsp_led.h"

/* �궨�塣�洢�����洢����Ҫ��DMA2��������ͨ����Ҫ����Ϊû�õ���������*/
#define DMA_STREAM          DMA2_Stream0
#define DMA_CHANNEL         DMA_Channel_0
#define DMA_STREAM_CLOCK    RCC_AHB1Periph_DMA2
#define DMA_FLAG_TCIF       DMA_FLAG_TCIF0

#define BUFFER_SIZE         32
#define TIMEOUT_MAX         10000

/* �����������飬Դ��Ŀ�Ĵ洢�� */
const u32 aSrcBuf[BUFFER_SIZE] = {
    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80
};
u32 aDstBuf[BUFFER_SIZE] = {0};

// dma���ò���������
static ErrorStatus DMA_Cfg(void); 
// �ȶԺ���
ErrorStatus cmp(const u32 *pBuf1, const u32 *pBuf2, u16 len); 

int main(){
    // �洢���д���DMA���ݣ�LED3������DMA��ʼ����LED2���������ݱȶԴ���LED1���������ݱȶ���ȷ
    
    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;

    // dma���úʹ�������
    if(DMA_Cfg()==ERROR){
        LED3_ON;
    }else{
        // �ȴ��������
        while(DMA_GetFlagStatus(DMA_STREAM, DMA_FLAG_TCIF) == DISABLE);
        
        // �ж������Ƿ�һ��
        if(cmp(aSrcBuf, aDstBuf, BUFFER_SIZE)==SUCCESS){
            LED1_ON;
        }else{
            LED2_ON;
        }
    }

    while(1){}
    
    return 0;
}

// dma���ò���������
static ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    
    // ʹ�� DMAʱ��
    RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
    
    // ��λDMA������
    DMA_DeInit(DMA_STREAM);
    
    // �ȴ���λ���
    while(DMA_GetCmdStatus(DMA_STREAM) != DISABLE);
    
    // ���� dma
    dmaInit.DMA_Channel = DMA_CHANNEL; // DMA������ͨ��
    dmaInit.DMA_PeripheralBaseAddr = (u32)aSrcBuf; // Դ���ݵ�ַ
    dmaInit.DMA_Memory0BaseAddr = (u32)aDstBuf; // Ŀ���ַ
    dmaInit.DMA_DIR = DMA_DIR_MemoryToMemory; // ģʽ���洢�����洢��
    dmaInit.DMA_BufferSize = (u32)BUFFER_SIZE; // ���ݴ�С
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Enable; // ʹ�������Զ�����
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // ʹ�ܴ洢���Զ�����
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // Դ���ݿ�ȣ��ִ�С 32bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // Ŀ�����ݿ�ȣ��ִ�С
    dmaInit.DMA_Mode = DMA_Mode_Normal; // ģʽ��һ�δ���
    dmaInit.DMA_Priority = DMA_Priority_High; // ���������ȼ�����
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFOģʽ��ʹ�ܣ�FIFOThreshold��ֵ��Ч
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // �洢��ͻ��ģʽ������
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // ����ͻ��ģʽ������
    // ��ʼ��
    DMA_Init(DMA_STREAM, &dmaInit);
    
    // ���DMA������������ɱ�־
    DMA_ClearFlag(DMA_STREAM, DMA_FLAG_TCIF);
    
    // ʹ������������ʼDMA���ݴ���
    DMA_Cmd(DMA_STREAM, ENABLE);
    
    // ����������Ƿ���Ч
    while((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // �ж��Ƿ�ʱ
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
}
// �ȶԺ���
ErrorStatus cmp(const u32 *pBuf1, const u32 *pBuf2, u16 len){
    while(len--){
        if(*pBuf1++ != *pBuf2++){
            return ERROR;
        }
        
    }
    return SUCCESS;
}
