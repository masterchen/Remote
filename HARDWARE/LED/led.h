#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"


//LED�˿ڶ���
#define LED0 PAout(8)// PA8
#define LED1 PDout(2)// PD2	

void LED_Init(void);//��ʼ��
void Beep_Init(void);

		 				    
#endif