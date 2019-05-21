#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	   

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
  
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	 
//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{   
	LCD->LCD_REG=regval;//д��Ҫд�ļĴ������	 
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{	 
	LCD->LCD_RAM=data;		 
}
//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;	
	return ram;	 
}					   
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//д��Ҫд�ļĴ������	 
	LCD->LCD_RAM = LCD_RegValue;//д������	    		 
}	   
//���Ĵ���
//LCD_Reg:�Ĵ�����ַ
//����ֵ:����������
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//д��Ҫ���ļĴ������
	delay_us(5);		  
	return LCD_RD_DATA();		//���ض�����ֵ
}   
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
} 

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//�ر���ʾ
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);
	  LCD_WR_DATA(Xpos&0XFF); 		
	  LCD_WR_DATA((lcddev.width-1)>>8);
	  LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);
	  LCD_WR_DATA(Ypos&0XFF); 	
    LCD_WR_DATA((lcddev.height-1)>>8);
	  LCD_WR_DATA((lcddev.height-1)&0XFF);
} 		 
//����LCD���Զ�ɨ�跽��
  void LCD_Scan_Dir(u8 dir)
{
		u16 regval=0;
		u16 dirreg=0;
		u16 temp;  
	switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
    dirreg=0X36; 
		LCD_WriteReg(dirreg,regval);
    LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
  	LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
}     
    
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD->LCD_RAM=POINT_COLOR; 
}

//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{   
	if(dir==0)
	{
		lcddev.dir=1;	
		lcddev.width=320;
		lcddev.height=240;
		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  
		LCD_WriteReg(0x36,0xC9);
		LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
  }	 
}	

//��ʼ��lcd
void LCD_Init(void)
{ 					
 	GPIO_InitTypeDef GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//ʹ��FSMCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);//ʹ��PORTB,D,E,G�Լ�AFIO���ù���ʱ��

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 			//PC8 ������� ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	//�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
 	//PORTD�����������  
	//PD14 - D0       	PD15 - D1
	//PD0  - D2					PD1 - D3
	//PD9  - D14			 	PD10 - D15
	//PD7--NE1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_7|GPIO_Pin_11;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 	//�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  	 
	//PORTE�����������  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //	//PORTD�����������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 	//�����������   
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);    	    	 											 
	
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/36M=27ns
  readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
  readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // ���ݱ���ʱ��Ϊ16��HCLK,��ΪҺ������IC�Ķ����ݵ�ʱ���ٶȲ���̫�죬�����1289���IC��
  readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
  readWriteTiming.FSMC_CLKDivision = 0x00;
  readWriteTiming.FSMC_DataLatency = 0x00;
  readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    

	writeTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK  
  writeTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨A		
  writeTiming.FSMC_DataSetupTime = 0x03;		 ////���ݱ���ʱ��Ϊ4��HCLK	
  writeTiming.FSMC_BusTurnAroundDuration = 0x00;
  writeTiming.FSMC_CLKDivision = 0x00;
  writeTiming.FSMC_DataLatency = 0x00;
  writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 

 
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
	//FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//  ��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // ���������ݵ�ַ
  FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit   
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  �洢��дʹ��
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // ��дʹ�ò�ͬ��ʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //��дʱ��
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //дʱ��

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

 	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1 
	//FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  // ʹ��BANK1 
		
	delay_ms(50); 					// delay 50 ms 
 	lcddev.id=0x9341;
	LCD_Clear(YELLOW);
	delay_ms(5000); 					// delay 5s 
}  

