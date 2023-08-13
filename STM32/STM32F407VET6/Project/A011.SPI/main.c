
#include "stm32f4xx.h"

#include "./led/bsp_led.h"

#include "./usart/bsp_usart.h"
#include "./spi/bsp_spi.h"
#include "./spi/bsp_flash.h"

#define ADDR 0x0000
u8 data[100] = "大道至拙，重剑无锋";
u8 data2[100] = {0};

//简单的延时函数
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}



int main(){
    u32 id;
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\n");
    
    // 初始化FLASH，读取ID
    SPI_FLASH_INIT();
    Delay(200);
    id = SPI_FLASH_ReadId();
    printf("id=%x\n", id);

    // 擦除数据
    if(SPI_FLASH_SectorErase(ADDR)==ERROR){
        printf("数据擦除失败\n");
    }
    
    // 发送数据
    if(SPI_FLASH_PageWrite(data, ADDR, sizeof(data))==ERROR){
        printf("数据写入失败\n");
    }else{
        printf("数据写入成功，len=%d\n", sizeof(data));
    }
    
    // 读取数据
    if(SPI_FLASH_ReadData(data2, ADDR, 100)==ERROR){
        printf("数据写入失败\n");
    }else{
        printf("数据读取成功，data=%s\n", data2);
    }
        
    
    while(1){
        
        
    }
    
    return 0;
}
