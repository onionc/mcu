#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H

#include "stm32f4xx.h"
#include "bsp_sdio_sd.h"


#define BLOCK_SIZE      512     // 块大小，字节
#define NUM_OF_BLOCKS   100     // 块个数

// sd卡测试
void SD_TEST();


#endif