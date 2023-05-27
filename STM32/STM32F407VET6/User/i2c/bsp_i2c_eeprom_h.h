#ifndef BSP_IIC_HARDWARE_H
#define BSP_IIC_HARDWARE_H

#include "stm32f4xx.h"
#include <stdio.h>

// I2C���ʣ�����ģʽ400k
#define I2C_Speed       400000

// STM32��I2C��ַ���Զ���
#define I2C_OWN_ADDR    0x5A

// EEPROM ��ַ
#define EEPROM_ADDR     0xA0

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

/* �Զ�������� u8 */
#define ErrCodeEv5          1 // ��ʼλ���ʹ���
#define ErrCodeEv6          2 // �豸��ַ���ʹ���
#define ErrCodeEv8_Addr     3 // д���ַ���ʹ���
#define ErrCodeEv8_Data     4 // ����д�����
#define ErrCodeBusy         5 // ����æ
#define ErrCodeRecv         6 // �������ݳ�ʱ




// I2C gpio ���� 
static void I2C_GPIO_Cfg(void);

// I2C ����ģʽ����
static void I2C_Mode_Cfg(void);

// I2C �����ʼ��
void I2C_EE_INIT(void);

// ����¼�
ErrorStatus CheckEvent(uint32_t eventState, uint32_t errorCode);

// �ȴ� EEPROM ��׼��״̬
ErrorStatus I2C_EE_WaitEepromStandbyState(void);



/**
 * @brief дһ���ֽڵ�EEPROM
 * 
 * @param writeAddr д��ַ
 * @param pBuf ������ָ��
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteByte(u16 writeAddr, u8 *pBuf);

/**
 * @brief д����ֽ�
 * 
 * @param pBuf ������ָ��
 * @param writeAddr д��ַ
 * @param len д����ֽ���
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_WriteBytes(u8 *pBuf, u16 writeAddr, u16 len);

// ��ҳ��д����ֽ��Լ�ͨ��ҳ����д����ֽ� ��


/**
 * @brief ��ȡ����
 * 
 * @param readAddr ��ȡ���ݵĵ�ַ
 * @param pBuf �������ݵĻ�����ָ��
 * @param len ��ȡ���ֽ���
 * @return ErrorStatus 
 */
ErrorStatus I2C_EE_ReadBytes(u8 readAddr, u8 *pBuf, u16 len);



#endif
