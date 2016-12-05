#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DHT11 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved										  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
#define DHT11_IO_IN()  {GPIOC->CRL&=0XF0FFFFFF;GPIOC->CRL|=8<<24;}
#define DHT11_IO_OUT() {GPIOC->CRL&=0XF0FFFFFF;GPIOC->CRL|=3<<24;}
////IO��������											   
#define	DHT11_DQ_OUT PCout(6) //���ݶ˿�	PC6 
#define	DHT11_DQ_IN  PCin(6)  //���ݶ˿�	PC6





u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
u8 th_read(u8 th);
#endif















