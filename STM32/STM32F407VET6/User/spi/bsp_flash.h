#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

#include <stdbool.h>
#include "./spi/bsp_spi.h"
// flash 配置

#define FlashId                         0xEF4016  // W25Q64

/**
 * 说明
 * 1页=256 Byte
 * 1扇区=16*256=4KB
 * 1块 = 16*4 = 64KB
 * W25Q64是128块=8MB(128*64/1024=8MB)；W25Q128=16MB。
 */
#define FLASH_PER_PAGE_BYTES 256  // 每一页的字节
#define W25X_FLASH_PER_PAGE_BYTES_MAX FLASH_PER_PAGE_BYTES

 
/*FLASH常用命令*/
#define W25X_WriteEnable                0x06
#define W25X_WriteDisable               0x04
#define W25X_ReadStatusReg              0x05
#define W25X_WriteStatusReg             0x01
#define W25X_ReadData                   0x03
#define W25X_FastReadData               0x0B
#define W25X_FastReadDual               0x3B
#define W25X_PageProgram                0x02
#define W25X_BlockErase                 0xD8    // 块擦除 64KB
#define W25X_SectorErase                0x20    // 扇区擦除 4KB
#define W25X_ChipErase                  0xC7    // 整片擦除
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown           0xAB
#define W25X_DeviceID                   0xAB
#define W25X_ManufactDeviceID           0x90
#define W25X_JedecDeviceID              0x9F
/*其它*/
#define Dummy_Byte                      0xFF


// 读取 FLASH ID
u32 SPI_FLASH_ReadId(void);

// FLASH 写使能/失能
void SPI_FLASH_WriteEnable(bool enable);

/**
 * 等待FLASH不忙碌（ BUSY 标志被置0）
 * @param times 超时时间次数，基数是spi超时时间。一般都是1，擦除时耗时多传入其他参数
 * @return Status
 */
ErrorStatus SPI_FLASH_WaitForWriteEnd(u8 times);

// 按扇区擦除FLASH
ErrorStatus SPI_FLASH_SectorErase(u32 sectorAddr);

// 按页写入数据
ErrorStatus SPI_FLASH_PageWrite(u8 *pBuf, u32 writeAddr, u16 writeLen);

// 不定量写入
ErrorStatus SPI_FLASH_BufWrite(u8 *pBuf, u32 writeAddr, u16 writeLen);

// 读取 FLASH 数据
ErrorStatus SPI_FLASH_ReadData(u8 *pBuf, u32 readAddr, u16 readLen);

// 唤醒 FLASH
void SPI_FLASH_WAKEUP();

#endif