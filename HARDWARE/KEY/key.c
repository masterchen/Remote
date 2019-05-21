#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//Mini STM32¿ª·¢°å
//°´¼üÊäÈë Çý¶¯´úÂë		   
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2011/11/28 
//°æ±¾£ºV1.1
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
 GPIO_InitTypeDef GPIO_InitStructure;

void KEY_Init(void) //IO³õÊ¼»¯
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
	int keyValue=0;//æŒ‰é”®å€¼
	GPIO_Write(GPIOC,((GPIOA->ODR & 0x00ff )| 0x03C0));//è®©PC5-9è¾“å‡ºé«˜ç”µå¹³
		if((GPIOC->IDR & 0x001f)==0x0000)//è‹¥PC0-4å…¨ä¸º0ï¼Œåˆ™æ²¡æœ‰æŒ‰é”®æŒ‰ä¸‹
		return -1;
	else
	{
		delay_ms(5);//å»¶æ—¶æ¶ˆæŠ–
		if((GPIOA->IDR & 0x001f)==0x0000)//è‹¥PC0-4å…¨ä¸º0ï¼Œåˆ™åˆšåˆšæ˜¯æŠ–åŠ¨äº§ç”Ÿ
		return -1;	
	}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0020);//è®©PA8-11è¾“å‡º0001ï¼Œæ£€æµ‹ç¬¬å››è¡Œ
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=25;break;
			case 0x0001 : keyValue=24;break;
			case 0x0002 : keyValue=23;break;
			case 0x0004 : keyValue=22;break;
			case 0x0008 : keyValue=21;break;
			
		}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0040);//è®©PA8-11è¾“å‡º0001ï¼Œæ£€æµ‹ç¬¬å››è¡Œ
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=20;break;
			case 0x0001 : keyValue=19;break;
			case 0x0002 : keyValue=18;break;
			case 0x0004 : keyValue=17;break;	
			case 0x0008 : keyValue=16;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0080);//è®©PA8-11è¾“å‡º0010ï¼Œæ£€æµ‹ç¬¬ä¸‰è¡Œ
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=15;break;
			case 0x0001 : keyValue=14;break;
			case 0x0002 : keyValue=13;break;
			case 0x0004 : keyValue=12;break; 
			case 0x0008 : keyValue=11;break;	
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0100);//è®©PA8-11è¾“å‡º0100ï¼Œæ£€æµ‹ç¬¬äºŒè¡Œ
	  switch(GPIOB->IDR & 0x001f)
		{
			case 0x0000 : keyValue=10;break;
			case 0x0001 : keyValue=9;break;
			case 0x0002 : keyValue=8;break;
			case 0x0004 : keyValue=7;break; 
			case 0x0008 : keyValue=6;break;
		}
	GPIO_Write(GPIOA,(GPIOA->ODR & 0xfc1f )| 0x0200);//è®©PA8-11è¾“å‡º1000ï¼Œæ£€æµ‹ç¬¬ä¸€è¡Œ
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









