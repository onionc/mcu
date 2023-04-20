#include "matrix.h"

#define KEY_MATRIX_PORT P1

/**
 * 使用行列式扫描4*4矩阵按键是否按下
 * @return 如果按下返回 1~16，没有按下返回0
 */
u8 key_matrix_ranks_scan(){
    u8 k = 0;

    // 第一列处理
    KEY_MATRIX_PORT = 0xF7; // 给第一列赋低电平0,，其余列（也包含其余行）为高电平
    if(KEY_MATRIX_PORT != 0xF7){ // 判断第一列是否有某行按键按下
        delay1ms(5); // 消抖
        switch(KEY_MATRIX_PORT){
            case 0x77: k=1; break;
            case 0xb7: k=5; break;
            case 0xd7: k=9; break;
            case 0xe7: k=13; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xF7); // 等待按键松开
   
    // 第二列处理
    KEY_MATRIX_PORT = 0xFB; // 给第二列赋低电平0,，其余列（也包含其余行）为高电平
    if(KEY_MATRIX_PORT != 0xFB){
        delay1ms(5); // 消抖
        switch(KEY_MATRIX_PORT){
            case 0x7b: k=2; break;
            case 0xbb: k=6; break;
            case 0xdb: k=10; break;
            case 0xeb: k=14; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFB); // 等待按键松开

    // 第三列处理
    KEY_MATRIX_PORT = 0xFD; // 给第二列赋低电平0,，其余列（也包含其余行）为高电平
    if(KEY_MATRIX_PORT != 0xFD){
        delay1ms(5); // 消抖
        switch(KEY_MATRIX_PORT){
            case 0x7d: k=3; break;
            case 0xbd: k=7; break;
            case 0xdd: k=11; break;
            case 0xed: k=15; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFD); // 等待按键松开

    // 第四列处理
    KEY_MATRIX_PORT = 0xFE; // 给第二列赋低电平0,，其余列（也包含其余行）为高电平
    if(KEY_MATRIX_PORT != 0xFE){
        delay1ms(5); // 消抖
        switch(KEY_MATRIX_PORT){
            case 0x7e: k=4; break;
            case 0xbe: k=8; break;
            case 0xde: k=12; break;
            case 0xee: k=16; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFE); // 等待按键松开
      
    return k;
}

#define KEY_MATRIX_PORT P1

/**
 * 使用线翻转扫描4*4矩阵按键
 * @return 如果按下返回 1~16，没有按下返回0
 */
u8 key_matrix_flip_scan(){
    u8 k = 0;

    KEY_MATRIX_PORT = 0x0f; // 所有行为0，列为1
    if(KEY_MATRIX_PORT != 0x0f){ // 判断按键是否按下
        delay1ms(5); // 消抖
        if(KEY_MATRIX_PORT != 0x0f){
            // 检测行变化
            KEY_MATRIX_PORT = 0x0f; // 所有列为低电平，switch中检测行是否有低电平。
            switch(KEY_MATRIX_PORT){
                case 0x07: k=1; break;
                case 0x0b: k=2; break;
                case 0x0d: k=3; break;
                case 0x0e: k=4; break;
            }

            // 检测列变化
            KEY_MATRIX_PORT = 0xf0;
            switch(KEY_MATRIX_PORT){
                case 0x70: k+=0; break;
                case 0xb0: k+=4; break;
                case 0xd0: k+=8; break;
                case 0xe0: k+=12; break;
            }

            while(KEY_MATRIX_PORT!=0xf0); // 等待按键松开

        }
        
    }

    return k;
}
