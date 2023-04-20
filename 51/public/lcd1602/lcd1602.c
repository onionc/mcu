#include "lcd1602.h"

/**
 * д����
 * @cmd ����
 */
void write_cmd(u8 cmd){
    LCD_RS = 0; // ����ģʽ
    LCD_RW = 0; // д����
    LCD_E = 0; // ʹ�ܽ�

    LCD_DATA = cmd; // ׼������
    delay1ms(1);
    LCD_E = 1; // ʹ�ܽ�������д��
    delay1ms(1);
    LCD_E = 0; // ʹ�ܽ��½��أ������䣩���д��

    delay40us(); // ��ָͨ��ִ��ʱ��40΢��
}


/**
 * д����
 * @data ����
 */
void write_data(u8 dat){
    LCD_RS = 1; // ����ģʽ
    LCD_RW = 0; // д����
    LCD_E = 0; // ʹ�ܽ�

    LCD_DATA = dat; // ׼������
    delay1ms(1);
    LCD_E = 1; // ʹ�ܽ�������д��
    delay1ms(1);
    LCD_E = 0; // ʹ�ܽ��½��أ������䣩���д��
}

/**
 * ����
 */
void clear(){
    write_cmd(0x01);
    delay1ms(2); // ִ��ʱ�� 1.64ms
}

/**
 * ��ʼ��
 */
void init(){
    write_cmd(0x38); // �����趨ָ���������8λ����ʾ���У�5*7����/ÿ�ַ�
    write_cmd(0x0c); // ��ʾ���ܣ���ʾ�����޹�꣬�������˸
    write_cmd(0x06); // ����ģʽ���ã�д�������ݹ�����ƣ���ʾ�����ƶ�
    clear();
}

/**
 * ��ʾ
 * @param line ��
 * @param column ��
 * @param str �ַ���
 */
void show(u8 line, u8 column, u8 *str){
    u8 i = 0;
    if(line>1 || column>15) return;

    if(line<1){ // ��һ��
        while(*str != '\0'){
            if(i<16-column)
                write_cmd(0x80+i+column); // ��һ����ʾ��ַ����
            else
                write_cmd(0x80+0x40+i+column-16); // �ڶ�����ʾ��ַ����
            
            write_data(*str);
            str++;
            i++;
        }
    }else{
        while(*str != '\0'){
            if(i<16-column)
                write_cmd(0x80+0x40+i+column); // �ڶ�����ʾ��ַ����
            else
                break; // �������˳�
            
            write_data(*str);
            str++;
            i++;
        }
    }
}
