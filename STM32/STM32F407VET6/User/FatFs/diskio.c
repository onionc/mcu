/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "../spi/bsp_flash.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


// 为每个设备定义一个物理编号
#define ATA         0 // 预留SD卡
#define SPI_FLASH   1 // 外部SPI_FLASH

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT; // 默认：不初始化
	int result;

	switch (pdrv) {
        case ATA: // SD 卡
            break;
        case SPI_FLASH:
            // SPI Flash 状态检测
            if(FlashId == SPI_FLASH_ReadId()){
                // 设备ID读取正确
                stat &= ~STA_NOINIT; // 
            }
            break;
        default:
            break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
    uint16_t i;
	int result;

	switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            // 初始化SPI flash
            SPI_FLASH_INIT();
            i=500; while(i--);
            // 唤醒 Flash
            SPI_FLASH_WAKEUP();
            // 获取状态
            stat = disk_status(SPI_FLASH);
        
            break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;


	switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            // 扇区偏移6MB（做其他实验用），就只剩下2MB可用了
            // 1扇区=4kb, 6*1024/4 = 1536
            sector+=1536;
            SPI_FLASH_ReadData(buff, sector<<12, count<<12); // <<12 为乘4096，从扇区化为字节
            res = RES_OK;
            break;
	}

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	uint32_t write_addr;

    if(count<1){
        return res;
    }
    
	switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            sector+=1536; // 偏移6MB
            SPI_FLASH_SectorErase(sector<<12);
            if(SPI_FLASH_PageWrite((u8*)buff, sector<<12, count<<12)==SUCCESS){
                res = RES_OK;
            }else{
                res = RES_PARERR;
            }
            
            break;
    }

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	int result;

    switch (pdrv) {
        case ATA:
            break;
        case SPI_FLASH:
            switch(cmd){
                case GET_SECTOR_COUNT:
                    // 扇区数量 2MB=2*1024/4=512 个扇区
                    *(WORD *)buff = 512;
                    break;
                case GET_SECTOR_SIZE:
                    // 扇区大小
                    *(WORD *)buff = 4096;
                    break;
                case GET_BLOCK_SIZE:
                    // 同时擦除的扇区个数
                    *(WORD *)buff = 1;
                    break;
            }
            res = RES_OK;
            break;
            
    }

	return res;
}

__weak DWORD get_fattime(void) {
    /* 返回当前时间戳 */
    return	  ((DWORD)(2023 - 1980) << 25)  /* Year 2015 */
            | ((DWORD)1 << 21)              /* Month 1 */
            | ((DWORD)1 << 16)              /* Mday 1 */
            | ((DWORD)0 << 11)              /* Hour 0 */
            | ((DWORD)0 << 5)               /* Min 0 */
            | ((DWORD)0 >> 1);              /* Sec 0 */
}


