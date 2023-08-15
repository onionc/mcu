
#include "stm32f4xx.h"
#include "./systick/bsp_systick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"

//EPD
#include "./HARDWARE/EPD/Display_EPD_W21_spi.h"
#include "./HARDWARE/EPD/Display_EPD_W21.h"
#include "./HARDWARE/EPD/Ap_29demo.h"	



// 系统定时器中断服务函数定义在项目中，不影响工程的stm32f4xx_it.c
void SysTick_Handler(void)
{
    if(TimingDelay != 0){ 
        TimingDelay--;
    }
}

//延时s
void delay_s(u16 s)
{
	u16 i;
	for(i=0;i<s;i++)
	Delay_10us(100*1000);
}

int	main(void)
{
    unsigned char i;
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 初始化usart
    Usart1_Cfg(9600);
    printf("串口初始化成功\n");
     // 配置 SysTick, Delay function initialization
    if(SysTick_Init()==ERROR){
        LED1_ON; // 定时失败
        return 1;
    }
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级	//Set NVIC interrupt grouping 2
    EPD_GPIO_Init();       //EPD GPIO  initialization
	
    printf("1");
    while(1)
	{
#if 1 //Full screen refresh, fast refresh, and partial refresh demostration.
			EPD_HW_Init(); //Full screen refresh initialization.
			EPD_WhiteScreen_White(); //Clear screen function.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
      printf("2");delay_s(2); //Delay for 2s.	
		 /************Full display(2s)*******************/
			EPD_HW_Init(); //Full screen refresh initialization.
			EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen refresh.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			printf("3");delay_s(2); //Delay for 2s.	
						
	#if 1 //Partial refresh demostration.
	//Partial refresh demo support displaying a clock at 5 locations with 00:00.  If you need to perform partial refresh more than 5 locations, please use the feature of using partial refresh at the full screen demo.
	//After 5 partial refreshes, implement a full screen refresh to clear the ghosting caused by partial refreshes.
	//////////////////////Partial refresh time demo/////////////////////////////////////
	    EPD_HW_Init(); //Electronic paper initialization	
	    EPD_SetRAMValue_BaseMap(gImage_basemap); //Partial refresh background color
printf("3");  		
      for(i=0;i<6;i++)
			EPD_Dis_Part_Time(  696+32*1,272,(unsigned char *)&Num[1],            //x-A,y-A,DATA-A
													696+32*2,272,(unsigned char *)&Num[0],         //x-B,y-B,DATA-B
													696+32*3,272,(unsigned char *)gImage_numdot,      //x-C,y-C,DATA-C
													696+32*4,272,(unsigned char *)&Num[0],        //x-D,y-D,DATA-D
													696+32*5,272,(unsigned char *)&Num[i],32,64);	//x-E,y-E,DATA-E,Resolution 32*64
		printf("4");
			EPD_DeepSleep();  //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay_s(2);	//Delay for 2s.
			EPD_HW_Init(); //Full screen refresh initialization.
			EPD_WhiteScreen_White(); //Clear screen function.
			EPD_DeepSleep(); //Enter the sleep mode and please do not delete it, otherwise it will reduce the lifespan of the screen.
			delay_s(2); //Delay for 2s.
	#endif
#endif
                                                    printf("5");
		while(1);	// The program stops here
	}
}	

