#include "matrix.h"

#define KEY_MATRIX_PORT P1

/**
 * ʹ������ʽɨ��4*4���󰴼��Ƿ���
 * @return ������·��� 1~16��û�а��·���0
 */
u8 key_matrix_ranks_scan(){
    u8 k = 0;

    // ��һ�д���
    KEY_MATRIX_PORT = 0xF7; // ����һ�и��͵�ƽ0,�������У�Ҳ���������У�Ϊ�ߵ�ƽ
    if(KEY_MATRIX_PORT != 0xF7){ // �жϵ�һ���Ƿ���ĳ�а�������
        delay1ms(5); // ����
        switch(KEY_MATRIX_PORT){
            case 0x77: k=1; break;
            case 0xb7: k=5; break;
            case 0xd7: k=9; break;
            case 0xe7: k=13; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xF7); // �ȴ������ɿ�
   
    // �ڶ��д���
    KEY_MATRIX_PORT = 0xFB; // ���ڶ��и��͵�ƽ0,�������У�Ҳ���������У�Ϊ�ߵ�ƽ
    if(KEY_MATRIX_PORT != 0xFB){
        delay1ms(5); // ����
        switch(KEY_MATRIX_PORT){
            case 0x7b: k=2; break;
            case 0xbb: k=6; break;
            case 0xdb: k=10; break;
            case 0xeb: k=14; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFB); // �ȴ������ɿ�

    // �����д���
    KEY_MATRIX_PORT = 0xFD; // ���ڶ��и��͵�ƽ0,�������У�Ҳ���������У�Ϊ�ߵ�ƽ
    if(KEY_MATRIX_PORT != 0xFD){
        delay1ms(5); // ����
        switch(KEY_MATRIX_PORT){
            case 0x7d: k=3; break;
            case 0xbd: k=7; break;
            case 0xdd: k=11; break;
            case 0xed: k=15; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFD); // �ȴ������ɿ�

    // �����д���
    KEY_MATRIX_PORT = 0xFE; // ���ڶ��и��͵�ƽ0,�������У�Ҳ���������У�Ϊ�ߵ�ƽ
    if(KEY_MATRIX_PORT != 0xFE){
        delay1ms(5); // ����
        switch(KEY_MATRIX_PORT){
            case 0x7e: k=4; break;
            case 0xbe: k=8; break;
            case 0xde: k=12; break;
            case 0xee: k=16; break;
        }
    }
    while(KEY_MATRIX_PORT!=0xFE); // �ȴ������ɿ�
      
    return k;
}

#define KEY_MATRIX_PORT P1

/**
 * ʹ���߷�תɨ��4*4���󰴼�
 * @return ������·��� 1~16��û�а��·���0
 */
u8 key_matrix_flip_scan(){
    u8 k = 0;

    KEY_MATRIX_PORT = 0x0f; // ������Ϊ0����Ϊ1
    if(KEY_MATRIX_PORT != 0x0f){ // �жϰ����Ƿ���
        delay1ms(5); // ����
        if(KEY_MATRIX_PORT != 0x0f){
            // ����б仯
            KEY_MATRIX_PORT = 0x0f; // ������Ϊ�͵�ƽ��switch�м�����Ƿ��е͵�ƽ��
            switch(KEY_MATRIX_PORT){
                case 0x07: k=1; break;
                case 0x0b: k=2; break;
                case 0x0d: k=3; break;
                case 0x0e: k=4; break;
            }

            // ����б仯
            KEY_MATRIX_PORT = 0xf0;
            switch(KEY_MATRIX_PORT){
                case 0x70: k+=0; break;
                case 0xb0: k+=4; break;
                case 0xd0: k+=8; break;
                case 0xe0: k+=12; break;
            }

            while(KEY_MATRIX_PORT!=0xf0); // �ȴ������ɿ�

        }
        
    }

    return k;
}