void LCD_Reginit(void)
{
	LCD_WR_REG(0xC8);       //Set EXTC
  LCD_WR_DATA(0xFF);
  LCD_WR_DATA(0x93);
  LCD_WR_DATA(0x42);
  
  LCD_WR_REG(0x36);       //Memory Access Control
  LCD_WR_DATA(0xC8); //MY,MX,MV,ML,BGR,MH
  LCD_WR_REG(0x3A);       //Pixel Format Set
  LCD_WR_DATA(0x55); //DPI [2:0],DBI [2:0]
  
  LCD_WR_REG(0xC0);       //Power Control 1
  LCD_WR_DATA(0x10); //VRH[5:0]
  LCD_WR_DATA(0x10); //VC[3:0]
  
  LCD_WR_REG(0xC1);       //Power Control 2
  LCD_WR_DATA(0x01); //SAP[2:0],BT[3:0]
  
  LCD_WR_REG(0xC5);       //VCOM
  LCD_WR_DATA(0xF5);
  
  LCD_WR_REG(0xB1);      
  LCD_WR_DATA(0x00);     
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xB4);      
  LCD_WR_DATA(0x00);
  
  LCD_WR_REG(0xE0);
  LCD_WR_DATA(0x0F);//P01-VP63   
  LCD_WR_DATA(0x13);//P02-VP62   
  LCD_WR_DATA(0x17);//P03-VP61   
  LCD_WR_DATA(0x04);//P04-VP59   
  LCD_WR_DATA(0x13);//P05-VP57   
  LCD_WR_DATA(0x07);//P06-VP50   
  LCD_WR_DATA(0x40);//P07-VP43   
  LCD_WR_DATA(0x39);//P08-VP27,36
  LCD_WR_DATA(0x4F);//P09-VP20   
  LCD_WR_DATA(0x06);//P10-VP13   
  LCD_WR_DATA(0x0D);//P11-VP6    
  LCD_WR_DATA(0x0A);//P12-VP4    
  LCD_WR_DATA(0x1F);//P13-VP2    
  LCD_WR_DATA(0x22);//P14-VP1    
  LCD_WR_DATA(0x00);//P15-VP0    
  
  LCD_WR_REG(0xE1);
  LCD_WR_DATA(0x00);//P01
  LCD_WR_DATA(0x21);//P02
  LCD_WR_DATA(0x24);//P03
  LCD_WR_DATA(0x03);//P04
  LCD_WR_DATA(0x0F);//P05
  LCD_WR_DATA(0x05);//P06
  LCD_WR_DATA(0x38);//P07
  LCD_WR_DATA(0x32);//P08
  LCD_WR_DATA(0x49);//P09
  LCD_WR_DATA(0x00);//P10
  LCD_WR_DATA(0x09);//P11
  LCD_WR_DATA(0x08);//P12
  LCD_WR_DATA(0x32);//P13
  LCD_WR_DATA(0x35);//P14
  LCD_WR_DATA(0x0F);//P15
  
  LCD_WR_REG(0x11);//Exit Sleep
  delay_ms(120);
  LCD_WR_REG(0x29);//Display On
  LCD_Display_Dir(0);	//
	LCD_LED=1;					//��������
	LCD_Clear(BLUE);
}

//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���	 			
	LCD_SetCursor(0x00,0x0000);		//���ù��λ��  
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	
	}
}  
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{   
	u16 i,j;
		u16 xlen=0;
		xlen=ey-sy+1;	
	
		for(i=sx;i<=ex;i++)
		{   
			  LCD_SetCursor(i,sy);  
	    	LCD_WriteRAM_Prepare();    
			  for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//��ʾ��ɫ 	    
		}
		
}  
					  
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,1);
        x+=size/2;
        p++;
    }  
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);
	u16 colortemp=POINT_COLOR;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    for(t=0;t<csize;t++)
	    {   
				if(size==12)temp=asc2_1206[num][t];  //����1206����
				else if(size==16)temp=asc2_1608[num][t];	//����1608����
				else if(size==24)temp=asc2_2412[num][t];	
				else return;				
	      for(t1=0;t1<8;t1++)
			  {			    
		      if(temp&0x80)POINT_COLOR=colortemp;
				  else POINT_COLOR=BACK_COLOR;
				  LCD_DrawPoint(x,y);	
				  temp<<=1;
				  y++;
				  if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
				  if((y-y0)==size)
				 {
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			 }  	 
	   }    
	}else//���ӷ�ʽ
	{
	    for(t=0;t<csize;t++)
	    {   
			if(size==12)temp=asc2_1206[num][t];  //����1206����
			else if (size==16)temp=asc2_1608[num][t];		 //����1608���� 
      else if(size==24)temp=asc2_2412[num][t];	
			else return;				
	        for(t1=0;t1<8;t1++)
			{			    
		        if(temp&0x80)LCD_DrawPoint(x,y); 
				temp<<=1;
				y++;
				if(y>=lcddev.height){POINT_COLOR=colortemp;return;}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					if(x>=lcddev.width){POINT_COLOR=colortemp;return;}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR=colortemp;	  
	
}   


void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode)
{
	u16 len=strlen((const char *)str);
	u16 x1=(lcddev.width-len*8)/2;
	Show_Str(x+x1,y,fc,bc,str,size,mode);
} 

void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   

void LCD_DrawPoint_16Bit(u16 color)
{
	LCD_WR_DATA(color); 
}

void LCD_ShowCharone(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
{  
    u8 temp;
    u8 pos,t;
	u16 colortemp=POINT_COLOR;      
		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x,y,x+size/2-1,y+size-1);//���õ���������ʾ����
	if(!mode) //�ǵ��ӷ�ʽ
	{
		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint_16Bit(fc); 
				else LCD_DrawPoint_16Bit(bc); 
				temp>>=1; 
				
		    }
			
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//����1206����
			else temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����    
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//�ָ�����Ϊȫ��    	   	 	  
} 

void Gui_Drawbmp16WH(u16 x,u16 y,u16 width,u16 hight,const unsigned char *p) //��ʾͼƬ
{
  	int i; 
	unsigned char picH,picL; 
	LCD_SetWindows(x,y,x+width-1,y+hight-1);//��������
    for(i=0;i<width*hight;i++)
	{	
	 	picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);				
		LCD_WR_DATA(picH<<8|picL);  						
	}	
	//�ָ���ʾ����Ϊȫ��	

}































