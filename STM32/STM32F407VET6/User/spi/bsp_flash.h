#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

// flash 配置

/*FLASH常用命令*/
#define W25X_WriteEnable                0x06
#define W25X_WriteDisable               0x04
#define W25X_ReadStatusReg              0x05
#define W25X_WriteStatusReg             0x01
#define W25X_ReadData                   0x03
#define W25X_FastReadData               0x0B
#define W25X_FastReadDual               0x3B
#define W25X_PageProgram                0x02
#define W25X_BlockErase                 0xD8
#define W25X_SectorErase                0x20
#define W25X_ChipErase                  0xC7
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown           0xAB
#define W25X_DeviceID                   0xAB
#define W25X_ManufactDeviceID           0x90
#define W25X_JedecDeviceID              0x9F
/*其它*/
#define  sFLASH_ID                      0XEF4018
#define Dummy_Byte                      0xFF

// 读取 FLASH ID
u32 SPI_FLASH_ReadId(void);

#endif