#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�������� ��������		   
//������̳:www.openedv.com
//�޸�����:2011/11/28 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
 GPIO_InitTypeDef GPIO_InitStructure;

void KEY_Init(void) //IO��ʼ��
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
	int keyValue=0;//按键值
	GPIO_Write(GPIOC,((GPIOA->ODR & 0x00ff )| 0x03C0));//让PC5-9输出高电平
		if((GPIOC->IDR & 0x001f)==0x0000)//若PC0-4全为0，则没有按键按下
		return -1;
	else
	{
		delay_ms(5);//延时消抖
		if((GPIOA->IDR & 0x001f)==0x0000)//若PC0-4全为0，则刚刚是抖动产生
		return -1;	
	}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0020);//让PA8-11输出0001，检测第四行
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=25;break;
			case 0x0001 : keyValue=24;break;
			case 0x0002 : keyValue=23;break;
			case 0x0004 : keyValue=22;break;
			case 0x0008 : keyValue=21;break;
			
		}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0040);//让PA8-11输出0001，检测第四行
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=20;break;
			case 0x0001 : keyValue=19;break;
			case 0x0002 : keyValue=18;break;
			case 0x0004 : keyValue=17;break;	
			case 0x0008 : keyValue=16;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0080);//让PA8-11输出0010，检测第三行
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=15;break;
			case 0x0001 : keyValue=14;break;
			case 0x0002 : keyValue=13;break;
			case 0x0004 : keyValue=12;break; 
			case 0x0008 : keyValue=11;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0100);//让PA8-11输出0100，检测第二行
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=10;break;
			case 0x0001 : keyValue=9;break;
			case 0x0002 : keyValue=8;break;
			case 0x0004 : keyValue=7;break; 
			case 0x0008 : keyValue=6;break;
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0200);//让PA8-11输出1000，检测第一行
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









