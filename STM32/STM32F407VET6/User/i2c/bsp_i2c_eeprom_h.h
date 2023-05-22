#ifndef BSP_IIC_HARDWARE_H
#define BSP_IIC_HARDWARE_H

#include "stm32f4xx.h"
#include <stdio.h>

// I2C速率：快速模式400k
#define I2C_Speed       400000

// STM32的I2C地址，自定义
#define I2C_OWN_ADDR    0x1A

/* EEPROM I2C 接口 */
#define EEPROM_I2C                  I2C1
#define EEPROM_I2C_CLK              RCC_APB1Periph_I2C1

#define EEPROM_I2C_SCL_PIN          GPIO_Pin_8
#define EEPROM_I2C_SCL_GPIO_PORT    GPIOB
#define EEPROM_I2C_SCL_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_SOURCE       GPIO_PinSource8
#define EEPROM_I2C_SCL_AF           GPIO_AF_I2C1

#define EEPROM_I2C_SDA_PIN          GPIO_Pin_9
#define EEPROM_I2C_SDA_GPIO_PORT    GPIOB
#define EEPROM_I2C_SDA_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_SOURCE       GPIO_PinSource9
#define EEPROM_I2C_SDA_AF           GPIO_AF_I2C1

/* 超时时间 */
#define I2CT_FLAG_TIMEOUT ((u32)0x1000)
#define I2CT_LONG_TIMEOUT ((u32)(10*I2C_FLAG_TIMEOUT))



// I2C gpio 配置 
static void I2C_GPIO_Cfg(void);

// I2C 工作模式配置
static void I2C_Mode_Cfg(void);

// I2C 外设初始化
void I2C_EE_INIT(void);




#endif
