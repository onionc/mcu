/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"config.h"
#include	"gpio.h"
#include	"UART.h"
#include	"delay.h"
#include 	"eeprom.h"
#include <string.h>

/*************	本程序功能说明	**************

本例程基于STC8H8K64U为主控芯片的实验箱8进行编写测试，STC8G、STC8H系列芯片可通用参考.

通过串口对STC内部自带的EEPROM(FLASH)进行读写测试。

对FLASH做扇区擦除、写入、读出的操作，命令指定地址。

默认波特率:  115200,N,8,1. 

串口命令设置: (命令字母不区分大小写)
    E 0x0040             --> 对0x0040地址扇区内容进行擦除.
    W 0x0040 1234567890  --> 对0x0040地址写入字符1234567890.
    R 0x0040 10          --> 对0x0040地址读出10个字节数据. 

注意：下载时，下载界面"硬件选项"中设置用户EEPROM大小，

并确保串口命令中的地址在EEPROM设置的大小范围之内。

下载时, 选择时钟 22.1184MHz (可以在配置文件"config.h"中修改).

******************************************/

#define     Max_Length          100      //读写EEPROM缓冲长度

/*************	本地常量声明	**************/
#define LVD2V0 0x00 //LVD@2.0V
#define LVD2V4 0x01 //LVD@2.4V
#define LVD2V7 0x02 //LVD@2.7V
#define LVD3V0 0x03 //LVD@3.0V

/*************	本地变量声明	**************/
u8  xdata   tmp[Max_Length];        //EEPROM操作缓冲
u8  xdata rxTmp[Max_Length];

/*************	本地函数声明	**************/


/*************  外部函数和变量声明 *****************/


/******************* IO配置函数 *******************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义

	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化
    
    
	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);
}

/***************  串口初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Priority    = Priority_0;			//指定中断优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
	UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
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
// 函数: u16    GetAddress(void)
// 描述: 计算各种输入方式的地址.
// 参数: 无.
// 返回: 16位EEPROM地址.
// 版本: V1.0, 2013-6-6
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

/**************** 获取要读出数据的字节数 ****************************/
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
u16 addr, aLen=0; // 地址
u16 addrBak = 0x0200; // 备用地址 第2扇区

// 低压中断
void Lvd_Isr() interrupt 6
{
   
    if(dataFlag==1){
        EEPROM_SectorErase(addr); 
        EEPROM_write_n(addr,rxTmp,aLen);      //写N个字节
        EEPROM_SectorErase(addrBak); 
        EEPROM_write_n(addrBak,rxTmp,aLen);      //写N个字节
        P33 = 1; // 测试端口
        
        dataFlag = 0;
    }
    
    
    PCON &= ~0X20; // 清中断标志
}

/********************* 主函数 *************************/
void main(void)
{
	u8  i,j,k;
	u8 type, len, xorNum;
	u8  status;
    
    // 指示led
    P33 = 0;

    
    
    
	GPIO_config();
	UART_config();
	EA = 1;

    // 低压测试
    PCON &= ~0x20; // 测试端口
    RSTCFG = LVD3V0; // 使能 3.0V 时低压中断
    ELVD = 1; // 使能 LVD 中断
    EA = 1;
   
    
    /*
	PrintString1("STC8系列单片机EEPROM测试程序，串口命令设置如下:\r\n");    //UART1发送一个字符串
	PrintString1("E 0x0040             --> 对0x0040地址扇区内容进行擦除.\r\n");     //UART1发送一个字符串
	PrintString1("W 0x0040 1234567890  --> 对0x0040地址写入字符1234567890.\r\n");  //UART1发送一个字符串
	PrintString1("R 0x0040 10          --> 对0x0040地址读出10个字节内容.\r\n");    //UART1发送一个字符串
    
    接收：
    99 66 type addrL addrH len xx xx xx ... xor(2~end)
    type = 'E' 'W' 'R' 擦除
    
    99 66 (45)E 10 00 00 xor
    99 66 (57)W 10 00 03 d1 d2 d3 xor
    99 66 (52)R 10 00 01 rL xor (这里的rL代表获取数据长度）
    
    发送：
    99 66 len data... xor(2~end);
    
    
    */
    
     // 开始就擦除备用地址，在中断中只写入
    //EEPROM_SectorErase(addrBak);
    
	while(1)
	{
        delay_ms(1);
        if(COM1.RX_TimeOut > 0)		//超时计数
        {
            if(--COM1.RX_TimeOut == 0)
            {
                //for(i=0; i<COM1.RX_Cnt; i++)    TX1_write2buff(RX1_Buffer[i]);    //把收到的数据原样返回,用于测试
                status = 0xff;  //状态给一个非0值
                xorNum = 0;
                while(1){
                    if((COM1.RX_Cnt >= 7) && (RX1_Buffer[0] == 0x99) && (RX1_Buffer[1] == 0x66)) //最短命令为7个字节
                    {
                        type = RX1_Buffer[2];
                        addr = (RX1_Buffer[4]<<8) + RX1_Buffer[3];
                        len = RX1_Buffer[5];
                        
                        
                        // 判断校验
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
                        
                        
                        if(addr < 63488)    //限制在0~123扇区，每个扇区 512 字节
                        {
                            if(type == 'E')    //写入N个字节
                            {
                                EEPROM_SectorErase(addr);           //擦除扇区
                                status = 0; //命令正确
                            }

                            else if((type == 'W') && (len>0))    //写入N个字节
                            {
                                if(len > Max_Length)  len = Max_Length; //越界检测
                                //EEPROM_SectorErase(addr);           //擦除扇区
                                //EEPROM_write_n(addr,&RX1_Buffer[6],len);      //写N个字节
                                memcpy(rxTmp, &RX1_Buffer[6], len);
                                aLen = len;
                                dataFlag = 1;
                                
                                /*
                                if(j >= 100)    {TX1_write2buff(j/100+'0');   j = j % 100;}
                                if(j >= 10)     {TX1_write2buff(j/10+'0');    j = j % 10;}
                                TX1_write2buff(j%10+'0');
                                PrintString1("字节！\r\n");
                                */
                                status = 0; //命令正确
                            }

                            else if((type == 'R') && (len>0))   //PC请求返回N字节EEPROM数据
                            {
                                if(len > Max_Length)  len = Max_Length; //越界检测
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
                                status = 0; //命令正确
                                
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


