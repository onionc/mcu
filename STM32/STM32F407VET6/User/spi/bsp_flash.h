#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include <stdbool.h>
#include "./spi/bsp_spi.h"
// flash ����

#define FlashId                         0xEF4016  // W25Q64

/**
 * ˵��
 * 1ҳ=256 Byte
 * 1����=16*256=4KB
 * 1�� = 16*4 = 64KB
 * W25Q64��128��=8MB(128*64/1024=8MB)��W25Q128=16MB��
 */
#define FLASH_PER_PAGE_BYTES 256  // ÿһҳ���ֽ�
#define W25X_FLASH_PER_PAGE_BYTES_MAX FLASH_PER_PAGE_BYTES

 
/*FLASH��������*/
#define W25X_WriteEnable                0x06
#define W25X_WriteDisable               0x04
#define W25X_ReadStatusReg              0x05
#define W25X_WriteStatusReg             0x01
#define W25X_ReadData                   0x03
#define W25X_FastReadData               0x0B
#define W25X_FastReadDual               0x3B
#define W25X_PageProgram                0x02
#define W25X_BlockErase                 0xD8    // ����� 64KB
#define W25X_SectorErase                0x20    // �������� 4KB
#define W25X_ChipErase                  0xC7    // ��Ƭ����
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown           0xAB
#define W25X_DeviceID                   0xAB
#define W25X_ManufactDeviceID           0x90
#define W25X_JedecDeviceID              0x9F
/*����*/
#define Dummy_Byte                      0xFF


// ��ȡ FLASH ID
u32 SPI_FLASH_ReadId(void);

// FLASH дʹ��/ʧ��
void SPI_FLASH_WriteEnable(bool enable);

/**
 * �ȴ�FLASH��æµ�� BUSY ��־����0��
 * @param times ��ʱʱ�������������spi��ʱʱ�䡣һ�㶼��1������ʱ��ʱ�ഫ����������
 * @return Status
 */
ErrorStatus SPI_FLASH_WaitForWriteEnd(u8 times);

// ����������FLASH
ErrorStatus SPI_FLASH_SectorErase(u32 sectorAddr);

// ��ҳд������
ErrorStatus SPI_FLASH_PageWrite(u8 *pBuf, u32 writeAddr, u16 writeLen);

// ������д��
ErrorStatus SPI_FLASH_BufWrite(u8 *pBuf, u32 writeAddr, u16 writeLen);

// ��ȡ FLASH ����
ErrorStatus SPI_FLASH_ReadData(u8 *pBuf, u32 readAddr, u16 readLen);

// ���� FLASH
void SPI_FLASH_WAKEUP();

#endif