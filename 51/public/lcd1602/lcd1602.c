#include "lcd1602.h"

/**
 * 写命令
 * @cmd 命令
 */
void write_cmd(u8 cmd){
    LCD_RS = 0; // 命令模式
    LCD_RW = 0; // 写数据
    LCD_E = 0; // 使能脚

    LCD_DATA = cmd; // 准备命令
    delay1ms(1);
    LCD_E = 1; // 使能脚上升沿写入
    delay1ms(1);
    LCD_E = 0; // 使能脚下降沿（负跳变）完成写入

    delay40us(); // 普通指令执行时间40微秒
}


/**
 * 写数据
 * @data 数据
 */
void write_data(u8 dat){
    LCD_RS = 1; // 数据模式
    LCD_RW = 0; // 写数据
    LCD_E = 0; // 使能脚

    LCD_DATA = dat; // 准备数据
    delay1ms(1);
    LCD_E = 1; // 使能脚上升沿写入
    delay1ms(1);
    LCD_E = 0; // 使能脚下降沿（负跳变）完成写入
}

/**
 * 清屏
 */
void clear(){
    write_cmd(0x01);
    delay1ms(2); // 执行时间 1.64ms
}

/**
 * 初始化
 */
void init(){
    write_cmd(0x38); // 功能设定指令：数据总线8位，显示两行，5*7点阵/每字符
    write_cmd(0x0c); // 显示功能：显示开，无光标，光标无闪烁
    write_cmd(0x06); // 进入模式设置：写入新数据光标右移，显示屏不移动
    clear();
}

/**
 * 显示
 * @param line 行
 * @param column 列
 * @param str 字符串
 */
void show(u8 line, u8 column, u8 *str){
    u8 i = 0;
    if(line>1 || column>15) return;

    if(line<1){ // 第一行
        while(*str != '\0'){
            if(i<16-column)
                write_cmd(0x80+i+column); // 第一行显示地址设置
            else
                write_cmd(0x80+0x40+i+column-16); // 第二行显示地址设置
            
            write_data(*str);
            str++;
            i++;
        }
    }else{
        while(*str != '\0'){
            if(i<16-column)
                write_cmd(0x80+0x40+i+column); // 第二行显示地址设置
            else
                break; // 超过则退出
            
            write_data(*str);
            str++;
            i++;
        }
    }
}
