
#include "stm32f4xx.h"
#include "./systick/bsp_systick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./screen/wx_spi.h"

#include "./screen/EPD_Test.h"



// ϵͳ��ʱ���жϷ�������������Ŀ�У���Ӱ�칤�̵�stm32f4xx_it.c
void SysTick_Handler(void)
{
    if(TimingDelay != 0){ 
        TimingDelay--;
    }
}


int main(){
    // led ��ʼ��
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // ��ʼ��usart
    Usart1_Cfg(9600);
    printf("���ڳ�ʼ���ɹ�\n");
    
    // ���� SysTick
    if(SysTick_Init()==ERROR){
        LED1_ON; // ��ʱʧ��
        return 1;
    }


    // ��ʼ��FLASH
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
