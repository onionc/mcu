
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
BYTE xBuf[FF_MAX_SS]={0}; // ��������buffer
BYTE rBuf[4000]={0}; // ������buffer
BYTE wBuf[] = "����֮����¼�����������ͷ������ڳ��֮�¡����������ˮ�����ˡ��پ����ͣ�������֮ʫ������֮�¡����ɣ��³��ڶ�ɽ֮�ϣ��ǻ��ڶ�ţ֮�䡣��¶�Ὥ��ˮ����졣��һέ֮���磬������֮ãȻ���ƺƺ���������磬����֪����ֹ��ƮƮ���������������𻯶����ɡ�(�� ͨ��ƾ)\
���������������������϶���֮����Ի�����������������������������⡣�������軳������������һ���������д������ߣ��и����֮����������Ȼ����Թ��Ľ���������ߣ������������������ơ�������֮Ǳ�ԣ�������֮�˸���\
���������Ȼ������Σ�����ʿ�Ի������Ϊ��ȻҲ������Ի������������ϡ����ȵ�Ϸɡ����˷ǲ��ϵ�֮ʫ���������Ŀڣ����������ɽ�����ѣ������Բԣ��˷��ϵ�֮���������ߺ��������ƾ��ݣ��½��꣬˳������Ҳ������ǧ����οգ������ٽ������ø�ʫ����һ��֮��Ҳ���������գ��������������ڽ��֮�ϣ�����Ϻ������¹����һҶ֮���ۣ�������������������������أ���׺�֮һ�ڡ�������֮�������۳���֮���Ю���������Σ������¶����ա�֪���ɺ���ã��������ڱ��硣��\
��������Ի��������֪��ˮ���º���������˹����δ����Ҳ��ӯ������ˣ�����Ī����Ҳ���ǽ�������߶���֮���������������һ˲�����䲻���߶���֮���������ҽ��޾�Ҳ�����ֺ��ۺ����ҷ����֮�䣬���������������֮���У���һ����Īȡ��Ω����֮��磬��ɽ��֮���£�����֮��Ϊ����Ŀ��֮����ɫ��ȡ֮�޽�����֮���ߡ���������֮�޾���Ҳ����������֮�����ʡ���(���� һ������ʳ)\
������ϲ��Ц��ϴյ���á��Ⱥ˼Ⱦ��������Ǽ��������������У���֪����֮�Ȱס�";
const char *physicalNum = "0:"; // 1��SPI FLASH, 0��SD��
u32 num; // �ֽ���


#define FORCE_FORMAT    0   // ǿ�Ƹ�ʽ��
#define WRITE_TEST      1   // д���ݲ���
#define READ_TEST       1   // �����ݲ���
#define SEEK_TEST       1   // �ļ���λ����
#define RENAME_DEL_TEST     0   // ������ɾ��������Ŀ¼ ����



FRESULT formatSpace(); // ��ʽ��
FRESULT normalFuncTest(); // ���÷�������
FRESULT fileInfo(); // �ļ�����
FRESULT scanFiles(); // ɨ���ļ�


#define CONTACT(y) ("0:" y) // ƴ�ӵ��豸

