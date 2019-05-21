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


//设置任务堆栈大小
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define LCD_STK_SIZE     64
#define KEY_STK_SIZE     64
#define START_STK_SIZE   512
#define UART_STK_SIZE    512


//设置任务优先级
#define LED_TASK_Prio       6
#define LED1_TASK_Prio      5
#define LCD_TASK_Prio       4
#define KEY_TASK_Prio       2
#define START_TASK_Prio     10
#define UART_TASK_Prio      3

//任务堆栈
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED1_STK_SIZE];
OS_STK  TASK_LCD_STK[LCD_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_KEY_STK[KEY_STK_SIZE];
OS_STK	TASK_UART_STK[UART_STK_SIZE];

OS_EVENT *msg_key;//按键邮箱事件块指针

//任务申明
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);
void TaskLCD(void *pdata);
void TaskKey(void *pdata);
void Taskuart(void *pdata);


//加载触摸画板界面
 void Load_Drow_Dialog(void)
{   
	LCD_Fill(0, 180,240,320,WHITE); 
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(0,40,320,24,24,"testreset 13dhhhg!#$$dgh");   
    POINT_COLOR=RED;//设置画笔蓝色 
}

 int main(void)
 {
	SystemInit();   //系统初始化 72M
	delay_init(72);	     //延时初始化
	NVIC_Configuration();
	USART2_Init();
 	LED_Init();	    //LED0，LED1端口初始化
	KEY_Init();		//KEY端口初始化
	Beep_Init();
	//LCD_Init();		//LCD显示初始化	
	//LCD_Reginit(); 
	SPI_Flash_Init();
	//AT24CXX_Init();		//IIC初始化 
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
  
//开始任务
void TaskStart(void * pdata)
{
	msg_key=OSMboxCreate((void *)0);//创建消息邮箱
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
//任务1
//控制DS0的亮灭.
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
//任务2
//蜂鸣器响
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

//任务3
//按键检测  
 void TaskKey(void *pdata)
 {	
    u8 key=0;
    while(1)
	{
	  key=Key_Scan();
	  printf("got this" );
	  if(key)OSMboxPost(msg_key,(void*)key);//发送消息
	  OSTimeDlyHMSM(0,0,0,20);
	}

 }
 //任务4
 //液晶显示
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
		 if(USART_RX_STA&0x8000)	//判断是否接收完数据
		 {						
			 len=USART_RX_STA&0x3f;//得到此次接收到的数据长度	前两位为状态位 所以要与去
			 printf("\r\n您发送的消息为:\r\n\r\n");
			 for(t=0;t<len;t++) 							   //通过长度进行循环
			 {
				 USART_SendData(USART2, USART_RX_BUF[t]);	   //将数组采集好的数据 一次向串口1发送
				 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			 }
			 printf("\r\n\r\n");//插入换行
			 USART_RX_STA=0;		   //清零 虚拟寄存器 	   为一下次采集做准备
		 }else
		 {
			 delay_ms(6);	
		 }
	 }	 
	 

}

