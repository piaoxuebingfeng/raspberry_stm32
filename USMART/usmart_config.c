#include "usmart.h"
#include "usmart_str.h"
////////////////////////////�û�������///////////////////////////////////////////////
//������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����) 
#include "delay.h"		
#include "sys.h"
#include "dht11.h"
#include "rtc.h"
#include "ds18b20.h"			
#define OUTPUT 1
#define INPUT  2
#define PWM    3
void PWM_set_compare(u8 GPIO_PIN,uint16_t value)
{
	switch(GPIO_PIN)
	{
		case 9:
			TIM_SetCompare3(TIM3,value);
			break;
		case 10:
			TIM_SetCompare4(TIM3,value);
			break;
		case 16:
			TIM_SetCompare3(TIM4,value);
			break;
		case 17:
			TIM_SetCompare4(TIM4,value);
			break;
	}
}
int GPIO_set_mode(u8 GPIO_PIN,u8 mode)// OUTPUT INPUT PWM
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	switch(GPIO_PIN)
	{
		case 9 :
			if(mode==PWM)
			{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
		      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3  PB0 ��Ӧ��ʱ��3��ͨ��3
	        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	        GPIO_Init(GPIOB, &GPIO_InitStructure);
				
			    TIM_TimeBaseStructure.TIM_Period = 71; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	        TIM_TimeBaseStructure.TIM_Prescaler =10; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
        	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
				
          TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
          TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE �����ʹ��	
	        TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	        TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
			}
			else if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_0);  //RELAY0
			}
			break;
		case 10:
			if(mode==PWM)
			{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
		      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM3_CH4  PB0 ��Ӧ��ʱ��3��ͨ��4
	        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	        GPIO_Init(GPIOB, &GPIO_InitStructure);
				
			    TIM_TimeBaseStructure.TIM_Period = 71; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	        TIM_TimeBaseStructure.TIM_Prescaler =10; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
        	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
				
          TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
          TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE �����ʹ��	
	        TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	        TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
			}
			else if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);  //RELAY1
			}
			break;
		case 11:
			if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOC, &GPIO_InitStructure);
				GPIO_SetBits(GPIOC,GPIO_Pin_12);  //RELAY2
			}
			break;
		case 12:
			if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOD, &GPIO_InitStructure);
				GPIO_SetBits(GPIOC,GPIO_Pin_2);  //RELAY3
			}
			break;
		case 13:
			if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_3);  //RELAY4
			}
			break;
		case 14:
			if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_4);  //RELAY5
			}
			break;
		case 15:
			if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_5);  //RELAY6
			}
			break;
		case 16:
			if(mode==PWM)
			{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
		      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM4_CH3  PB8 ��Ӧ��ʱ��4��ͨ��3
	        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	        GPIO_Init(GPIOB, &GPIO_InitStructure);
				
			    TIM_TimeBaseStructure.TIM_Period = 71; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	        TIM_TimeBaseStructure.TIM_Prescaler =10; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
        	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
				
          TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
          TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE �����ʹ��	
	        TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH3Ԥװ��ʹ��	 
	        TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	        TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
			}
			else if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_8);  //RELAY7
			}
			break;
		case 17:
						if(mode==PWM)
			{
					RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
		      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM4_CH4  PB9 ��Ӧ��ʱ��4��ͨ��3
	        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	        GPIO_Init(GPIOB, &GPIO_InitStructure);
				
			    TIM_TimeBaseStructure.TIM_Period = 71; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	        TIM_TimeBaseStructure.TIM_Prescaler =10; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
        	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
				
          TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
        	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
        	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
          TIM_CtrlPWMOutputs(TIM4,ENABLE);	//MOE �����ʹ��	
	        TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	        TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	        TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
			}
			else if(mode==OUTPUT)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
				GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_SetBits(GPIOB,GPIO_Pin_9);  //RELAY8
			}
			break;
	}
	return 0;
}
//	+----------------------GPIO----------------+------------------GPIO---------------0--------+
//	|- DEF|----------------ADC-----------------+------------------RELAY-----------------------|
//	|- PIN|0   1   2   3   4   5   6   7   8   +   9   10   11   12   13   14   15   16   17--|
//                                                 B0  B1   C12  D2   B3   B4   B5   B8   B9
//	|-MODE|------------------------------------+----------------------------------------------|
//	|-VLUE|------------------------------------+----------------------------------------------|
//	+----------------------GND-----------------+------------------GND-------------------------+
int GPIO_Read_All(u8 value)
{
	int v0,v1,v2,v3,v4,v5,v6,v7,v8;
	v0=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
	v1=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1);
	v2=GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12);
	v3=GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2);
	v4=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3);
	v5=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4);
	v6=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5);
	v7=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8);
	v8=GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9);
	USART_printf(USART2,"%d%d%d%d%d%d%d%d%d\r\n",v0,v1,v2,v3,v5,v6,v7,v8);
	return 0;
}
int GPIO_Read(u8 GPIO_PIN)
{
	switch(GPIO_PIN)
	{
		case 9 :
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0))
			{
				USART_printf(USART2,"GPIO_9: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_9: 0\r\n");
			}
			break;
		case 10:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
			{
				USART_printf(USART2,"GPIO_10: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_10: 0\r\n");
			}
			break;
		case 11:
			if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12))
			{
				USART_printf(USART2,"GPIO_11: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_11: 0\r\n");
			}
			break;
		case 12:
			if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2))
			{
				USART_printf(USART2,"GPIO_12: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_12: 0\r\n");
			}
			break;
		case 13:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_3))
			{
				USART_printf(USART2,"GPIO_13: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_13: 0\r\n");
			}
			break;
		case 14:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4))
			{
				USART_printf(USART2,"GPIO_14: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_14: 0\r\n");
			}
			break;
		case 15:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5))
			{
				USART_printf(USART2,"GPIO_15: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_15: 0\r\n");
			}
			break;
		case 16:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))
			{
				USART_printf(USART2,"GPIO_16: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_16: 0\r\n");
			}
			break;
		case 17:
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9))
			{
				USART_printf(USART2,"GPIO_17: 1\r\n");
			}
			else
			{
				USART_printf(USART2,"GPIO_17: 0\r\n");
			}
			break;
		default:
			break;
	}
	return 0;
}
void GPIO_SetBit(u8 GPIO_PIN,u8 value)
{
		switch(GPIO_PIN)
	{
		case 9 :
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			break;
		case 10:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);
			break;
		case 11:
			if(value)
				GPIO_SetBits(GPIOC,GPIO_Pin_12);
			else
				GPIO_ResetBits(GPIOC,GPIO_Pin_12);
			break;
		case 12:
			if(value)
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
			else
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			break;
		case 13:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_3);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_3);
			break;
		case 14:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_4);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
			break;
		case 15:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_5);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			break;
		case 16:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_8);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_8);
			break;
		case 17:
			if(value)
				GPIO_SetBits(GPIOB,GPIO_Pin_9);
			else
				GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			break;
		default:
			break;
	}
}
void led_set(u8 sta)
{
	if(sta>0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	else if(sta==0)
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void test_fun(void(*ledset)(u8),u8 sta)
{
	ledset(sta);
}
void get_ds18b20_tem(u8 null)
{
	//static u8 DS18B20_check_flag=0;
	u8 tmp;
	short temperature;	
//	if(!DS18B20_check_flag)
//	{
		tmp=DS18B20_Init();
		if(0!=tmp)
		{
		USART_printf(USART2,"DS18B20 not found,please check the hardware!\r\n");	
		return ;
		}
//		else if(0 == tmp)
//			DS18B20_check_flag=1;
//	}
	temperature=DS18B20_Get_Temp();	
		if(temperature<0)
		{
			USART_printf(USART2,"-");
			temperature=-temperature;					//תΪ����
		}else
		USART_printf(USART2,"%d",temperature/10);	//��ʾ��������
		USART_printf(USART2,".");
		USART_printf(USART2,"%d\r\n",temperature%10);	//��ʾС������ 		   		
}
void get_date(u8 null)
{
			u8 t;
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			USART_printf(USART2,"%d year %d month %d date  ",calendar.w_year,calendar.w_month,calendar.w_date);
			switch(calendar.week)
			{
				case 0:
					USART_printf(USART2,"Sunday  ");
					break;
				case 1:
					USART_printf(USART2,"Monday  ");
					break;
				case 2:
					USART_printf(USART2,"Tuesday  ");
					break;
				case 3:
					USART_printf(USART2,"Wednesday  ");
					break;
				case 4:
					USART_printf(USART2,"Thursday  ");
					break;
				case 5:
					USART_printf(USART2,"Friday  ");
					break;
				case 6:
					USART_printf(USART2,"Saturday  ");
					break;  
			}
			USART_printf(USART2,"%d hour %d min %d sec\r\n",calendar.hour,calendar.min,calendar.sec);
		}	
}
//�������б��ʼ��(�û��Լ����)
//�û�ֱ������������Ҫִ�еĺ�����������Ҵ�
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//���ʹ���˶�д����
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif	
	(void*)th_read,"u8 th_read(u8 th)",
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",
	(void*)led_set,"void led_set(u8 sta)",
	(void*)test_fun,"void test_fun(void(*ledset)(u8),u8 sta)",
	(void*)RTC_Set,"u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)",
	(void*)get_date,"void get_date(u8 null)",
	(void*)get_ds18b20_tem,"void get_ds18b20_tem(u8 null)",
	(void*)GPIO_Read,"GPIO_Read(u8 GPIO_PIN)",
  (void*)GPIO_SetBit,"GPIO_SetBit(u8 GPIO_PIN,u8 value)",	
	(void*)GPIO_Read_All,"GPIO_Read_All(u8 value)",
	(void*)GPIO_set_mode,"GPIO_set_mode(u8 GPIO_PIN,u8 mode)",
	(void*)PWM_set_compare,"PWM_set_compare(u8 GPIO_PIN,uint16_t value)",
};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������ƹ�������ʼ��
//�õ������ܿغ���������
//�õ�����������
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//��������
	0,	  	//��������
	0,	 	//����ID
	1,		//������ʾ����,0,10����;1,16����
	0,		//��������.bitx:,0,����;1,�ַ���	    
	0,	  	//ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ��
	0,		//�����Ĳ���,��ҪPARM_LEN��0��ʼ��
};   



















