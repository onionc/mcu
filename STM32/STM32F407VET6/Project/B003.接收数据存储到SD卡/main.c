
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./ff.h"
#include "../sdio/bsp_sdio_sd.h"
#include <string.h>

#define FRAME_LEN   300  // һ֡����
#define BUF_COUNT   10 // �����������������ļ�������Ҫʱ����10~20ms����Ҫ�������������������ݡ������СΪ 2~10����ʹ�� 20ms/һ֡����ʱ�䣨��5ms��*2 
#define SAVE_BYTES 30000 // �ȴ������ֽڱ���һ�Σ�����һ����������f_write����д��flash�����������С���Ժܴ󣬲�����FatFs buffer�Ĵ�С�����鸳ֵΪ���ɼ�ϵͳд��1s���ֽ���������LED�����ᶪ�������ֽ�����������
/* DMA �궨�� */
#define USART2_DR_BASE      (USART2_BASE+0x04)   // +0x04��USART�����ݼĴ�����ַ
#define USARTx_DMA_CLK      RCC_AHB1Periph_DMA1
#define USARTx_DMA_STREAM   DMA1_Stream5
#define USARTx_DMA_CHANNEL  DMA_Channel_4
#define RECV_BUF_SIZE       FRAME_LEN // DMA���յ��������������һ֡���ݣ���ʱ�����ȴ����У�
#define TIMEOUT_MAX         10000
u8 RxBuf[RECV_BUF_SIZE] = {0};

// DMA���ú������洢�������裨USART2.DR)
ErrorStatus DMA_Cfg(void);
void DMA1_Stream5_IRQHandler(void);
    
// ��������
FATFS fs;           //�ļ�ϵͳ����
FIL file;           // �ļ�����
FRESULT fRes;      // �ļ��������

const char *physicalNum = "0:"; // 1��SPI FLASH, 0��SD��
u32 num; // �ֽ���
BYTE xBuf[FF_MAX_SS]={0}; // ��������buffer


#define FORCE_FORMAT    0   // ǿ�Ƹ�ʽ��



FRESULT formatSpace(); // ��ʽ��
FRESULT normalFuncTest(); // ���÷�������
FRESULT fileInfo(); // �ļ�����
FRESULT scanFiles(); // ɨ���ļ�


#define CONTACT(y) ("0:" y) // ƴ�ӵ��豸

// �жϴ�����
u8 RxFlag=0;

// ������
struct BUF_ARR_T{
    char buffer[RECV_BUF_SIZE];
    u16 len;
}; 

struct BUF_ARR_T bufT[BUF_COUNT];
int bufIndex=0,useIndex=0;

u32 rxCount = 0;


void USART2_IRQHandler(void){
    u8 temp;
    int recvLen;
    // ���ڿ����ж�
    if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET){
        // ��ȡDMA�е�����
        {
            DMA_Cmd(USARTx_DMA_STREAM, DISABLE);
            
            recvLen = RECV_BUF_SIZE - DMA_GetCurrDataCounter(USARTx_DMA_STREAM); // ͨ��ʣ������жϳ���
            
            DMA_SetCurrDataCounter(USARTx_DMA_STREAM, RECV_BUF_SIZE); // ��Ҫ�Ѽ������û�ȥ
            if(recvLen>0){
                RxFlag = 1;

                // ��ֵ��������
                memcpy(bufT[bufIndex].buffer, RxBuf, recvLen);
                bufT[bufIndex].len = recvLen;
                
                bufIndex++;
                bufIndex%=BUF_COUNT;
            
            }
            
            DMA_ClearFlag(USARTx_DMA_STREAM, DMA_FLAG_TCIF5); // DMA��������ж�����
            DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
            
            
        }
        
        // ���IDLE��־
        temp = USART2->SR;
        temp = USART2->DR;
    }
    
}


