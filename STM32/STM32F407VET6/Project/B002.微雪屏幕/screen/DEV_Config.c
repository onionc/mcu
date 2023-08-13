/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V2.0
* | Date        :   2018-10-30
* | Info        :
# ******************************************************************************
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"
#include "./spi/bsp_spi.h"


void DEV_SPI_WriteByte(UBYTE value)
{
    // 开始通讯：CS低
    SPI_FLASH_CS_LOW();
  
    SPI_FLASH_sendByte(value);
   
    
    // 停止通讯：CS高
    SPI_FLASH_CS_HIGH();
}

int DEV_Module_Init(void)
{
    GPIO_ResetBits(EPD_DC_PIN);
    GPIO_ResetBits(EPD_CS_PIN);
    //GPIO_SetBits(EPD_PWR_PIN);
    GPIO_SetBits(EPD_RST_PIN);
		return 0;
}

void DEV_Module_Exit(void)
{
    GPIO_ResetBits(EPD_DC_PIN);
    GPIO_ResetBits(EPD_CS_PIN);

    //close 5V
    //GPIO_ResetBits(EPD_PWR_PIN);
    GPIO_ResetBits(EPD_RST_PIN);
}


void MX_GPIO_Init(void)
{

  /*
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(GPIOA, RST_Pin|DC_Pin|SPI_CS_Pin|PWR_Pin, GPIO_PIN_RESET);

  // Configure GPIO pins : PAPin PAPin PAPin PAPin
  GPIO_InitStruct.Pin = RST_Pin|DC_Pin|SPI_CS_Pin|PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configure GPIO pin : PtPin
  GPIO_InitStruct.Pin = BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_GPIO_Port, &GPIO_InitStruct);
  
  */
  
    GPIO_InitTypeDef GPIO_init;
  
    // 使能 GPIO 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOC, ENABLE);
    
    
    
    // 引脚配置
    GPIO_init.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_init.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_init.GPIO_OType = GPIO_OType_PP;
    GPIO_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_init.GPIO_Pin = RST_Pin;
    GPIO_Init(RST_GPIO_Port, &GPIO_init);
    GPIO_init.GPIO_Pin = DC_Pin;
    GPIO_Init(DC_GPIO_Port, &GPIO_init);
    //GPIO_init.GPIO_Pin = SPI_CS_Pin; // CS已经在SPI初始化中设置过了
    //GPIO_Init(SPI_CS_GPIO_Port, &GPIO_init);
    GPIO_init.GPIO_Pin = PWR_Pin;
    GPIO_Init(PWR_GPIO_Port, &GPIO_init);
    

    GPIO_init.GPIO_Pin = BUSY_Pin;
    GPIO_init.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(BUSY_GPIO_Port, &GPIO_init);

}

