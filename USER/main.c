#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "spi.h"
#include "flash.h"
#include "stdio.h"
#include "string.h"
#include "includes.h"
#include "gui.h"


//���������ջ��С
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define START_STK_SIZE   512
#define UART_STK_SIZE    512


//�����������ȼ�
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define START_TASK_Prio     10
#define UART_TASK_Prio      3

//�����ջ
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK	TASK_UART_STK[UART_STK_SIZE];

OS_EVENT *msg_key;//���������¼���ָ��

//��������
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void Taskuart(void *pdata);


//���ش����������
 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
	LCD_ShowString(0,40,320,24,24,"testreset 13dhhhg!#$$dgh");   
    POINT_COLOR=RED;//���û�����ɫ 
}

 int main(void)
 {
	SystemInit();   //ϵͳ��ʼ�� 72M
	delay_init(72);	     //��ʱ��ʼ��
	NVIC_Configuration();
	USART2_Init();
 	LED_Init();	    //LED0��LED1�˿ڳ�ʼ��
	KEY_Init();		//KEY�˿ڳ�ʼ��
	Beep_Init();
	//LCD_Init();		//LCD��ʾ��ʼ��	
	//LCD_Reginit(); 
	SPI_Flash_Init();
	//AT24CXX_Init();		//IIC��ʼ�� 
	delay_ms(500);
	//Load_Drow_Dialog();
	OSInit();
	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	OSStart();
	return 0;

 }	
  
//��ʼ����
void TaskStart(void * pdata)
{
	msg_key=OSMboxCreate((void *)0);//������Ϣ����
	pdata = pdata; 	//
	OS_ENTER_CRITICAL();   
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	//OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
 	//OSTaskCreate(TaskLCD, (void * )0, (OS_STK *)&TASK_LCD_STK[LCD_STK_SIZE-1], LCD_TASK_Prio);
  	//OSTaskCreate(TaskKey, (void * )0, (OS_STK *)&TASK_KEY_STK[KEY_STK_SIZE-1], KEY_TASK_Prio);
  	OSTaskCreate(Taskuart, (void * )0, (OS_STK *)&TASK_UART_STK[UART_STK_SIZE-1], UART_TASK_Prio);
 	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//����1
//����DS0������.
void TaskLed(void *pdata)
{
	while(1)
	{
	//	LED0=!LED0;
	GPIO_ResetBits(GPIOE,GPIO_Pin_6); 		
	OSTimeDlyHMSM(0,0,3,0);	
    GPIO_SetBits(GPIOE,GPIO_Pin_6); 			
	OSTimeDlyHMSM(0,0,3,0);		
    GPIO_ResetBits(GPIOE,GPIO_Pin_6); 				
	OSTimeDlyHMSM(0,0,3,0);	
    GPIO_SetBits(GPIOE,GPIO_Pin_6); 			
	OSTimeDlyHMSM(0,0,3,0);	
	}
}
//����2
//��������
void TaskLed1(void *pdata)
{
	while(1)
	{     
	GPIO_SetBits(GPIOE,GPIO_Pin_4); 			
	OSTimeDlyHMSM(0,0,3,0);	
	GPIO_ResetBits(GPIOE,GPIO_Pin_4); 	
	OSTimeDlyHMSM(0,0,3,0);	
	}
}

//����3
//�������  
 void TaskKey(void *pdata)
 {	
    u8 key=0;
    while(1)
	{
	  key=Key_Scan();
	  printf("got this" );
	  if(key)OSMboxPost(msg_key,(void*)key);//������Ϣ
	  OSTimeDlyHMSM(0,0,0,20);
	}

 }
 //����4
 //Һ����ʾ
 void TaskLCD(void *pdata)
 {
 	 u32 key=0;
	 u8 err;	 
	 u8 wakeup_readback[21632] = {0};	 
	 u32 t=21632;
	 while (1)
	 {
		key=(u32)OSMboxPend(msg_key,10,&err);
     	switch(key)
 		{
 			case 1:				
				LCD_Fill(80,40,160,120,YELLOW);
				OSTimeDlyHMSM(0,0,0,400);			
				SPI_Flash_Read(wakeup_readback,4*t,21632);				
				Gui_Drawbmp16WH(50,100,104,104,wakeup_readback);
				break;
			case 2:
				LCD_Fill(80,40,160,120,RED);
				OSTimeDlyHMSM(0,0,0,400);
				break;
			case 3:
				LCD_Fill(80,40,160,120,GREEN);
				OSTimeDlyHMSM(0,0,0,400);
				break;
 		}
	 }
		   
 }	

void Taskuart(void *pdata)
{
	 u8 t;
	 u8 len; 
	 while(1)
	 {
		 if(USART_RX_STA&0x8000)	//�ж��Ƿ����������
		 {						
			 len=USART_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���	ǰ��λΪ״̬λ ����Ҫ��ȥ
			 printf("\r\n�����͵���ϢΪ:\r\n\r\n");
			 for(t=0;t<len;t++) 							   //ͨ�����Ƚ���ѭ��
			 {
				 USART_SendData(USART2, USART_RX_BUF[t]);	   //������ɼ��õ����� һ���򴮿�1����
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			 }
			 printf("\r\n\r\n");//���뻻��
			 USART_RX_STA=0;		   //���� ����Ĵ��� 	   Ϊһ�´βɼ���׼��
		 }else
		 {
			 delay_ms(6);	
		 }
	 }	 
	 

}

