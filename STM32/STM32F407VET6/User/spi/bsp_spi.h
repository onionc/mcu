#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f4xx.h"

// SPI�ź�ʱ�ӳ�ʼ������
#define FLASH_SPI           SPI2
#define FLASH_SPI_CLK       RCC_APB1Periph_SPI2
#define FLASH_SPI_CLK_INIT  RCC_APB1PeriphClockCmd

// sck ����
#define FLASH_SPI_SCK_PIN       GPIO_Pin_10
#define FLASH_SPI_SCK_GPIO_PORT GPIOB
#define FLASH_SPI_SCK_GPIO_CLK  RCC_AHB1Periph_GPIOB
#define FLASH_SPI_SCK_PINSOURCE GPIO_PinSource10
#define FLASH_SPI_SCK_AF        GPIO_AF_SPI2

// miso ����
#define FLASH_SPI_MISO_PIN          GPIO_Pin_2
#define FLASH_SPI_MISO_GPIO_PORT    GPIOC
#define FLASH_SPI_MISO_GPIO_CLK     RCC_AHB1Periph_GPIOC
#define FLASH_SPI_MISO_PINSOURCE    GPIO_PinSource2
#define FLASH_SPI_MISO_AF           GPIO_AF_SPI2

// MOSI ����
#define FLASH_SPI_MOSI_PIN          GPIO_Pin_3
#define FLASH_SPI_MOSI_GPIO_PORT    GPIOC
#define FLASH_SPI_MOSI_GPIO_CLK     RCC_AHB1Periph_GPIOC
#define FLASH_SPI_MOSI_PINSOURCE    GPIO_PinSource3
#define FLASH_SPI_MOSI_AF           GPIO_AF_SPI2

// CS/NSS ����
#define FLASH_CS_PIN                GPIO_Pin_3
#define FLASH_CS_GPIO_PORT          GPIOE
#define FLASH_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE




// ����CS��������͵�ƽ���������Ų���
#define SPI_FLASH_CS_LOW()          FLASH_CS_GPIO_PORT->BSRRH=FLASH_CS_PIN;
#define SPI_FLASH_CS_HIGH()         FLASH_CS_GPIO_PORT->BSRRL=FLASH_CS_PIN;

/* ��ʱʱ�� */
#define SPIT_FLAG_TIMEOUT ((u32)0x1000)

/* ���� */
// ����¼�
ErrorStatus SPI_CheckEvent(uint32_t eventState, char *errorMsg);
// SPI FLASH ��ʼ��
void SPI_FLASH_INIT();
// ʹ��SPI����һ���ֽ�����
u8 SPI_FLASH_sendByte(u8 byte);
// ʹ��SPI��ȡһ���ֽ�����
u8 SPI_FLASH_readByte();


#endif