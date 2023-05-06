// led的板级支持包 board support packet

#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"




/* led 引脚宏定义 */
// led1
#define LED1_PIN        GPIO_Pin_13
#define LED1_GPIO_PORT  GPIOE
#define LED1_GPIO_CLK   RCC_AHB1Periph_GPIOE

// led2
#define LED2_PIN        GPIO_Pin_14
#define LED2_GPIO_PORT  GPIOE
#define LED2_GPIO_CLK   RCC_AHB1Periph_GPIOE

// led3
#define LED3_PIN        GPIO_Pin_15
#define LED3_GPIO_PORT  GPIOE
#define LED3_GPIO_CLK   RCC_AHB1Periph_GPIOE

/* 控制led 宏定义 */

// led 低电平亮 on=0 off=1
// led 低电平亮 on=1 off=0
#define ON 0
#define OFF 1

// 使用库函数控制 led
#define LED1(state) if(state)\
    GPIO_SetBits(LED1_GPIO_PORT, LED1_PIN);\
    else\
    GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN)

#define LED2(state) if(state)\
    GPIO_SetBits(LED2_GPIO_PORT, LED2_PIN);\
    else\
    GPIO_ResetBits(LED2_GPIO_PORT, LED2_PIN)

#define LED3(state) if(state)\
    GPIO_SetBits(LED3_GPIO_PORT, LED3_PIN);\
    else\
    GPIO_ResetBits(LED3_GPIO_PORT, LED3_PIN)


// 操作寄存器控制 led
#define digitalH(p, i) {p->BSRRL=i;} // 设置为高电平
#define digitalL(p, i) {p->BSRRH=i;} // 设置为低电平
#define digitalToggle(p, i) {p->ODR^=i;} // 反转
// 寄存器操作的封装
#if (ON==0)
    #define LED1_ON digitalL(LED1_GPIO_PORT, LED1_PIN)
    #define LED2_ON digitalL(LED1_GPIO_PORT, LED2_PIN)
    #define LED3_ON digitalL(LED1_GPIO_PORT, LED3_PIN)

    #define LED1_OFF digitalH(LED1_GPIO_PORT, LED1_PIN)
    #define LED2_OFF digitalH(LED2_GPIO_PORT, LED2_PIN)
    #define LED3_OFF digitalH(LED3_GPIO_PORT, LED3_PIN)
#else
    #define LED1_ON digitalH(LED1_GPIO_PORT, LED1_PIN)
    #define LED2_ON digitalH(LED1_GPIO_PORT, LED1_PIN)
    #define LED3_ON digitalH(LED1_GPIO_PORT, LED1_PIN)

    #define LED1_OFF digitalL(LED1_GPIO_PORT, LED1_PIN)
    #define LED2_OFF digitalL(LED2_GPIO_PORT, LED2_PIN)
    #define LED3_OFF digitalL(LED3_GPIO_PORT, LED3_PIN)
#endif

#define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT, LED1_PIN)
#define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT, LED2_PIN)
#define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT, LED3_PIN)


void LED_GPIO_Config(void);

#endif

