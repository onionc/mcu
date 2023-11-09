/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "../spi/bsp_flash.h"
#include "../sdio/bsp_sdio_sd.h"
#include <string.h>
#include "./usart/bsp_usart.h"
/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


// Ϊÿ���豸����һ��������
#define ATA         0 // Ԥ��SD��
#define SPI_FLASH   1 // �ⲿSPI_FLASH

// SD�� ���С
#define SD_BLOCK_SIZE   512

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT; // Ĭ�ϣ�����ʼ��
	int result;
    u32 a;

	switch (pdrv) {
        case ATA: // SD ��
            stat &= ~STA_NOINIT;
            break;
        case SPI_FLASH:
            // SPI Flash ״̬���
            if(FlashId == SPI_FLASH_ReadId()){
                // �豸ID��ȡ��ȷ
                stat &= ~STA_NOINIT; // 
            }
            break;
        default:
            stat = STA_NOINIT; // �豸δ��ʼ��
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
            if(SD_Init()==SD_OK){
                stat = disk_status(ATA);
            }
            break;
        case SPI_FLASH:
            // ��ʼ��SPI flash
            SPI_FLASH_INIT();
            i=500; while(i--);
            // ���� Flash
            SPI_FLASH_WAKEUP();
            // ��ȡ״̬
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
    SD_Error sdState = SD_OK;


	switch (pdrv) {
        case ATA:
            if((DWORD)buff&3){ // ��ַû��4�ֽڶ���
                DRESULT resT = RES_OK;
                DWORD temp[SD_BLOCK_SIZE/4];
                while(count--){
                    resT = disk_read(ATA, (void*)temp, sector++, 1);
                    if(resT!=RES_OK){
                        break;
                    }
                    memcpy(buff, temp, SD_BLOCK_SIZE);
                    buff+=SD_BLOCK_SIZE;
                }
                return resT;
            }
            // ��ַ�Ѿ���4�ֽڶ��룬��ȡ�����
            sdState = SD_ReadMultiBlocks(buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);
            if(sdState == SD_OK){
                sdState = SD_WaitReadOperation();
                while(SD_GetStatus()!=SD_TRANSFER_OK);
            }
            if(sdState == SD_OK){
                res = RES_OK;
            }
            break;
        case SPI_FLASH:
            // ����ƫ��4MB��������ʵ���ã�����ֻʣ��4MB������
            // 1����=4kb, 4*1024/4 = 1024
            sector+=1024;
            SPI_FLASH_ReadData(buff, sector<<12, count<<12); // <<12 Ϊ��4096����������Ϊ�ֽ�
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
    SD_Error sdState = SD_OK;

    if(count<1){
        return res;
    }
    
	switch (pdrv) {
        case ATA:
            if((DWORD)buff&3){ // ��ַû��4�ֽڶ���
                DRESULT resT = RES_OK;
                DWORD temp[SD_BLOCK_SIZE/4];
                while(count--){
                    memcpy(temp, buff, SD_BLOCK_SIZE);
                    resT = disk_write(ATA, (void*)temp, sector++, 1);
                    if(resT!=RES_OK){
                        break;
                    }
                    buff+=SD_BLOCK_SIZE;
                }
                return resT;
            }
            // ��ַ�Ѿ���4�ֽڶ��룬��ȡ�����
            sdState = SD_WriteMultiBlocks((uint8_t*)buff, sector*SD_BLOCK_SIZE, SD_BLOCK_SIZE, count);
            if(sdState == SD_OK){
                sdState = SD_WaitWriteOperation();
                while(SD_GetStatus()!=SD_TRANSFER_OK);
            }
            if(sdState == SD_OK){
                res = RES_OK;
            }
            
            break;
        case SPI_FLASH:
            sector+=1024; // ƫ��4MB
            SPI_FLASH_SectorErase(sector<<12);
            if(SPI_FLASH_BufWrite((u8*)buff, sector<<12, count<<12)==SUCCESS){
                res = RES_OK;
            }else{
                res = RES_PARERR;
            }
            
            break;
    }

	return res;
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
    DWORD a = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;

    switch (pdrv) {
        case ATA:
             switch(cmd){
                case GET_SECTOR_COUNT:
                    // ��������
                    
                    *(DWORD *)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;

                    break;
                case GET_SECTOR_SIZE:
                    // ������С
                    *(DWORD *)buff = SD_BLOCK_SIZE;
                    break;
                case GET_BLOCK_SIZE:
                    // ͬʱ��������������
                    *(DWORD *)buff = 1;//SDCardInfo.CardBlockSize;
                    break;
            }
            res = RES_OK;
            break;
        case SPI_FLASH:
            switch(cmd){
                case GET_SECTOR_COUNT:
                    // �������� 4MB=4*1024/4=1024 ������
                    *(DWORD *)buff = 1024;
                    break;
                case GET_SECTOR_SIZE:
                    // ������С
                    *(DWORD *)buff = 4096;
                    break;
                case GET_BLOCK_SIZE:
                    // ͬʱ��������������
                    *(DWORD *)buff = 1;
                    break;
            }
            res = RES_OK;
            break;
            
    }

	return res;
}

__weak DWORD get_fattime(void) {
    /* ���ص�ǰʱ��� */
    return	  ((DWORD)(2023 - 1980) << 25)  /* Year 2015 */
            | ((DWORD)1 << 21)              /* Month 1 */
            | ((DWORD)1 << 16)              /* Mday 1 */
            | ((DWORD)0 << 11)              /* Hour 0 */
            | ((DWORD)0 << 5)               /* Min 0 */
            | ((DWORD)0 >> 1);              /* Sec 0 */
}


