
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"

#include "./ff.h"

// ��������
FATFS fs;           //�ļ�ϵͳ����
FIL file;           // �ļ�����
FRESULT fRes;      // �ļ��������
BYTE rBuf[FF_MAX_SS]={0};
BYTE wBuf[] = "";
const char *physicalNum = "1:";



int main(){
    u32 id;
    MKFS_PARM parm;
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
    if(fRes == FR_NO_FILESYSTEM){
        printf("û���ļ�ϵͳ���������и�ʽ������\r\n");
        
        // ��ʽ��
        parm.fmt = FM_FAT | FM_SFD;
        parm.n_fat = 0; // �߼���������
        parm.align = 512; // ������С
        parm.n_root = 0;
        parm.au_size = 0;
        fRes = f_mkfs(physicalNum, &parm, rBuf, sizeof rBuf);
        
        if(fRes==FR_OK){
            printf("�Ѹ�ʽ���ļ�ϵͳ\r\n");
            // ���¹���
            fRes = f_mount(NULL, physicalNum, 1);
            fRes = f_mount(&fs, physicalNum, 1);
        }
        
    }
    
    if(fRes==FR_OK){
        printf("�ļ�ϵͳ���سɹ�");
    }else{
        printf("�ļ�ϵͳ����ʧ�� code=%d", fRes);
        LED1_ON;
    }
   
    while(1){
        
        
    }
    
    return 0;
}
