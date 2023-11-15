
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./ff.h"
#include "../sdio/bsp_sdio_sd.h"
#include <string.h>

#define FRAME_LEN   300  // 一帧长度
#define BUF_COUNT   10 // 创建缓存区个数，文件保存需要时间大概10~20ms，需要开几个缓冲区来存数据。建议大小为 2~10，可使用 20ms/一帧数据时间（如5ms）*2 
#define SAVE_BYTES 30000 // 等待多少字节保存一次，大于一个扇区的用f_write都会写入flash，所以这个大小可以很大，不必是FatFs buffer的大小，建议赋值为待采集系统写入1s的字节数（会闪LED）。会丢最后不足此字节数量的数据
/* DMA 宏定义 */
#define USART2_DR_BASE      (USART2_BASE+0x04)   // +0x04是USART的数据寄存器地址
#define USARTx_DMA_CLK      RCC_AHB1Periph_DMA1
#define USARTx_DMA_STREAM   DMA1_Stream5
#define USARTx_DMA_CHANNEL  DMA_Channel_4
#define RECV_BUF_SIZE       FRAME_LEN // DMA接收的数据量，需大于一帧数据（有时间来等待空闲）
#define TIMEOUT_MAX         10000
u8 RxBuf[RECV_BUF_SIZE] = {0};

// DMA配置函数，存储器到外设（USART2.DR)
ErrorStatus DMA_Cfg(void);
void DMA1_Stream5_IRQHandler(void);
    
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

// 缓冲区
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
    // 串口空闲中断
    if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET){
        // 读取DMA中的数据
        {
            DMA_Cmd(USARTx_DMA_STREAM, DISABLE);
            
            recvLen = RECV_BUF_SIZE - DMA_GetCurrDataCounter(USARTx_DMA_STREAM); // 通过剩余计数判断长度
            
            DMA_SetCurrDataCounter(USARTx_DMA_STREAM, RECV_BUF_SIZE); // 需要把计数设置回去
            if(recvLen>0){
                RxFlag = 1;

                // 赋值给缓冲区
                memcpy(bufT[bufIndex].buffer, RxBuf, recvLen);
                bufT[bufIndex].len = recvLen;
                
                bufIndex++;
                bufIndex%=BUF_COUNT;
            
            }
            
            DMA_ClearFlag(USARTx_DMA_STREAM, DMA_FLAG_TCIF5); // DMA传输完成中断清零
            DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
            
            
        }
        
        // 清除IDLE标志
        temp = USART2->SR;
        temp = USART2->DR;
    }
    
}


int main(){
    u32 id;
    u8 rxBuf[256]={0};

    
    // 配置NVIC优先级分组为1(1位主优先级，3位子优先级)
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF; // 1灯亮代表文件系统问题
    LED2_OFF;
    LED3_OFF;

    // 初始化usart，usart2口是485，0代表接收模式
    Usart2_Cfg(921600, 0);
    Usart1_Cfg(115200);
    
    printf("串口初始化成功\r\n");
    
    // dma配置
    {
        // 配置使用DMA
        if(DMA_Cfg()==ERROR){
            printf("DMA初始化失败\r\n");
        }else{
            printf("DMA初始化成功\r\n");
        }
        
    }
    

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
  
            
        while(useIndex!=bufIndex){
            // 写数据到文件，未保存
            rxCount+=bufT[useIndex].len;
            fRes = f_write(&file, bufT[useIndex].buffer, bufT[useIndex].len, &num);
            bufT[useIndex].len = 0;
            
            useIndex++;
            useIndex%=BUF_COUNT;
           
            if(fRes != FR_OK || num<1){
                LED2_ON;
            }
        }

        
        // 保存到文件。会丢最后不足设定
        if(rxCount>SAVE_BYTES){
            // 保存文件
            f_sync(&file);
            
            rxCount=0;
            
            LED3_TOGGLE;
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


// DMA配置函数，存储器到外设（USART1.DR)
ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    NVIC_InitTypeDef nvicInit;
    
    // 使能 DMA时钟
    RCC_AHB1PeriphClockCmd(USARTx_DMA_CLK, ENABLE);
    
    // 复位DMA数据流
    DMA_DeInit(USARTx_DMA_STREAM);
    
    // 等待复位完成
    while(DMA_GetCmdStatus(USARTx_DMA_STREAM) != DISABLE);
    
    // 配置 dma。usart2.Rx对应dma1,通道4，数据流5
    dmaInit.DMA_Channel = USARTx_DMA_CHANNEL; // DMA数据流通道
    dmaInit.DMA_PeripheralBaseAddr = (u32)USART2_DR_BASE; // 外设地址：UART1的数据寄存器地址
    dmaInit.DMA_Memory0BaseAddr = (u32)RxBuf; // 存储器地址：内存地址
    dmaInit.DMA_DIR = DMA_DIR_PeripheralToMemory; // 模式：外设到存储器
    dmaInit.DMA_BufferSize = (u32)RECV_BUF_SIZE; // 数据大小
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 使能外设地址不自动递增
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // 使能存储器自动递增
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 源数据宽度：字节大小 8bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 目标数据宽度：字节大小
    dmaInit.DMA_Mode = DMA_Mode_Normal; // 模式：循环
    dmaInit.DMA_Priority = DMA_Priority_High; // 数据流优先级：中
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFO模式不使能，FIFOThreshold阈值无效
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // 存储器突发模式：单次
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // 外设突发模式：单次
    DMA_Init(USARTx_DMA_STREAM, &dmaInit); // 配置DMA，上面为单缓冲
    
    // 使能DMA数据流
    DMA_Cmd(USARTx_DMA_STREAM, ENABLE);
    
    // USART1 向 DMA发出RX请求
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    
    // 双缓冲开启
    // DMA_DoubleBufferModeConfig(USARTx_DMA_STREAM, (u32)RxBuf2, DMA_Memory_0);
    // DMA_DoubleBufferModeCmd(USARTx_DMA_STREAM, ENABLE);
    
    
    // 使能DMA中断
    /*
    DMA_ITConfig(USARTx_DMA_STREAM, DMA_IT_TC, ENABLE); // 传输完成和错误中断
    DMA_ClearITPendingBit(USARTx_DMA_STREAM, DMA_IT_TCIF5);
    
    // 配置DMA中断向量和优先级
    nvicInit.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
    nvicInit.NVIC_IRQChannelSubPriority = 0;
    nvicInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicInit);
    */
    
    
    
    
    
    // 检测数据流是否有效
    while((DMA_GetCmdStatus(USARTx_DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // 判断是否超时
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
}
