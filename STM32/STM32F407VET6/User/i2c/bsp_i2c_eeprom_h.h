#ifndef BSP_IIC_HARDWARE_H
#define BSP_IIC_HARDWARE_H

#include "stm32f4xx.h"
#include <stdio.h>

// I2C���ʣ�����ģʽ400k
#define I2C_Speed       400000

// STM32��I2C��ַ���Զ���
#define I2C_OWN_ADDR    0x1A

/* EEPROM I2C �ӿ� */
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

/* ��ʱʱ�� */
#define I2CT_FLAG_TIMEOUT ((u32)0x1000)
#define I2CT_LONG_TIMEOUT ((u32)(10*I2C_FLAG_TIMEOUT))



// I2C gpio ���� 
static void I2C_GPIO_Cfg(void);

// I2C ����ģʽ����
static void I2C_Mode_Cfg(void);

// I2C �����ʼ��
void I2C_EE_INIT(void);




#endif
