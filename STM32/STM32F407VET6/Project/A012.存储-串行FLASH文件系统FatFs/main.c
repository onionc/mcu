
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"

#include "./ff.h"

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
const char *physicalNum = "1:";
u32 num; // �ֽ���


#define FORCE_FORMAT    0   // ǿ�Ƹ�ʽ��
#define WRITE_TEST      0   // д���ݲ���
#define READ_TEST       0   // �����ݲ���


FRESULT formatSpace(); // ��ʽ��
FRESULT miscellaneous(); // ��ȡ����

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
    fRes = f_open(&file, "1:a.txt", FA_CREATE_ALWAYS | FA_WRITE);
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
    }else{
        printf("���ļ�ʧ��\r\n");
        LED1_ON;
    }
#endif

#if READ_TEST==1
    /************* �����ݲ��� **************/
    fRes = f_open(&file, "1:a.txt", FA_OPEN_EXISTING | FA_READ);
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
    
    // ȡ������
    f_mount(NULL, physicalNum, 1);
#endif
    
    /************ ��ȡ�豸��Ϣ�ȳ��ù��ܲ��� ***************/
    printf("-----------------���ù��ܲ���-----------------\r\n");
    if((fRes=miscellaneous()) != FR_OK){
        printf("�������� code=%d\r\n", fRes);
    }
    
    while(1){
        
        
    }
    
    return 0;
}

FRESULT formatSpace(){
    MKFS_PARM parm;
    printf("�������и�ʽ������\r\n");
        
    // ��ʽ��
    parm.fmt = FM_FAT | FM_SFD;
    parm.n_fat = 0; // �߼���������
    parm.align = 512; // ������С
    parm.n_root = 0;
    parm.au_size = 0;
    fRes = f_mkfs(physicalNum, &parm, xBuf, sizeof xBuf);
    
    if(fRes==FR_OK){
        printf("�Ѹ�ʽ���ļ�ϵͳ\r\n");
        // ���¹���
        fRes = f_mount(NULL, physicalNum, 1);
        fRes = f_mount(&fs, physicalNum, 1);
    }
    return fRes;
}

FRESULT miscellaneous(){
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
        printf("�豸�ܿռ䣺%10lu KB, ���ÿռ� %10lu KB\r\n", totalSect*4, freeSect*4);
    }
    
    printf("�ļ���λ���ܣ�\r\n");
    if(1){
        fRes = f_open(&file, "1:b.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if(fRes!=FR_OK) return fRes;

        // �ļ���λ��ĩβ
        printf("file seek:%lu\r\n", f_size(&file));
        fRes = f_lseek(&file, f_size(&file));
        if(fRes!=FR_OK) return fRes;
        
        f_printf(&file, "\n׷��һ��, ��ǰ�ļ���С��%lu\n �豸�ܿռ䣺%10lu KB, ���ÿռ� %10lu KB\n", f_size(&file), totalSect*4, freeSect*4);
        
        // ��λ����ʼλ��
        fRes = f_lseek(&file, 0);
        if(fRes!=FR_OK) return fRes;
        
        // ��ȡ����
        //fRes = f_read(&file, rBuf, f_size(&file), &num);
        //if(fRes!=FR_OK) return fRes;
        //printf("�ļ�����Ϊ��%s\r\n", rBuf);
        
        f_close(&file);
    }
    
    
    
    return FR_OK;
}