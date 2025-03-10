#ifndef __W25Q128_INS_H
#define __W25Q128_INS_H
 
#define W25Q128_WRITE_ENABLE						0x06//写指令使能指令
#define W25Q128_WRITE_DISABLE						0x04
#define W25Q128_READ_STATUS_REGISTER_1				0x05//读状态寄存器1指令
#define W25Q128_READ_STATUS_REGISTER_2				0x35
#define W25Q128_WRITE_STATUS_REGISTER				0x01
#define W25Q128_PAGE_PROGRAM						0x02
#define W25Q128_QUAD_PAGE_PROGRAM					0x32//读读页编程地址
#define W25Q128_BLOCK_ERASE_64KB					0xD8
#define W25Q128_BLOCK_ERASE_32KB					0x52
#define W25Q128_SECTOR_ERASE_4KB					0x20//擦除扇形指令
#define W25Q128_CHIP_ERASE							0xC7
#define W25Q128_ERASE_SUSPEND						0x75
#define W25Q128_ERASE_RESUME						0x7A
#define W25Q128_POWER_DOWN							0xB9
#define W25Q128_HIGH_PERFORMANCE_MODE				0xA3
#define W25Q128_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Q128_RELEASE_POWER_DOWN_HPM_DEVICE_ID	0xAB
#define W25Q128_MANUFACTURER_DEVICE_ID				0x90
#define W25Q128_READ_UNIQUE_ID						0x4B
#define W25Q128_JEDEC_ID							0x9F//读ID指令
#define W25Q128_READ_DATA							0x03//读数据指令
#define W25Q128_FAST_READ							0x0B
#define W25Q128_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Q128_FAST_READ_DUAL_IO					0xBB
#define W25Q128_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Q128_FAST_READ_QUAD_IO					0xEB
#define W25Q128_OCTAL_WORD_READ_QUAD_IO				0xE3
 
#define W25Q128_DUMMY_BYTE							0xFF//垃圾地址，用来换数据指令
 
 
#define W25Q128_FLASH_ID                            0xEF4018 // flash id=厂商,存储器类型和容量
#endif