#include "stm32f10x.h"
#include "led.h"

void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//使能PA端口时钟
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_6;	    		 //LED1-->PE.5 端口配置, 推挽输出
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 			 
	 //GPIO_SetBits(GPIOE,GPIO_Pin_1); 			
	 //GPIO_SetBits(GPIOE,GPIO_Pin_6); 			 
}

void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 				//LED1-->PE.5 端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//推挽输出 ，IO口速度为50MHz
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);	   

}
 
