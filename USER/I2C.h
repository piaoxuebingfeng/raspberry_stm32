 
#ifndef __I2C_H
#define __I2C_H
//#include "BSP.h"
#include "stm32f10x.h"
//============================= I2C端口定义 ===========================
#define I2C_PCLK          RCC_APB2Periph_GPIOB
#define I2C_PORT          GPIOB

#define I2C_SDA           GPIO_Pin_1
#define I2C_SCL           GPIO_Pin_0


#define I2C_SCL_H         I2C_PORT->BSRR = I2C_SCL	
#define I2C_SCL_L         I2C_PORT->BRR  = I2C_SCL 	
   
#define I2C_SDA_H         I2C_PORT->BSRR = I2C_SDA	 
#define I2C_SDA_L         I2C_PORT->BRR  = I2C_SDA	

#define READ_SDA      I2C_PORT->IDR  & I2C_SDA
//IO方向设置
#define SDA_IN()  {I2C_PORT->CRL&=0XFFFFFF0F;I2C_PORT->CRL|=8<<(4*1);}	  //PB1 
#define SDA_OUT() {I2C_PORT->CRL&=0XFFFFFF0F;I2C_PORT->CRL|=3<<(4*1);}
//===========================  函数声明   =============================
void I2C_GPIO_Config(void);			 
void I2C_Start(void);				//发送I2C开始信号
void I2C_Stop(void);	  			//发送I2C停止信号
void I2C_Send_Byte(u8 SendByte);			//I2C发送一个字节
u8 I2C_Read_Byte(u8 ack);//I2C读取一个字节
u8 I2C_Wait_Ack(void); 				//I2C等待ACK信号
void I2C_Ack(void);					//I2C发送ACK信号
void I2C_NAck(void);				//I2C不发送ACK信号
#endif  
 






