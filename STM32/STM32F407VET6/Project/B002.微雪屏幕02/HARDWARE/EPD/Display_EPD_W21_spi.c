#include "Display_EPD_W21_spi.h"


void EPD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE, ENABLE);
	  				     	
	
	 //CS-->PD8   SCK-->PD9  SDO--->PD10 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  	
		
	 // D/C--->PE15	   RES-->PE14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				     		
	
	// BUSY--->PE13
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//Pull up input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				//Initialize GPIO
	
	 //LED 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    */
    
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE);
	  				     	
	
     // 引脚为输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    // 输出模式为推挽
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    // 上拉模式 ？todo 不确定推挽输出时这里应该是上/下/浮空
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    // 引脚速率 2M
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
	 //CS-->E7   SCK-->Pb10  SDO--->Pc3 
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		//Port configuration
	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//Port configuration
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//Port configuration
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;		//Port configuration
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  	
		
	 // D/C--->PE8	   RES-->PC2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		//Port configuration
	GPIO_Init(GPIOE, &GPIO_InitStructure);	  				     		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//Port configuration
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
    // BUSY--->PE9
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;
    // 引脚为输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    // 上拉模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    // todo 模式为推挽
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_PP;
    
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				//Initialize GPIO
	
	
}



void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
        if(value & 0x80)
        	EPD_W21_MOSI_1;
        else
        	EPD_W21_MOSI_0;		
        value = (value << 1); 
        EPD_W21_CLK_1; 
    }
}

void Epaper_Write_Command(unsigned char command)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
void Epaper_Write_Data(unsigned char data)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// data write
	SPI_Write(data);
	EPD_W21_CS_1;
}



/***********************************************************
						end file
***********************************************************/
