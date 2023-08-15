#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000*10;
		while(i--);
	}
}

void EPD_Display(unsigned char *BW_Image)
{
    unsigned int Width, Height,i,j;
    Width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    Epaper_Write_Command(0x24);
    for (j = 0; j < Height; j++) {
        for ( i = 0; i < Width; i++) {
           Epaper_Write_Data(BW_Image[i + j * Width]);
        }
    }
     EPD_Update();		
}
//SSD1677
void EPD_HW_Init(void)
{	
		EPD_W21_RST_0;  // Module reset   
		delay_xms(10);//At least 10ms delay 
		EPD_W21_RST_1;
		delay_xms(10); //At least 10ms delay 
	
		Epaper_Write_Command(0x12); 		  //SWRESET
		Epaper_READBUSY();        //waiting for the electronic paper IC to release the idle signal

		Epaper_Write_Command(0x0C);  // Soft start setting
		Epaper_Write_Data(0xAE);
		Epaper_Write_Data(0xC7);
		Epaper_Write_Data(0xC3);
		Epaper_Write_Data(0xC0);
		Epaper_Write_Data(0xFF);   //0x80 To 0xFF  

		Epaper_Write_Command(0x01);  // Set MUX as 527
		Epaper_Write_Data(0x7F);
		Epaper_Write_Data(0x02);
		Epaper_Write_Data(0x00);

		Epaper_Write_Command(0x11);  // Data entry mode
		Epaper_Write_Data(0x01);
		
		Epaper_Write_Command(0x44);
		Epaper_Write_Data(0x00); // RAM x address start at 0
		Epaper_Write_Data(0x00);
		Epaper_Write_Data(0xBF); // RAM x address end at 36Fh -> 879
		Epaper_Write_Data(0x03);
		Epaper_Write_Command(0x45);
		Epaper_Write_Data(0x7F); // RAM y address start at 20Fh;
		Epaper_Write_Data(0x02);
		Epaper_Write_Data(0x00); // RAM y address end at 00h;
		Epaper_Write_Data(0x00);

		Epaper_Write_Command(0x3C); // VBD
		Epaper_Write_Data(0x01); // LUT1, for white

		Epaper_Write_Command(0x18);
		Epaper_Write_Data(0X80);

		Epaper_Write_Command(0x4E); 
		Epaper_Write_Data(0x00);
		Epaper_Write_Data(0x00);
		Epaper_Write_Command(0x4F); 
		Epaper_Write_Data(0x7F);
		Epaper_Write_Data(0x02);
}

void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); //Display Update Control
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}

/*When the electronic paper screen is updated, do not unplug the electronic paper to avoid damage to the screen*/
void EPD_Part_Update(void)
{
	Epaper_Write_Command(0x22); //Display Update Control
	Epaper_Write_Data(0xFF);   
	Epaper_Write_Command(0x20); //Activate Display Update Sequence
	Epaper_READBUSY(); 			
}
void EPD_DeepSleep(void)
{  	
  Epaper_Write_Command(0x10); //enter deep sleep
  Epaper_Write_Data(0x01); 
  delay_xms(100);
}
void EPD_WhiteScreen_ALL(const unsigned char *datas)
{
   unsigned int i;	
  Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<76800;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }
   EPD_Update();	 	 
}

void EPD_WhiteScreen_White(void)
{
   unsigned int i;
	 Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
		for(i=0;i<76800;i++)
		{
			Epaper_Write_Data(0xff);		
    }
	EPD_Update();
}
void EPD_WhiteScreen_WhiteBasemap(void)
{
   unsigned int i;
	 Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
		for(i=0;i<76800;i++)
		{
			Epaper_Write_Data(0xff);		
    }
	 Epaper_Write_Command(0x26);   //write RAM for black(0)/white (1)
		for(i=0;i<76800;i++)
		{
			Epaper_Write_Data(0xff);		
    }
	EPD_Update();
}

