#ifndef _LCD1602_H
#define _LCD1602_H

#include <reg52.h>
#include "../common.h"

// 定义引脚
sbit LCD_RS = P2^6;
sbit LCD_RW = P2^5;
sbit LCD_E = P2^7;
#define LCD_DATA P0

// 方法
void init();
void clear();
void show(u8, u8, u8*);

#endif