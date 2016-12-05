
#include "I2C.h"
#include "Delay.h"
#include "stm32f10x_gpio.h"
//===================================================================== 
//�� �� ��:	 I2C_GPIO_Config
//��    ��:  
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//=====================================================================
  void I2C_GPIO_Config(void)
  {
 GPIO_InitTypeDef    GPIO_InitStructure;  

  GPIO_InitStructure.GPIO_Pin = I2C_SCL;          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = I2C_SDA;           
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  }
/*
 void I2C_GPIO_Config(void)
{
	 
 	RCC->APB2ENR|=1<<3;//��ʹ������PORTBʱ��						 
	I2C_PORT->CRL&=0X00FFFFFF;//PB6 7 �������
	I2C_PORT->CRL|=0X33000000;	   
	I2C_PORT->ODR|=0X00C0;     //PB6 7 �����
    
}
*/
//===================================================================== 
//�� �� ��: I2C_Start(void)
//��    ��: ����I2C��ʼ�ź� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//=====================================================================
void I2C_Start(void)
{
  SDA_OUT();     //sda�����
	I2C_SDA_H;	  	  
	I2C_SCL_H;
	delay_us(8);
 	I2C_SDA_L;//START:when CLK is high,DATA change form high to low 
	delay_us(8);
	I2C_SCL_L;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
//===================================================================== 
//�� �� ��:	void I2C_Stop(void)
//��    ��: ����I2Cֹͣ�ź� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//=====================================================================	  
void I2C_Stop(void)
{
  SDA_OUT();//sda�����
	I2C_SCL_L;
	I2C_SDA_L;//STOP:when CLK is high DATA change form low to high
 	delay_us(8);
	I2C_SCL_H; 
	I2C_SDA_H;//����I2C���߽����ź�
	delay_us(8);			   	
}
 //===================================================================== 
//�� �� ��:	u8 I2C_Wait_Ack(void)
//��    ��: �ȴ�Ӧ���źŵ��� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: 1������Ӧ��ʧ��	 0������Ӧ��ɹ�
//=====================================================================        
u8 I2C_Wait_Ack(void)
{ 
	u8 ucErrTime=0;	
  SDA_IN();      //SDA����Ϊ���� 
	I2C_SDA_H;delay_us(1);	   
	I2C_SCL_H;delay_us(1);
 		
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_L;//ʱ�����0 	   
	return 0;  
}
//===================================================================== 
//�� �� ��:	void I2C_Ack(void)
//��    ��: ����ACKӦ�� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//===================================================================== 
void I2C_Ack(void)
{		
	I2C_SCL_L;
  SDA_OUT();
	I2C_SDA_L;
	delay_us(6);
	I2C_SCL_H;
	delay_us(6);
	I2C_SCL_L;
}
 //===================================================================== 
//�� �� ��:	void I2C_NAck(void)
//��    ��: ������ACKӦ�� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: ��
//=====================================================================		    
void I2C_NAck(void)
{			
	I2C_SCL_L;
  SDA_OUT();
	I2C_SDA_H;
	delay_us(6);
	I2C_SCL_H;
	delay_us(6);
	I2C_SCL_L;
}
//===================================================================== 
//�� �� ��:	void I2C_Send_Byte(u8 txd)
//��    ��: I2C����һ���ֽ� 
//��ڲ���: ��
//���ڲ���: ��
//�� �� ֵ: 
//=====================================================================					 				     		  
void I2C_Send_Byte(u8 SendByte)
{                        
 u8 t;   
	SDA_OUT(); 	    
    I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
      if((SendByte&0x80)>>7)
        I2C_SDA_H;  
      else 
        I2C_SDA_L;  
        SendByte<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		I2C_SCL_H;
		delay_us(2); 
		I2C_SCL_L;	
		delay_us(2);
    }	 
}
//===================================================================== 
//�� �� ��:	u8 I2C_Read_Byte(u8 ack)
//��    ��:  ��1���ֽ�
//��ڲ���:ack=1ʱ������ACK��ack=0������nACK 
//���ڲ���: ��
//�� �� ֵ: ��
//===================================================================== 	      
u8 I2C_Read_Byte(u8 ack)
{
  u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        I2C_SCL_L; 
        delay_us(2);
		    I2C_SCL_H;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        I2C_NAck();//����nACK
    else
        I2C_Ack(); //����ACK   
    return receive;
}


