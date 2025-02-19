#include "i2c.h"
#include <intrins.h>

// ��������
void iic_init(){

    // P20 P21 ��©ģʽ
    P2M0 |= 0x03; P2M1 |= 0x03; 

    // �����ڲ�����
    P2PU |= 0x03; 

}


void delay10us(void)	//@11.0592MHz
{
	unsigned char data i;

	_nop_();
	i = 25;
	while (--i);
}


void iic_delay(){
    delay10us();
}

// ���Ϳ�ʼ�źš�sclΪ�ߵ�ƽʱ��SDA�ɸߵ�������
void iic_start(){
    IIC_SDA = 1;
    IIC_SCL = 1;
    iic_delay();
    
    IIC_SDA = 0;
    iic_delay();
    IIC_SCL = 0;
}

// ����ֹͣ�źš� sclΪ�ߵ�ƽʱ��SDA�ɵ͵�������
void iic_stop(){
    IIC_SDA = 0;
    IIC_SCL = 1;
    iic_delay();
    
    IIC_SDA = 1;
    iic_delay();
}

// ���� ACK �źš�SCL�ɵ͵����ٵ�ʱ��SDAΪ�͵�ƽ
void iic_ack(){
    IIC_SCL = 0;
    IIC_SDA = 0;
    iic_delay();

    IIC_SCL = 1;
    iic_delay();
    IIC_SCL = 0;
    iic_delay();
}

// ���� no Ack �źš�SCL�ɵ͵����ٵ�ʱ��SDAΪ�ߵ�ƽ
void iic_nack(){
    IIC_SCL = 0;
    IIC_SDA = 1;
    iic_delay();

    IIC_SCL = 1;
    iic_delay();
    IIC_SCL = 0;
    iic_delay();
}

// �ȴ�ACK�ź�
u8 iic_wait_ack(){
    u8 timeTemp = 0;

    IIC_SCL = 1;
    while(IIC_SDA){ // �ȴ�SDAΪ�͵�ƽ
        if(timeTemp++>120){
            iic_stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

// ����һ���ֽڡ�������Ч�ԣ�SCLΪ�ߵ�ƽ�ڼ䣬SDA���ݱ����ȶ�
void iic_write_byte(u8 txd){
    u8 i = 0;

    IIC_SCL = 0;
    for(i=0; i<8; i++){
        if(txd&0x80){ // �������λ
            IIC_SDA = 1;
        }else{
            IIC_SDA = 0;
        }

        txd <<= 1;
        iic_delay();
        IIC_SCL = 1;
        iic_delay();
        IIC_SCL = 0;
        iic_delay();
    }
}

/**
 * @brief ��ȡһ���ֽ�
 * 
 * @param ack ack��Ϊ0 ����ack, ���߷��� nack
 * @return u8 
 */
u8 iic_read_byte(u8 ack){
    u8 i=0, r=0;
    IIC_SDA = 1;
    for(i=0; i<8; i++){
        IIC_SCL = 0;
        iic_delay();
        IIC_SCL = 1;
        r <<= 1;
        if(IIC_SDA) r++;
        iic_delay();
    }

    if(ack){
        iic_ack();
    }else{
        iic_nack();
    }
    return r;
}