int main(){
    u32 id;
    u8 rxBuf[256]={0};

    
    // ����NVIC���ȼ�����Ϊ1(1λ�����ȼ���3λ�����ȼ�)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF; // 1���������ļ�ϵͳ����
    LED2_OFF;
    LED3_OFF;

    // ��ʼ��usart��usart2����485��0�������ģʽ
    Usart2_Cfg(921600, 0);
    Usart1_Cfg(115200);
    
    printf("���ڳ�ʼ���ɹ�\r\n");
    
    // dma����
    {
        // ����ʹ��DMA
        if(DMA_Cfg()==ERROR){
            printf("DMA��ʼ��ʧ��\r\n");
        }else{
            printf("DMA��ʼ���ɹ�\r\n");
        }
        
    }
    

    // ����SD��
    {
        // ��SPI FLASH�����ļ�ϵͳ�����ʼ���豸
        fRes = f_mount(&fs, physicalNum, 1); // �ļ������豸��ţ���������
        
        /************* ��ʽ������ **************/
        // û���ļ�ϵͳ
        if(FORCE_FORMAT || fRes == FR_NO_FILESYSTEM){
            printf("û���ļ�ϵͳ\r\n");
            fRes=formatSpace();
        }
        
        if(fRes==FR_OK){
            printf("�ļ�ϵͳ���سɹ�\r\n");
        }else{
            printf("�ļ�ϵͳ����ʧ�� code=%d\r\n", fRes);
            LED1_ON;
        }
    }

    // �����ļ�
    {
        fRes = f_open(&file, CONTACT("c.raw"), FA_CREATE_ALWAYS | FA_WRITE);
        if(fRes != FR_OK){
            //printf("���ļ�ʧ��\r\n");
            LED1_ON;
        }
    }

    while(1){
  
            
        while(useIndex!=bufIndex){
            // д���ݵ��ļ���δ����
            rxCount+=bufT[useIndex].len;
            fRes = f_write(&file, bufT[useIndex].buffer, bufT[useIndex].len, &num);
            bufT[useIndex].len = 0;
            
            useIndex++;
            useIndex%=BUF_COUNT;
           
            if(fRes != FR_OK || num<1){
                LED2_ON;
            }
        }

        
        // ���浽�ļ����ᶪ������趨
        if(rxCount>SAVE_BYTES){
            // �����ļ�
            f_sync(&file);
            
            rxCount=0;
            
            LED3_TOGGLE;
        }


    }
    
    // �ر��ļ�
    // f_close(&file);
    // ȡ������
    // f_mount(NULL, physicalNum, 1);
    
    return 0;
}


FRESULT formatSpace(){
    MKFS_PARM parm;
    printf("�������и�ʽ������\r\n");
        
    // ��ʽ��
    
    parm.fmt = FM_FAT32 | FM_SFD; // ʹ��FAT32
    parm.n_fat = 0; // �߼���������
    parm.align = 0; // ��������������
    parm.n_root = 0;
    parm.au_size = 4096; // �ش�С��Ҫ���õĴش�С,���ֽ�Ϊ��λ
    
    fRes = f_mkfs(physicalNum, &parm, xBuf, sizeof xBuf);
    
    if(fRes==FR_OK){
        printf("�Ѹ�ʽ���ļ�ϵͳ\r\n");
        // ���¹���
        fRes = f_mount(NULL, physicalNum, 1);
        fRes = f_mount(&fs, physicalNum, 1);
    }
    return fRes;
}

