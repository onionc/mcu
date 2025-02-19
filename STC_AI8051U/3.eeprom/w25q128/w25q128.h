#ifndef __W25Q128_H
#define __W25Q128_H

#include "spi.h"
#include "W25Q128_Ins.h"

void W25Q128_Init(void);
void W25Q128_ReadID(u8 *MID, u16 *DID);
void W25Q128_PageProgram(u32 Address, u8 *DataArray, u16 Count);
void W25Q128_SectorErase(u32 Address);
void W25Q128_ReadData(u32 Address, u8 *DataArray, u32 Count);
 
#endif