/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"gpio.h"
#include	"UART.h"
#include	"delay.h"
#include 	"eeprom.h"
#include <string.h>

/*************	��������˵��	**************

�����̻���STC8H8K64UΪ����оƬ��ʵ����8���б�д���ԣ�STC8G��STC8Hϵ��оƬ��ͨ�òο�.

ͨ�����ڶ�STC�ڲ��Դ���EEPROM(FLASH)���ж�д���ԡ�

��FLASH������������д�롢�����Ĳ���������ָ����ַ��

Ĭ�ϲ�����:  115200,N,8,1. 

������������: (������ĸ�����ִ�Сд)
    E 0x0040             --> ��0x0040��ַ�������ݽ��в���.
    W 0x0040 1234567890  --> ��0x0040��ַд���ַ�1234567890.
    R 0x0040 10          --> ��0x0040��ַ����10���ֽ�����. 

ע�⣺����ʱ�����ؽ���"Ӳ��ѡ��"�������û�EEPROM��С��

��ȷ�����������еĵ�ַ��EEPROM���õĴ�С��Χ֮�ڡ�

����ʱ, ѡ��ʱ�� 22.1184MHz (�����������ļ�"config.h"���޸�).

******************************************/

#define     Max_Length          100      //��дEEPROM���峤��

/*************	���س�������	**************/
#define LVD2V0 0x00 //LVD@2.0V
#define LVD2V4 0x01 //LVD@2.4V
#define LVD2V7 0x02 //LVD@2.7V
#define LVD3V0 0x03 //LVD@3.0V

/*************	���ر�������	**************/
u8  xdata   tmp[Max_Length];        //EEPROM��������
u8  xdata rxTmp[Max_Length];

/*************	���غ�������	**************/


/*************  �ⲿ�����ͱ������� *****************/


/******************* IO���ú��� *******************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO, GPIO_Pin_0 ~ GPIO_Pin_7
	GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��
    
    
	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//ָ��Ҫ��ʼ����IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;		//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
}

/***************  ���ڳ�ʼ������ *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Priority    = Priority_0;			//ָ���ж����ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
	UART_Configuration(UART1, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4
}

/**********************************************/

u8	CheckData(u8 dat)
{
	if((dat >= '0') && (dat <= '9'))		return (dat-'0');
	if((dat >= 'A') && (dat <= 'F'))		return (dat-'A'+10);
	if((dat >= 'a') && (dat <= 'f'))		return (dat-'a'+10);
	return 0xff;
}

//========================================================================
// ����: u16    GetAddress(void)
// ����: ����������뷽ʽ�ĵ�ַ.
// ����: ��.
// ����: 16λEEPROM��ַ.
// �汾: V1.0, 2013-6-6
//========================================================================
u16 GetAddress(void)
{
	u16 address;
	u8  i,j;
	
	address = 0;
	if((RX1_Buffer[2] == '0') && (RX1_Buffer[3] == 'X'))
	{
		for(i=4; i<8; i++)
		{
			j = CheckData(RX1_Buffer[i]);
			if(j >= 0x10)   return 65535;   //error
			address = (address << 4) + j;
		}
		return (address);
	}
	return  65535;  //error
}

/**************** ��ȡҪ�������ݵ��ֽ��� ****************************/
u8 GetDataLength(void)
{
	u8  i;
	u8  length;
	
	length = 0;
	for(i=9; i<COM1.RX_Cnt; i++)
	{
		if(CheckData(RX1_Buffer[i]) >= 10)  break;
		length = length * 10 + CheckData(RX1_Buffer[i]);
	}
	return (length);
}

u8 dataFlag=0;
u16 addr, aLen=0; // ��ַ
u16 addrBak = 0x0200; // ���õ�ַ ��2����

// ��ѹ�ж�
void Lvd_Isr() interrupt 6
{
   
    if(dataFlag==1){
        EEPROM_SectorErase(addr); 
        EEPROM_write_n(addr,rxTmp,aLen);      //дN���ֽ�
        EEPROM_SectorErase(addrBak); 
        EEPROM_write_n(addrBak,rxTmp,aLen);      //дN���ֽ�
        P33 = 1; // ���Զ˿�
        
        dataFlag = 0;
    }
    
    
    PCON &= ~0X20; // ���жϱ�־
}

