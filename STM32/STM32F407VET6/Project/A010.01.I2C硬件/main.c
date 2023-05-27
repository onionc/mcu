
#include "stm32f4xx.h"

#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./i2c/bsp_i2c_eeprom_h.h"


//简单的延时函数
static void Delay(__IO u32 nCount)
{
    while(nCount--);
}

// i2c 读写测试
#define BUF_SIZE 256
#define FirstPage 0x00 // 起始地址
uint8_t wBuf[BUF_SIZE];
uint8_t rBuf[BUF_SIZE];
ErrorStatus I2C_Test(void){
    u16 i=0;
    printf("开始写入数据\n");
    
    // 准备数据
    for(i=0; i<BUF_SIZE; i++){
        wBuf[i] = i%9; // %3或%9是为了避免数据重复写入后 256一轮回，看不到差异。
        //printf("%02x", wBuf[i]);
    }
    printf("\n");
    // 写数据
    if(I2C_EE_WriteBytes(wBuf, FirstPage, BUF_SIZE)==ERROR){
        printf("写数据失败");
        return ERROR;
    }
    printf("写数据完成\n");
    //return SUCCESS;
    // 读数据
    if(I2C_EE_ReadBytes(FirstPage, rBuf, BUF_SIZE)==ERROR){
        printf("读数据失败\n");
        return ERROR;
    }
    
    printf("对比数据\n");
    
    for(i=0; i<BUF_SIZE; i++){
        //printf("%02x", rBuf[i]);
        if(wBuf[i]!=rBuf[i]){
            printf("error:%d 0x%02X", rBuf[i]);
            return ERROR;
        }
    }
    
    printf("测试完成\n");
    
    return SUCCESS;
}

int main(){

    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart_Cfg(9600);
    printf("串口初始化成功\n");

    
    // I2C 初始化
    I2C_EE_INIT();
    
    if(I2C_Test()==SUCCESS){
        LED1_ON;
    }else{
        LED2_ON;
    }

    while(1){
        LED3_TOGGLE;
        Delay(0xffffff);
    }
    
    return 0;
}
