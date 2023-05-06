#ifndef __BSP_CLKCFG_H
#define __BSP_CLKCFG_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

// 使用HSE设置系统时钟
uint8_t HSE_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q);
// 使用HSI设置系统时钟
uint8_t HSI_SetSysClk(uint32_t m, uint32_t n, uint32_t p, uint32_t q);
#endif