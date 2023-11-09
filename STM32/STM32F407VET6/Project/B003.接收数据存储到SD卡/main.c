
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./ff.h"
#include <string.h>
#include "../sdio/bsp_sdio_sd.h"

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
u8 chTemp=0;
void USART2_IRQHandler(void){
    if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET){
        RxFlag = 1;
        chTemp = USART_ReceiveData(USART2);
    }
}


int main(){
    u32 id;
    u8 rxBuf[256]={0};
    u32 rxCount = 0;
    u32 rxCount2 = 0;
    

    
    // ����NVIC���ȼ�����Ϊ1(1λ�����ȼ���3λ�����ȼ�)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF; // 1���������ļ�ϵͳ����
    LED2_OFF;
    LED3_OFF;
    
    
    // ��ʼ��usart��usart2����485��0�������ģʽ
    Usart2_Cfg(921600, 0);
    Usart1_Cfg(9600);
    
    
    printf("���ڳ�ʼ���ɹ�\r\n");
    

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
        if(RxFlag){
           ;
            if(rxCount < sizeof(rxBuf)){
                rxBuf[rxCount++] = chTemp;
            }else{
                rxCount = 0;
            }
            
            RxFlag = 0;
        }else{
            // ����ʱд����
            if(rxCount>0){
                
                rxCount2 += rxCount;
                
                
                fRes = f_write(&file, rxBuf, rxCount, &num);
                if(fRes != FR_OK || num<1){
                    LED2_ON;
                }
                
                rxCount = 0;
            }
            
            if(rxCount2>1000){
                LED3_ON;
                // �ر��ļ�
                f_close(&file);
                // ȡ������
                f_mount(NULL, physicalNum, 1);
            }
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
