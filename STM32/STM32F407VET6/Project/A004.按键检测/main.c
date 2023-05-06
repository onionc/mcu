
#include "stm32f4xx.h"
#include "led/bsp_led.h"
#include "key/bsp_key.h"

int main(){
    // led 初始化
    LED_GPIO_Config();
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    
    // 按键 初始化
    Key_GPIO_Config();
    
    while(1){
        if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN)==KEY_ON){
            LED1_TOGGLE;
        }
        if(Key_Scan(KEY2_GPIO_PORT, KEY2_PIN)==KEY_ON){
            LED2_TOGGLE;
        }
        if(Key_Scan(KEY3_GPIO_PORT, KEY3_PIN)==KEY_ON){
            LED3_TOGGLE;
        }
    
    }
    
    
    return 0;
}
