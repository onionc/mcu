#include "./key/bsp_key.h"

// 按键GPIO配置
void Key_GPIO_Config(){
    GPIO_InitTypeDef gpioInitStruct;
    
    // 开启按键GPIO的时钟
    RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK, ENABLE);
    
    
    // 引脚为输入模式
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN;
    // 不上拉不下拉，浮空
    gpioInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    // 设置引脚并初始化按键1
    gpioInitStruct.GPIO_Pin = KEY1_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    // 初始化按键2
    gpioInitStruct.GPIO_Pin = KEY2_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    // 初始化按键3
    gpioInitStruct.GPIO_Pin = KEY3_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &gpioInitStruct);
    
    
}



/**
  * @brief 检测是否有按键按下
  * @param gpiox 具体的GPIO
  * @param pin 具体的哪一位（0~15）
  * @return 按键状态 KEY_ON 按下，KEY_OFF 未按下
*/
uint8_t Key_Scan(GPIO_TypeDef* gpiox, uint16_t pin){
    // 检测按键是否按下
    if(GPIO_ReadInputDataBit(gpiox, pin) == KEY_ON){
        // 消抖，如果需要消抖，需要增加delay。这里有电容，可直接读取不用消抖
        
        // 等待按键释放
        while(GPIO_ReadInputDataBit(gpiox, pin) == KEY_ON);
        return KEY_ON;
    }
    return KEY_OFF;
}