FRESULT normalFuncTest(){
    DIR dir;
    FATFS *pFs;
    u32 freeClust, freeSect, totalSect;
    
    //printf("�豸��Ϣ��ȡ��\r\n");
    {
        // ��ȡ�豸��Ϣ�ʹش�С������1������
        fRes = f_getfree(physicalNum, &freeClust, &pFs);
        if(fRes!=FR_OK) return fRes;
        // ���������������Ϳ���������
        totalSect = (pFs->n_fatent - 2)* pFs->csize;
        freeSect = freeClust * pFs->csize;
        
        // ���ÿռ䰴�������£�4K�仯��ˢ��һ�Ρ�
        // ����һ��������512Bytes, /2 ����KB
        //printf("�豸�ܿռ䣺%10lu KB, ���ÿռ� %10lu KB\r\n", totalSect/2, freeSect/2);
    }
    
    //printf("�ļ���λ���ܣ�\r\n");
    #if SEEK_TEST==1
    {
        fRes = f_open(&file, CONTACT("a.txt"), FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if(fRes!=FR_OK) return fRes;

        // �ļ���λ��ĩβ
        //printf("file seek:%lu\r\n", f_size(&file));
        fRes = f_lseek(&file, f_size(&file));
        if(fRes!=FR_OK) return fRes;
        
        // ׷��һ������
        f_printf(&file, "\n׷��һ��, ��ǰ�ļ���С��%lu\n �豸�ܿռ䣺%10lu KB, ���ÿռ� %10lu KB\n", f_size(&file), totalSect/2, freeSect/2);
        
        // ��λ����ʼλ��
        fRes = f_lseek(&file, 0);
        if(fRes!=FR_OK) return fRes;
        
        // ��ȡ����
        //fRes = f_read(&file, rBuf, f_size(&file), &num);
        //if(fRes!=FR_OK) return fRes;
        //printf("�ļ�����Ϊ��%s\r\n", rBuf);
        
        f_close(&file);
    }
    #endif
    
    
    #if RENAME_DEL_TEST==1
    {
        //printf("Ŀ¼�������ļ�ɾ������������\r\n");
        // Ŀ¼����
        fRes = f_opendir(&dir, CONTACT("doc"));
        if(fRes!=FR_OK){
            //printf("����Ŀ¼\r\n");
            fRes = f_mkdir(CONTACT("doc"));
            if(fRes!=FR_OK) return fRes;
        }else{
            //printf("Ŀ¼�Ѵ��ڣ��ر�Ŀ¼\r\n");
            fRes = f_closedir(&dir);
            if(fRes!=FR_OK) return fRes;
        }
        
        // �ļ�������
        //printf("�ļ�������\r\n");
        fRes = f_rename(CONTACT("a.txt"), CONTACT("c.txt"));
        if(fRes!=FR_OK) return fRes;
        
        // �ļ�ɾ��
        //printf("ɾ���ļ�\r\n");
        fRes = f_unlink(CONTACT("c.txt"));
        if(fRes!=FR_OK) return fRes;
    }
    #endif
    
    return FR_OK;
}

FRESULT fileInfo(){
    FILINFO info;
    
    fRes = f_stat(CONTACT("a.txt"), &info);
    if(fRes!=FR_OK) return fRes;
    
    //printf("1:a.txt�ļ���Ϣ��");
    //printf("�ļ���С��", info.fsize );
    
    return FR_OK;
}

FRESULT scanFiles(){

    return FR_OK;
}


// DMA���ú������洢�������裨USART1.DR)
ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    NVIC_InitTypeDef nvicInit;
    
    // ʹ�� DMAʱ��
    RCC_AHB1PeriphClockCmd(USARTx_DMA_CLK, ENABLE);
    
    // ��λDMA������
    DMA_DeInit(USARTx_DMA_STREAM);
    
    // �ȴ���λ���
    while(DMA_GetCmdStatus(USARTx_DMA_STREAM) != DISABLE);
    
    // ���� dma��usart2.Rx��Ӧdma1,ͨ��4��������5
    dmaInit.DMA_Channel = USARTx_DMA_CHANNEL; // DMA������ͨ��
    dmaInit.DMA_PeripheralBaseAddr = (u32)USART2_DR_BASE; // �����ַ��UART1�����ݼĴ�����ַ
    dmaInit.DMA_Memory0BaseAddr = (u32)RxBuf; // �洢����ַ���ڴ��ַ
    dmaInit.DMA_DIR = DMA_DIR_PeripheralToMemory; // ģʽ�����赽�洢��
    dmaInit.DMA_BufferSize = (u32)RECV_BUF_SIZE; // ���ݴ�С
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // ʹ�������ַ���Զ�����
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // ʹ�ܴ洢���Զ�����
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // Դ���ݿ�ȣ��ֽڴ�С 8bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // Ŀ�����ݿ�ȣ��ֽڴ�С
    dmaInit.DMA_Mode = DMA_Mode_Normal; // ģʽ��ѭ��
    dmaInit.DMA_Priority = DMA_Priority_High; // ���������ȼ�����
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFOģʽ��ʹ�ܣ�FIFOThreshold��ֵ��Ч
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // �洢��ͻ��ģʽ������
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // ����ͻ��ģʽ������
    DMA_Init(USARTx_DMA_STREAM, &dmaInit); // ����DMA������Ϊ������
    
    // ʹ��DMA������
    DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
    
    // USART1 �� DMA����RX����
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    
    // ˫���忪��
    // DMA_DoubleBufferModeConfig(USARTx_DMA_STREAM, (u32)RxBuf2, DMA_Memory_0);
    // DMA_DoubleBufferModeCmd(USARTx_DMA_STREAM, ENABLE);
    
    
    // ʹ��DMA�ж�
    /*
    DMA_ITConfig(USARTx_DMA_STREAM, DMA_IT_TC, ENABLE); // ������ɺʹ����ж�
    DMA_ClearITPendingBit(USARTx_DMA_STREAM, DMA_IT_TCIF5);
    
    // ����DMA�ж����������ȼ�
    nvicInit.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
    nvicInit.NVIC_IRQChannelSubPriority = 0;
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicInit);
    */
    
    
    
    
    
    // ����������Ƿ���Ч
    while((DMA_GetCmdStatus(USARTx_DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // �ж��Ƿ�ʱ
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
}