/********************* ������ *************************/
void main(void)
{
	u8  i,j,k;
	u8 type, len, xorNum;
	u8  status;
    
    // ָʾled
    P33 = 0;

    
    
    
	GPIO_config();
	UART_config();
	EA = 1;

    // ��ѹ����
    PCON &= ~0x20; // ���Զ˿�
    RSTCFG = LVD3V0; // ʹ�� 3.0V ʱ��ѹ�ж�
    ELVD = 1; // ʹ�� LVD �ж�
    EA = 1;
   
    
    /*
	PrintString1("STC8ϵ�е�Ƭ��EEPROM���Գ��򣬴���������������:\r\n");    //UART1����һ���ַ���
	PrintString1("E 0x0040             --> ��0x0040��ַ�������ݽ��в���.\r\n");     //UART1����һ���ַ���
	PrintString1("W 0x0040 1234567890  --> ��0x0040��ַд���ַ�1234567890.\r\n");  //UART1����һ���ַ���
	PrintString1("R 0x0040 10          --> ��0x0040��ַ����10���ֽ�����.\r\n");    //UART1����һ���ַ���
    
    ���գ�
    99 66 type addrL addrH len xx xx xx ... xor(2~end)
    type = 'E' 'W' 'R' ����
    
    99 66 (45)E 10 00 00 xor
    99 66 (57)W 10 00 03 d1 d2 d3 xor
    99 66 (52)R 10 00 01 rL xor (�����rL�����ȡ���ݳ��ȣ�
    
    ���ͣ�
    99 66 len data... xor(2~end);
    
    
    */
    
     // ��ʼ�Ͳ������õ�ַ�����ж���ֻд��
    //EEPROM_SectorErase(addrBak);
    
	while(1)
	{
        delay_ms(1);
        if(COM1.RX_TimeOut > 0)		//��ʱ����
        {
            if(--COM1.RX_TimeOut == 0)
            {
                //for(i=0; i<COM1.RX_Cnt; i++)    TX1_write2buff(RX1_Buffer[i]);    //���յ�������ԭ������,���ڲ���
                status = 0xff;  //״̬��һ����0ֵ
                xorNum = 0;
                while(1){
                    if((COM1.RX_Cnt >= 7) && (RX1_Buffer[0] == 0x99) && (RX1_Buffer[1] == 0x66)) //�������Ϊ7���ֽ�
                    {
                        type = RX1_Buffer[2];
                        addr = (RX1_Buffer[4]<<8) + RX1_Buffer[3];
                        len = RX1_Buffer[5];
                        
                        
                        // �ж�У��
                        if(len+7>COM1.RX_Cnt){
                            break;
                        }
                        for(i=2; i<len+6; i++){
                            xorNum ^= RX1_Buffer[i];
                        }
                        if(xorNum != RX1_Buffer[i]){
                            break;
                        }
                        
                        if(type == 'R'){
                            len = RX1_Buffer[6];
                        }
                        
                        
                        if(addr < 63488)    //������0~123������ÿ������ 512 �ֽ�
                        {
                            if(type == 'E')    //д��N���ֽ�
                            {
                                EEPROM_SectorErase(addr);           //��������
                                status = 0; //������ȷ
                            }

                            else if((type == 'W') && (len>0))    //д��N���ֽ�
                            {
                                if(len > Max_Length)  len = Max_Length; //Խ����
                                //EEPROM_SectorErase(addr);           //��������
                                //EEPROM_write_n(addr,&RX1_Buffer[6],len);      //дN���ֽ�
                                memcpy(rxTmp, &RX1_Buffer[6], len);
                                aLen = len;
                                dataFlag = 1;
                                
                                /*
                                if(j >= 100)    {TX1_write2buff(j/100+'0');   j = j % 100;}
                                if(j >= 10)     {TX1_write2buff(j/10+'0');    j = j % 10;}
                                TX1_write2buff(j%10+'0');
                                PrintString1("�ֽڣ�\r\n");
                                */
                                status = 0; //������ȷ
                            }

                            else if((type == 'R') && (len>0))   //PC���󷵻�N�ֽ�EEPROM����
                            {
                                if(len > Max_Length)  len = Max_Length; //Խ����
                                EEPROM_read_n(addr,tmp,len);
                                TX1_write2buff(0x99);
                                TX1_write2buff(0x66);
                                xorNum=0;
                                TX1_write2buff(len);
                                xorNum^=len;
                                for(i=0; i<len; i++)  {
                                    TX1_write2buff(tmp[i]); xorNum^=tmp[i];
                                }
                                TX1_write2buff(xorNum);
                                status = 0; //������ȷ
                                
                            }
                        }else{
                            //PrintString1("4 ");
                        }
                    }
                    
                    break;
                }
                if(status != 0){   
                    PrintString1("err");
                }
                else{
                    //PrintString1("ok!");
                }
                
                COM1.RX_Cnt = 0;
            }
        }
    }
} 
/**********************************************/


