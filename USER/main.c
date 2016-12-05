#include "stm32f10x.h"
#include "delay.h"
#include "rtc.h"
#include "usart.h"
#include "sys.h"
#include "usmart.h"
#include "dht11.h"
#include "ds18b20.h"
#include "I2C.h"
#include "SI7021.h"

void GPIO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	
	// GPIO RELAY Init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  // RELAY0 | TIM3 CH3 | IIC_SCL
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  // RELAY1 | TIM3 CH4 | IIC_SDA
	
	GPIO_SetBits(GPIOB,GPIO_Pin_3);  //RELAY4  
	GPIO_SetBits(GPIOB,GPIO_Pin_4);  //RELAY5  
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);  //RELAY6
	GPIO_SetBits(GPIOB,GPIO_Pin_8);  //RELAY7   TIM4 CH3
	GPIO_SetBits(GPIOB,GPIO_Pin_9);  //RELAY8   TIM4 CH4
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_12);  //RELAY2
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);  //RELAY3
	
	
}
void LED_test(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	delay_ms(200);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	delay_ms(200);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	delay_ms(200);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	delay_ms(200);
}
void Hardware_check()
{
		if(0!=RTC_Init())		//RTC��ʼ��	��һ��Ҫ��ʼ���ɹ�
	{ 
		printf("RTC ERROR! \r\n");
		delay_ms(800);
		printf("RTC Trying...\r\n");
	}
	else if(0==RTC_Init())
		LED_test();
	if(0!=DHT11_Init())	//DHT11��ʼ��	
	{
		printf("DHT11 Error!\r\n");
		delay_ms(200);
	}
	else if(0==DHT11_Init())
		LED_test();
	if(0!=DS18B20_Init())	//DS18B20��ʼ��	
	{
		printf("DS18B20 Error!\r\n");
		delay_ms(200);
	}
	else if(0==DS18B20_Init())
		LED_test();
}
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
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
int main(void)
{	
	u8 t,len;
	u8 temperature;  	    
	u8 humidity;
	u16 temp,humi;
	
	NVIC_Configuration();	 
	GPIO_init();
	// iic init
	I2C_GPIO_Config();
	delay_init();
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	uart2_init(9600);
	usmart_dev.init(72); 	//��ʼ��USMART	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	//USART_printf(USART2,"AT+CWMODE=2\r\n");// APģʽ
	Hardware_check();
	DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��ֵ	
	delay_ms(100);
	printf("temperature:%d\r\n",temperature);
	printf("humidity:%d\r\n",humidity);
	
	//����SI7021ϵ����ʪ�ȴ������ɼ�
	if(SI7021_Read( &temp, &humi ))
		{
		printf( "�¶ȣ�%.1f ʪ�ȣ�%.1f%%\r\n", temp / 10.0, humi/ 10.0 );
		}
		else
		{
			 printf( "read SI7021 ERROR\r\n");
		}
	while(1)
	{	
		usmart_dev.scan();
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("%d year %d month %d date\r\n",calendar.w_year,calendar.w_month,calendar.w_date);
			switch(calendar.week)
			{
				case 0:
					printf("Sunday\r\n");
					break;
				case 1:
					printf("Monday\r\n");
					break;
				case 2:
					printf("Tuesday\r\n");
					break;
				case 3:
					printf("Wednesday\r\n");
					break;
				case 4:
					printf("Thursday\r\n");
					break;
				case 5:
					printf("Friday\r\n");
					break;
				case 6:
					printf("Saturday\r\n");
					break;  
			}
			printf("%d hour %d min %d sec\r\n",calendar.hour,calendar.min,calendar.sec);
			if(calendar.hour==21 && calendar.min==19 && calendar.sec==30)
			{
				USART_printf(USART2,"time correct request\r\n");
			}
		}	
		delay_ms(10);	
	} 
}

