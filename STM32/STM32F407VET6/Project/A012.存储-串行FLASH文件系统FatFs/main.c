
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_flash.h"

#include "./ff.h"

// 变量定义
FATFS fs;           //文件系统对象
FIL file;           // 文件对象
FRESULT fRes;      // 文件操作结果
BYTE rBuf[FF_MAX_SS]={0};
BYTE wBuf[] = "";
const char *physicalNum = "1:";



int main(){
    u32 id;
    MKFS_PARM parm;
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF; // 1灯亮代表文件系统问题
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\r\n");
    
    // 在SPI FLASH挂载文件系统，会初始化设备
    fRes = f_mount(&fs, physicalNum, 1); // 文件对象，设备编号，立即重载
    
    /************* 格式化测试 **************/
    // 没有文件系统
    if(fRes == FR_NO_FILESYSTEM){
        printf("没有文件系统，即将进行格式化……\r\n");
        
        // 格式化
        parm.fmt = FM_FAT | FM_SFD;
        parm.n_fat = 0; // 逻辑驱动器号
        parm.align = 512; // 扇区大小
        parm.n_root = 0;
        parm.au_size = 0;
        fRes = f_mkfs(physicalNum, &parm, rBuf, sizeof rBuf);
        
        if(fRes==FR_OK){
            printf("已格式化文件系统\r\n");
            // 重新挂载
            fRes = f_mount(NULL, physicalNum, 1);
            fRes = f_mount(&fs, physicalNum, 1);
        }
        
    }
    
    if(fRes==FR_OK){
        printf("文件系统挂载成功");
    }else{
        printf("文件系统挂载失败 code=%d", fRes);
        LED1_ON;
    }
   
    while(1){
        
        
    }
    
    return 0;
}