int main(){
    u32 id;

    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF; // 1���������ļ�ϵͳ����
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart1_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\r\n");
    
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

    printf("-----------------�������ܲ���:-----------------\r\n");
#if WRITE_TEST==1
    
    /************* д���ݲ��� **************/
    fRes = f_open(&file, CONTACT("a.txt"), FA_CREATE_ALWAYS | FA_WRITE);
    if(fRes == FR_OK){
        printf("���ļ��ɹ���׼��д������\r\n");
        fRes = f_write(&file, wBuf, sizeof wBuf, &num);
        if(fRes == FR_OK){
            printf("�ļ�д��ɹ���д���ֽ�����%u\r\n", num);
            
        }else{
            printf("�ļ�д��ʧ��\r\n");
        }
        // �ر��ļ�
        f_close(&file);
        
        // �ļ�����
        fileInfo();
    }else{
        printf("���ļ�ʧ��\r\n");
        LED1_ON;
    }
#endif

#if READ_TEST==1
    /************* �����ݲ��� **************/
    fRes = f_open(&file, CONTACT("a.txt"), FA_OPEN_EXISTING | FA_READ);
    if(fRes == FR_OK){
        printf("���ļ��ɹ���׼����ȡ����\r\n");
        fRes = f_read(&file, rBuf, sizeof rBuf, &num);
        if(fRes == FR_OK){
            printf("�ļ���ȡ�ɹ����ֽ�����%u\r\n ���ݣ�\r\n%s\r\n", num, rBuf);
        }else{
            printf("�ļ���дʧ��\r\n");
        }
        // �ر��ļ�
        f_close(&file);
    }else{
        printf("���ļ�ʧ��\r\n");
        LED1_ON;
    }
    
    
#endif
    
    /************ ��ȡ�豸��Ϣ�ȳ��ù��ܲ��� ***************/
    printf("-----------------���ù��ܲ���-----------------\r\n");
    if((fRes=normalFuncTest()) != FR_OK){
        printf("�������� code=%d\r\n", fRes);
    }
    
    // ȡ������
    f_mount(NULL, physicalNum, 1);
    while(1){
        
        
    }
    
    return 0;
}


FRESULT formatSpace(){
    MKFS_PARM parm;
    printf("�������и�ʽ������\r\n");
        
    // ��ʽ��
    
    parm.fmt = FM_FAT32 | FM_SFD; // ʹ��FAT32
    parm.n_fat = 0; // �߼���������
    parm.align = SDCardInfo.CardBlockSize; // ������С
    parm.n_root = 0;
    parm.au_size = 0;
    
    // parm SD��ʹ��Ĭ��ֵ������ȷ��ʽ����������Ļ���ִ���
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
    
    printf("�豸��Ϣ��ȡ��\r\n");
    {
        // ��ȡ�豸��Ϣ�ʹش�С������1������
        fRes = f_getfree(physicalNum, &freeClust, &pFs);
        if(fRes!=FR_OK) return fRes;
        // ���������������Ϳ���������
        totalSect = (pFs->n_fatent - 2)* pFs->csize;
        freeSect = freeClust * pFs->csize;
        
        // ���ÿռ䰴�������£�4K�仯��ˢ��һ�Ρ�
        // ����һ��������512Bytes, /2 ����KB
        printf("�豸�ܿռ䣺%10lu KB, ���ÿռ� %10lu KB\r\n", totalSect/2, freeSect/2);
    }
    
    printf("�ļ���λ���ܣ�\r\n");
    #if SEEK_TEST==1
    {
        fRes = f_open(&file, CONTACT("a.txt"), FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if(fRes!=FR_OK) return fRes;

        // �ļ���λ��ĩβ
        printf("file seek:%lu\r\n", f_size(&file));
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
        printf("Ŀ¼�������ļ�ɾ������������\r\n");
        // Ŀ¼����
        fRes = f_opendir(&dir, CONTACT("doc"));
        if(fRes!=FR_OK){
            printf("����Ŀ¼\r\n");
            fRes = f_mkdir(CONTACT("doc"));
            if(fRes!=FR_OK) return fRes;
        }else{
            printf("Ŀ¼�Ѵ��ڣ��ر�Ŀ¼\r\n");
            fRes = f_closedir(&dir);
            if(fRes!=FR_OK) return fRes;
        }
        
        // �ļ�������
        printf("�ļ�������\r\n");
        fRes = f_rename(CONTACT("a.txt"), CONTACT("c.txt"));
        if(fRes!=FR_OK) return fRes;
        
        // �ļ�ɾ��
        printf("ɾ���ļ�\r\n");
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
    
    printf("1:a.txt�ļ���Ϣ��");
    printf("�ļ���С��", info.fsize );
    
    return FR_OK;
}

FRESULT scanFiles(){

    return FR_OK;
}
