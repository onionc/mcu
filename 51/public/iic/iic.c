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

/**
 * @brief ��ָ�������ļĴ���ֵ��д��
 * @param devAddr ������ַ
 * @param addr �����Ĵ�����ַ
 * @param pData �����������׵�ַ
 * @param len ���������ݳ��� (���ֽ�Ϊ��λ)
 * @return u8 0�ɹ���1ʧ��
 */
u8 iic_write_mem(u8 devAddr, u8 addr, u8 *pData, u16 len){
    iic_start();
    iic_write_byte(devAddr<<1); // ����һλ����������0��ʾ������ӻ�д����
    if(iic_wait_ack())
        return 1;
    iic_write_byte(addr);
    if(iic_wait_ack())
        return 1;
    while(len--){
        iic_write_byte(*pData++);
        if(iic_wait_ack())
        return 1;
    }

    iic_stop();
    return 0;
}

/**
 * @brief ��ȡָ�������ļĴ���ֵ
 * @param devAddr ������ַ
 * @param addr �����Ĵ�����ַ
 * @param pData �����׵�ַ
 * @param len ���ݳ���
 * @return u8 0�ɹ���1ʧ��
 */
u8 iic_read_mem(u8 devAddr, u8 addr, u8 *pBuf, u16 len){
    iic_start();
    iic_write_byte(devAddr<<1);
    if(iic_wait_ack())
        return 1;
    iic_write_byte(addr);
    if(iic_wait_ack())
        return 1;

    iic_start();
    iic_write_byte(devAddr<<1 | 1);
    if(iic_wait_ack())
        return 1;
    while(len--){
        *pBuf++ = iic_read_byte(len!=0);
    }
    iic_stop();
    return 0;
}

/**
 * @brief д�루8λ�Ĵ����ģ����λ
 * @param devAddr ������ַ
 * @param addr �����Ĵ�����ַ
 * @param start ��ʼλ�ã�0~7��
 * @param len д�����ݳ���
 * @param dat д�������
 * @return u8 0�ɹ���1ʧ��
 */
u8 iic_write_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 dat){
    u8 b, mask=0;
    if(start>7){
        // start ȡֵ��Χ 0~7
        return 1;
    }
    if(len+start<8){
        // len+start���Ϊ8�������ּ������ start=0 & len=8, �� start=7 && len=1
        return 1;
    }

    /*
    Ҫд���ֵ 10 
    76543210 ����λ
       xx    args: start=4, len=2
    00011000 �����ֽ�
    10101111 ԭʼֵ��������
    10100111 ԭʼֵ & ~����
    10110011 ���� | ԭʼֵ
    */
    if(!iic_read_mem(devAddr, addr, &b, 1)){
        mask = ((1<<len)-1) << (start-len+1); // ����
        // ����д�������ƶ���ָ��λ��
        dat <<= (start-len+1); 
        dat &= mask;

        b &= ~mask;
        b != dat;
        return iic_write_mem(devAddr, addr, &b, 1); // д������
    }

    return 1;
}

/**
 * @brief ��ȡ��8λ�Ĵ����ģ����λ
 * @param devAddr ������ַ
 * @param addr �����Ĵ�����ַ
 * @param start ��ʼλ�ã�0~7��
 * @param len ��ȡ���ݳ���
 * @param pBuf ���ݴ洢��ַ
 * @return u8 0�ɹ���1ʧ��
 */
u8 iic_read_bits(u8 devAddr, u8 addr, u8 start, u8 len, u8 *pBuf){
    u8 b, mask=0;
    if(start>7){
        // start ȡֵ��Χ 0~7
        return 1;
    }
    if(len+start<8){
        // len+start���Ϊ8�������ּ������ start=0 & len=8, �� start=7 && len=1
        return 1;
    }

    /*
    01101001 ��ȡ�ֽ�
    76543210 ����λ
       xxx   args: start=4, len=3
       111   masked
       010   ���>>��λ����λ
    */
    if(!iic_read_mem(devAddr, addr, &b, 1)){

        mask = ((1<<len)-1) << (start-len+1); // ����
        b &= mask;
        b >>= (start-len+1);
        *pBuf = b;
        return 0;
    }
		return 1;
}
