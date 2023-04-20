#include "iic.h"
#include "../common.h"

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

/**
 * @brief ��ָ�������ļĴ���ֵ��д��
 * �������ݹ���Ϊ��S �ӻ���ַ 0 A ���� A ���� A/noA P
 * @param devAddr ������ַ
 * @param addr �����Ĵ�����ַ
 * @param pData �����������׵�ַ
 * @param len ���������ݳ���
 * @return u8 0�ɹ���1ʧ��
 */
u8 iic_write_mem(u8 devAddr, u8 addr, u8 *pData, u16 len){
    iic_start();
    iic_write_byte(devAddr<<1); // ����һλ����������0��ʾ������ӻ�д����
    if(iic_wait_ack())
        return 1;
    iic_write_byte(addr);
}

u8 iic_read_mem(u8 devAddr, u8 addr, u8 *pBuf, u16 len); // ��ָ�������ļĴ���ֵ�Ķ�ȡ

u8 iic_write_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 data); // д��8λ�Ĵ�����λ��1���λ��
u8 iic_read_bits(u8 devAddr, u8 addr, u8 start, u8 *pBuf); // ��ȡ��λ
