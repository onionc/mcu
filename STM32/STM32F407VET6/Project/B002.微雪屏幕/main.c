
#include "stm32f4xx.h"
#include "./systick/bsp_systick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./screen/wx_spi.h"

#include "./screen/EPD_Test.h"



// 系统定时器中断服务函数定义在项目中，不影响工程的stm32f4xx_it.c
void SysTick_Handler(void)
{
    if(TimingDelay != 0){ 
        TimingDelay--;
    }
}


int main(){
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\n");
    
    // 配置 SysTick
    if(SysTick_Init()==ERROR){
        LED1_ON; // 定时失败
        return 1;
    }


    // 初始化FLASH
    SPI_FLASH_INIT();
    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    EPD_test();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			DEV_Delay_ms(500);

    }
    
    return 0;
}
