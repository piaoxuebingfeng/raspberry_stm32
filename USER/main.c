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
		if(0!=RTC_Init())		//RTC初始化	，一定要初始化成功
	{ 
		printf("RTC ERROR! \r\n");
		delay_ms(800);
		printf("RTC Trying...\r\n");
	}
	else if(0==RTC_Init())
		LED_test();
	if(0!=DHT11_Init())	//DHT11初始化	
	{
		printf("DHT11 Error!\r\n");
		delay_ms(200);
	}
	else if(0==DHT11_Init())
		LED_test();
	if(0!=DS18B20_Init())	//DS18B20初始化	
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx


	TIM_CtrlPWMOutputs(TIM3,ENABLE);	//MOE 主输出使能	
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
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
	uart_init(9600);	 	//串口初始化为9600
	uart2_init(9600);
	usmart_dev.init(72); 	//初始化USMART	
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	//USART_printf(USART2,"AT+CWMODE=2\r\n");// AP模式
	Hardware_check();
	DHT11_Read_Data(&temperature,&humidity);		//读取温湿度值	
	delay_ms(100);
	printf("temperature:%d\r\n",temperature);
	printf("humidity:%d\r\n",humidity);
	
	//新增SI7021系列温湿度传感器采集
	if(SI7021_Read( &temp, &humi ))
		{
		printf( "温度：%.1f 湿度：%.1f%%\r\n", temp / 10.0, humi/ 10.0 );
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

