/**
  ******************************************************************************
  * @file    ANO.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   ������λ��
  ******************************************************************************
  */
	
	
#include "ANO.h"

int8_t send_buf[15]= {0};


/**
* @brief  ������λ�����ͺ���
  * @param  None 	
  * @retval None
  */
void ANO_Send_Data_V3(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
  int8_t data_sum=0;
	int i=0,cout=0;
	send_buf[cout++]=0xAA;                   //0
	send_buf[cout++]=0x01;                   //1
	send_buf[cout++]=0xAF;                   //2
	send_buf[cout++]=0xF1;                   //3
	send_buf[cout++]=0;                      //4
	send_buf[cout++]=Temp_Target1>>8;        //5
	send_buf[cout++]=Temp_Target1;           //6
	send_buf[cout++]=Temp_Now1>>8;           //7
	send_buf[cout++]=Temp_Now1;              //8
	send_buf[cout++]=Temp_Target2>>8;        //9
	send_buf[cout++]=Temp_Target2;           //10
	send_buf[cout++]=Temp_Now2>>8;           //11
	send_buf[cout++]=Temp_Now2;              //12
	send_buf[4]=cout-5;
	for(i=0;i<cout;i++)
	{
	data_sum+=send_buf[i];
	}
	send_buf[cout++]=data_sum;
	
	for(i=0;i<cout;i++)
	{
	USART_sendChar(USART2,send_buf[i]);
	}
}