void Epaper_READBUSY(void)
{ 
  while(1)
  {	 //=1 BUSY
     if(isEPD_W21_BUSY==0) break;;
  }  
	
}

void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
	unsigned int i;   	
  Epaper_Write_Command(0x24);   //Write Black and White image to RAM
   for(i=0;i<76800;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }
  Epaper_Write_Command(0x26);   //Write Black and White image to RAM
   for(i=0;i<76800;i++)
   {               
     Epaper_Write_Data(datas[i]);
   }
   EPD_Update();	 
	 
}

/////////////////////////////////////TIME///////////////////////////////////////////////////
void EPD_Dis_Part_RAM(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_LINE,unsigned int PART_COLUMN)
{
	unsigned int i;  
	unsigned int x_end,x_start1,x_start2,x_end1,x_end2;
	unsigned int y_start1,y_start2,y_end1,y_end2;
	x_start=x_start;
	x_end=x_start+PART_LINE-1; 
	
	x_start1=0;
	x_start2=x_start;
	if(x_start>=256)
	{
		x_start1=x_start2/256;
		x_start2=x_start2%256;
	}	
	x_end1=0;
	x_end2=x_end;
	if(x_end2>=256)
	{
		x_end1=x_end2/256;
		x_end2=x_end2%256;		
	}		

	y_start1=0;
	y_start2=y_start;
	if(y_start>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_start+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
//Reset
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay 
	
	Epaper_Write_Command(0x3C); //BorderWavefrom  
	Epaper_Write_Data(0x80);	
//	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(x_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(x_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(x_end1);    // ????=0	
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    // ????=0	


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_start2);
	Epaper_Write_Data(x_start1);
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
	 for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
	 {                         
		 Epaper_Write_Data(datas[i]);
	 } 
}
void EPD_Dis_Part_Time(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
	                       unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
												 unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
												 unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
											   unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,
												 unsigned int PART_LINE,unsigned int PART_COLUMN
	                      )
{
  EPD_Dis_Part_RAM(x_startA,y_startA,datasA,PART_LINE,PART_COLUMN);	 //A
	EPD_Dis_Part_RAM(x_startB,y_startB,datasB,PART_LINE,PART_COLUMN);	 //B
	EPD_Dis_Part_RAM(x_startC,y_startC,datasC,PART_LINE,PART_COLUMN);	 //C
	EPD_Dis_Part_RAM(x_startD,y_startD,datasD,PART_LINE,PART_COLUMN);	 //D
	EPD_Dis_Part_RAM(x_startE,y_startE,datasE,PART_LINE,PART_COLUMN);	 //E
	EPD_Part_Update();

}

void Pic_partial_display(const unsigned char *gImage)  
{
	unsigned int i;
	
	//Reset
	EPD_W21_RST_0;  // Module reset   
	delay_xms(10);//At least 10ms delay 
	EPD_W21_RST_1;
	delay_xms(10); //At least 10ms delay 
		
	Epaper_Write_Command(0x3C); //BorderWavefrom
	Epaper_Write_Data(0x80);	
// 	
	Epaper_Write_Command(0x44);       
	Epaper_Write_Data(0x00);     
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x1F);    
	Epaper_Write_Data(0x03); 

	Epaper_Write_Command(0x45);      
	Epaper_Write_Data(0x00);			
	Epaper_Write_Data(0x00);    
	Epaper_Write_Data(0xDF);      
	Epaper_Write_Data(0x01);
	
	Epaper_Write_Command(0x4E);     
  Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	
	Epaper_Write_Command(0x4F);      
  Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_READBUSY();   
	
/*******************bw image************/		
	Epaper_Write_Command(0x24);		
		for(i=0;i<76800;i++)
		{
			Epaper_Write_Data(gImage[i]);		
    }   
	 EPD_Part_Update();  
}

/***********************************************************
						end file
***********************************************************/

