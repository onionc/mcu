
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./ff.h"
#include <string.h>
#include "../sdio/bsp_sdio_sd.h"

// 变量定义
FATFS fs;           //文件系统对象
FIL file;           // 文件对象
FRESULT fRes;      // 文件操作结果

const char *physicalNum = "0:"; // 1是SPI FLASH, 0是SD卡
u32 num; // 字节数
BYTE xBuf[FF_MAX_SS]={0}; // 操作数据buffer


#define FORCE_FORMAT    0   // 强制格式化



FRESULT formatSpace(); // 格式化
FRESULT normalFuncTest(); // 常用方法测试
FRESULT fileInfo(); // 文件详情
FRESULT scanFiles(); // 扫描文件


#define CONTACT(y) ("0:" y) // 拼接到设备

// 中断处理函数
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
    

    
    // 配置NVIC优先级分组为1(1位主优先级，3位子优先级)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF; // 1灯亮代表文件系统问题
    LED2_OFF;
    LED3_OFF;
    
    
    // 初始化usart，usart2口是485，0代表接收模式
    Usart2_Cfg(921600, 0);
    Usart1_Cfg(9600);
    
    
    printf("串口初始化成功\r\n");
    

    // 挂载SD卡
    {
        // 在SPI FLASH挂载文件系统，会初始化设备
        fRes = f_mount(&fs, physicalNum, 1); // 文件对象，设备编号，立即重载
        
        /************* 格式化测试 **************/
        // 没有文件系统
        if(FORCE_FORMAT || fRes == FR_NO_FILESYSTEM){
            printf("没有文件系统\r\n");
            fRes=formatSpace();
        }
        
        if(fRes==FR_OK){
            printf("文件系统挂载成功\r\n");
        }else{
            printf("文件系统挂载失败 code=%d\r\n", fRes);
            LED1_ON;
        }
    }

    // 创建文件
    {
        fRes = f_open(&file, CONTACT("c.raw"), FA_CREATE_ALWAYS | FA_WRITE);
        if(fRes != FR_OK){
            //printf("打开文件失败\r\n");
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
            // 空闲时写数据
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
                // 关闭文件
                f_close(&file);
                // 取消挂载
                f_mount(NULL, physicalNum, 1);
            }
        }

    }
    
    // 关闭文件
    // f_close(&file);
    // 取消挂载
    // f_mount(NULL, physicalNum, 1);
    
    return 0;
}


FRESULT formatSpace(){
    MKFS_PARM parm;
    printf("即将进行格式化……\r\n");
        
    // 格式化
    
    parm.fmt = FM_FAT32 | FM_SFD; // 使用FAT32
    parm.n_fat = 0; // 逻辑驱动器号
    parm.align = 0; // 卷数据区域分配池
    parm.n_root = 0;
    parm.au_size = 4096; // 簇大小，要设置的簇大小,以字节为单位
    
    fRes = f_mkfs(physicalNum, &parm, xBuf, sizeof xBuf);
    
    if(fRes==FR_OK){
        printf("已格式化文件系统\r\n");
        // 重新挂载
        fRes = f_mount(NULL, physicalNum, 1);
        fRes = f_mount(&fs, physicalNum, 1);
    }
    return fRes;
}

FRESULT normalFuncTest(){
    DIR dir;
    FATFS *pFs;
    u32 freeClust, freeSect, totalSect;
    
    //printf("设备信息获取：\r\n");
    {
        // 获取设备信息和簇大小（等于1扇区）
        fRes = f_getfree(physicalNum, &freeClust, &pFs);
        if(fRes!=FR_OK) return fRes;
        // 计算总扇区个数和空扇区个数
        totalSect = (pFs->n_fatent - 2)* pFs->csize;
        freeSect = freeClust * pFs->csize;
        
        // 可用空间按扇区更新，4K变化才刷新一次。
        // 这里一个扇区是512Bytes, /2 则是KB
        //printf("设备总空间：%10lu KB, 可用空间 %10lu KB\r\n", totalSect/2, freeSect/2);
    }
    
    //printf("文件定位功能：\r\n");
    #if SEEK_TEST==1
    {
        fRes = f_open(&file, CONTACT("a.txt"), FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        if(fRes!=FR_OK) return fRes;

        // 文件定位到末尾
        //printf("file seek:%lu\r\n", f_size(&file));
        fRes = f_lseek(&file, f_size(&file));
        if(fRes!=FR_OK) return fRes;
        
        // 追加一行数据
        f_printf(&file, "\n追加一行, 当前文件大小：%lu\n 设备总空间：%10lu KB, 可用空间 %10lu KB\n", f_size(&file), totalSect/2, freeSect/2);
        
        // 定位到起始位置
        fRes = f_lseek(&file, 0);
        if(fRes!=FR_OK) return fRes;
        
        // 读取数据
        //fRes = f_read(&file, rBuf, f_size(&file), &num);
        //if(fRes!=FR_OK) return fRes;
        //printf("文件内容为：%s\r\n", rBuf);
        
        f_close(&file);
    }
    #endif
    
    
    #if RENAME_DEL_TEST==1
    {
        //printf("目录创建、文件删除和重命名：\r\n");
        // 目录创建
        fRes = f_opendir(&dir, CONTACT("doc"));
        if(fRes!=FR_OK){
            //printf("创建目录\r\n");
            fRes = f_mkdir(CONTACT("doc"));
            if(fRes!=FR_OK) return fRes;
        }else{
            //printf("目录已存在，关闭目录\r\n");
            fRes = f_closedir(&dir);
            if(fRes!=FR_OK) return fRes;
        }
        
        // 文件重命名
        //printf("文件重命名\r\n");
        fRes = f_rename(CONTACT("a.txt"), CONTACT("c.txt"));
        if(fRes!=FR_OK) return fRes;
        
        // 文件删除
        //printf("删除文件\r\n");
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
    
    //printf("1:a.txt文件信息：");
    //printf("文件大小：", info.fsize );
    
    return FR_OK;
}

FRESULT scanFiles(){

    return FR_OK;
}
