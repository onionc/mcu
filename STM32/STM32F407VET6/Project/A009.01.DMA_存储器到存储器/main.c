
#include "stm32f4xx.h"
#include "./led/bsp_led.h"

/* 宏定义。存储器到存储器需要用DMA2，对流和通道无要求，因为没用到其他外设*/
#define DMA_STREAM          DMA2_Stream0
#define DMA_CHANNEL         DMA_Channel_0
#define DMA_STREAM_CLOCK    RCC_AHB1Periph_DMA2
#define DMA_FLAG_TCIF       DMA_FLAG_TCIF0

#define BUFFER_SIZE         32
#define TIMEOUT_MAX         10000

/* 定义两个数组，源和目的存储器 */
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

// dma配置并传输数据
static ErrorStatus DMA_Cfg(void); 
// 比对函数
ErrorStatus cmp(const u32 *pBuf1, const u32 *pBuf2, u16 len); 

int main(){
    // 存储器中传输DMA数据，LED3亮代表DMA初始错误，LED2亮代表数据比对错误，LED1亮代表数据比对正确
    
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;

    // dma配置和传输数据
    if(DMA_Cfg()==ERROR){
        LED3_ON;
    }else{
        // 等待传输完成
        while(DMA_GetFlagStatus(DMA_STREAM, DMA_FLAG_TCIF) == DISABLE);
        
        // 判断数据是否一样
        if(cmp(aSrcBuf, aDstBuf, BUFFER_SIZE)==SUCCESS){
            LED1_ON;
        }else{
            LED2_ON;
        }
    }

    while(1){}
    
    return 0;
}

// dma配置并传输数据
static ErrorStatus DMA_Cfg(void){
    DMA_InitTypeDef dmaInit;
    u32 timeout = TIMEOUT_MAX;
    
    // 使能 DMA时钟
    RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
    
    // 复位DMA数据流
    DMA_DeInit(DMA_STREAM);
    
    // 等待复位完成
    while(DMA_GetCmdStatus(DMA_STREAM) != DISABLE);
    
    // 配置 dma
    dmaInit.DMA_Channel = DMA_CHANNEL; // DMA数据流通道
    dmaInit.DMA_PeripheralBaseAddr = (u32)aSrcBuf; // 源数据地址
    dmaInit.DMA_Memory0BaseAddr = (u32)aDstBuf; // 目标地址
    dmaInit.DMA_DIR = DMA_DIR_MemoryToMemory; // 模式：存储器到存储器
    dmaInit.DMA_BufferSize = (u32)BUFFER_SIZE; // 数据大小
    dmaInit.DMA_PeripheralInc = DMA_PeripheralInc_Enable; // 使能外设自动递增
    dmaInit.DMA_MemoryInc = DMA_MemoryInc_Enable; // 使能存储器自动递增
    dmaInit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 源数据宽度：字大小 32bit
    dmaInit.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // 目标数据宽度：字大小
    dmaInit.DMA_Mode = DMA_Mode_Normal; // 模式：一次传输
    dmaInit.DMA_Priority = DMA_Priority_High; // 数据流优先级：高
    dmaInit.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFO模式不使能，FIFOThreshold阈值无效
    dmaInit.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dmaInit.DMA_MemoryBurst = DMA_MemoryBurst_Single;  // 存储器突发模式：单次
    dmaInit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // 外设突发模式：单次
    // 初始化
    DMA_Init(DMA_STREAM, &dmaInit);
    
    // 清除DMA数据流传输完成标志
    DMA_ClearFlag(DMA_STREAM, DMA_FLAG_TCIF);
    
    // 使能数据流，开始DMA数据传输
    DMA_Cmd(DMA_STREAM, ENABLE);
    
    // 检测数据流是否有效
    while((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (timeout-- > 0)){}
    
    // 判断是否超时
    if(timeout==0){
        return ERROR;
    }
        
    return SUCCESS;
}
// 比对函数
ErrorStatus cmp(const u32 *pBuf1, const u32 *pBuf2, u16 len){
    while(len--){
        if(*pBuf1++ != *pBuf2++){
            return ERROR;
        }
        
    }
    return SUCCESS;
}
