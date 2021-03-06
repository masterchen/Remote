#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//按键输入 驱动代码		   
//技术论坛:www.openedv.com
//修改日期:2011/11/28 
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
 GPIO_InitTypeDef GPIO_InitStructure;

void KEY_Init(void) //IO初始化
{
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
		 
}

	
int Key_Scan(void)
{
	int keyValue=0;//鎸夐敭鍊�
	GPIO_Write(GPIOC,((GPIOA->ODR & 0x00ff )| 0x03C0));//璁㏄C5-9杈撳嚭楂樼數骞�
		if((GPIOC->IDR & 0x001f)==0x0000)//鑻C0-4鍏ㄤ负0锛屽垯娌℃湁鎸夐敭鎸変笅
		return -1;
	else
	{
		delay_ms(5);//寤舵椂娑堟姈
		if((GPIOA->IDR & 0x001f)==0x0000)//鑻C0-4鍏ㄤ负0锛屽垯鍒氬垰鏄姈鍔ㄤ骇鐢�
		return -1;	
	}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0020);//璁㏄A8-11杈撳嚭0001锛屾娴嬬鍥涜
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=25;break;
			case 0x0001 : keyValue=24;break;
			case 0x0002 : keyValue=23;break;
			case 0x0004 : keyValue=22;break;
			case 0x0008 : keyValue=21;break;
			
		}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0040);//璁㏄A8-11杈撳嚭0001锛屾娴嬬鍥涜
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=20;break;
			case 0x0001 : keyValue=19;break;
			case 0x0002 : keyValue=18;break;
			case 0x0004 : keyValue=17;break;	
			case 0x0008 : keyValue=16;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0080);//璁㏄A8-11杈撳嚭0010锛屾娴嬬涓夎
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=15;break;
			case 0x0001 : keyValue=14;break;
			case 0x0002 : keyValue=13;break;
			case 0x0004 : keyValue=12;break; 
			case 0x0008 : keyValue=11;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0100);//璁㏄A8-11杈撳嚭0100锛屾娴嬬浜岃
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=10;break;
			case 0x0001 : keyValue=9;break;
			case 0x0002 : keyValue=8;break;
			case 0x0004 : keyValue=7;break; 
			case 0x0008 : keyValue=6;break;
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0200);//璁㏄A8-11杈撳嚭1000锛屾娴嬬涓�琛�
	  switch(GPIOB->IDR & 0x001f)
		{
		    case 0x0000 : keyValue=5;break;
			case 0x0001 : keyValue=4;break;
			case 0x0002 : keyValue=3;break;
			case 0x0004 : keyValue=2;break;
			case 0x0008 : keyValue=1;break; 
		}	
	  
	return keyValue;

}









