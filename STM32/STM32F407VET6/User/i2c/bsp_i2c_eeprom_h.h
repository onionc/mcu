#ifndef BSP_IIC_HARDWARE_H
#define BSP_IIC_HARDWARE_H

#include "stm32f4xx.h"
#include <stdio.h>

// I2C速率：快速模式400k
#define I2C_Speed       400000

// STM32的I2C地址，自定义
#define I2C_OWN_ADDR    0x5A

// EEPROM 地址
#define EEPROM_ADDR     0xA0

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

/* 自定义错误码 u8 */
#define ErrCodeEv5          1 // 起始位发送错误
#define ErrCodeEv6          2 // 设备地址发送错误
#define ErrCodeEv8_Addr     3 // 写入地址发送错误
#define ErrCodeEv8_Data     4 // 数据写入错误
#define ErrCodeBusy         5 // 总线忙
#define ErrCodeRecv         6 // 接收数据超时




// I2C gpio 配置 
static void I2C_GPIO_Cfg(void);

// I2C 工作模式配置
static void I2C_Mode_Cfg(void);

// I2C 外设初始化
void I2C_EE_INIT(void);

// 检测事件
ErrorStatus CheckEvent(uint32_t eventState, uint32_t errorCode);

// 等待 EEPROM 到准备状态
ErrorStatus I2C_EE_WaitEepromStandbyState(void);



/**
 * @brief 写一个字节到EEPROM
 * 
 * @param writeAddr 写地址
 * @param pBuf 缓冲区指针
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteByte(u16 writeAddr, u8 *pBuf);

/**
 * @brief 写多个字节
 * 
 * @param pBuf 缓冲区指针
 * @param writeAddr 写地址
 * @param len 写入的字节数
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteBytes(u8 *pBuf, u16 writeAddr, u16 len);

// 在页内写多个字节以及通过页快速写多个字节 略


/**
 * @brief 读取数据
 * 
 * @param readAddr 读取数据的地址
 * @param pBuf 接收数据的缓冲区指针
 * @param len 读取的字节数
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_ReadBytes(u8 readAddr, u8 *pBuf, u16 len);



#endif
