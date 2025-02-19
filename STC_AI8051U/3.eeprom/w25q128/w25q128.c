#include "w25q128.h"
#include "spi.h"

 
//W25Q128函数封装
void W25Q128_Init(void)
{
    SPI_Init();
}
 
//获取ID数据函数
void W25Q128_ReadID(u8 *MID,u16 *DID)
{
	SPI_Start ();//开始条件
	SPI_SwapByte(W25Q128_JEDEC_ID);//指令码
	*MID = SPI_SwapByte(W25Q128_DUMMY_BYTE);//交换数据
	*DID = SPI_SwapByte(W25Q128_DUMMY_BYTE);//交换数据
	*DID <<= 8;//高8位右移
	*DID |= SPI_SwapByte(W25Q128_DUMMY_BYTE);//交换高8位+低8位指定地址数据
	SPI_Stop ();//终止条件
}

 
//写使能函数
void W25Q128_WriteEnable(void)
{
	SPI_Start ();//开始
	SPI_SwapByte(W25Q128_WRITE_ENABLE);//指定使能地址
	SPI_Stop ();//结束
}
 
//等待忙函数
void W25Q128_WaitBusy(void)
{
	u32 Timeout;
	
	SPI_Start ();//开始
	SPI_SwapByte(W25Q128_READ_STATUS_REGISTER_1);//读状态寄存器1地址
	Timeout = 100000;
	while((SPI_SwapByte(W25Q128_DUMMY_BYTE) & 0x01) == 0x01)//等待垃圾地址数据读完
	{
		Timeout --;//自减
	if(Timeout == 0)//自减到0结束程序
	{
		break;
	}
    }
	SPI_Stop ();//结束
}
 
//擦除函数
void W25Q128_SectorErase(u32 Address)
{
	W25Q128_WriteEnable();
	
	SPI_Start ();//开始
	SPI_SwapByte(W25Q128_SECTOR_ERASE_4KB);//指定擦除指令
	SPI_SwapByte(Address >> 16);//前8位指定地址
	SPI_SwapByte(Address >> 8);//中8位指定地址
	SPI_SwapByte(Address);//后8位指定地址
	SPI_Stop ();//结束
	
	W25Q128_WaitBusy();
}
 
//页编程函数
void W25Q128_PageProgram(u32 Address,u8 *DataArray,u16 Count)//指定地址，指定数据，指定次数
{
	u32 i;
	
	W25Q128_WriteEnable();
	
	SPI_Start ();//开始
	SPI_SwapByte(W25Q128_PAGE_PROGRAM);//指定页编程指令
	SPI_SwapByte(Address >> 16);//前8位指定地址
	SPI_SwapByte(Address >> 8);//中8位指定地址
	SPI_SwapByte(Address);//后8位指定地址
	for(i = 0;i < Count;i++)
	{
		SPI_SwapByte(DataArray[i]);//写入指定次数指定数据
	}
	SPI_Stop ();//结束
	
	W25Q128_WaitBusy();
}
 
//指定读函数
void W25Q128_ReadData(u32 Address,u8 *DataArray,u32 Count)//指定地址，指定数据，指定次数
{
	
	u32 i;
	SPI_Start ();//开始
	SPI_SwapByte(W25Q128_READ_DATA);//指定读数据指令
	SPI_SwapByte(Address >> 16);//前8位指定地址
	SPI_SwapByte(Address >> 8);//中8位指定地址
	SPI_SwapByte(Address);//后8位指定地址
	for(i = 0;i < Count;i++)
	{
		DataArray[i] = SPI_SwapByte(W25Q128_DUMMY_BYTE);//写入指定次数指定数据
	}
	SPI_Stop ();//结束
	
}