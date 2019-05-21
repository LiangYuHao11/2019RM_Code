/**
  ******************************************************************************
  * @file    ANO.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   
  ******************************************************************************
  */
	
	
#include "ANO.h"

int8_t send_buf[15]= {0};
int8_t send_bufs[30]= {0};
int16_t Temp_Target[8] = {0};

void ANO_Send_Data_Init(int16_t Target1,int16_t Target2,int16_t Target3,int16_t Target4,\
	int16_t Target5,int16_t Target6,int16_t Target7,int16_t Target8)
{
  Temp_Target[0] = Target1;
  Temp_Target[1] = Target2;
  Temp_Target[2] = Target3;
  Temp_Target[3] = Target4;
  Temp_Target[4] = Target5;
  Temp_Target[5] = Target6;
  Temp_Target[6] = Target7;
  Temp_Target[7] = Target8;

}

void ANO_Send_Data_V4(void)
{
		
  int8_t data_sum=0;
	int i=0,cout=0;
	send_bufs[cout++]=0xAA;                   
	send_bufs[cout++]=0x01;                 
	send_bufs[cout++]=0xAF;                  
	send_bufs[cout++]=0xF1;                  
	send_bufs[cout++]=0;                    
	send_bufs[cout++]=Temp_Target[0]>>8;       
	send_bufs[cout++]=Temp_Target[0];         
	send_bufs[cout++]=Temp_Target[1]>>8;       
	send_bufs[cout++]=Temp_Target[1];            
	send_bufs[cout++]=Temp_Target[2]>>8;      
	send_bufs[cout++]=Temp_Target[2];          
	send_bufs[cout++]=Temp_Target[3]>>8;          
	send_bufs[cout++]=Temp_Target[3];              
	send_bufs[cout++]=Temp_Target[4]>>8;           
	send_bufs[cout++]=Temp_Target[4];           
	send_bufs[cout++]=Temp_Target[5]>>8;         
	send_bufs[cout++]=Temp_Target[5];             
	send_bufs[cout++]=Temp_Target[6]>>8;         
	send_bufs[cout++]=Temp_Target[6];             
  send_bufs[cout++]=Temp_Target[7]>>8;          
	send_bufs[cout++]=Temp_Target[7];    
	
	send_bufs[4]=cout-5;
	for(i=0;i<cout;i++)
	{
	data_sum+=send_bufs[i];
	}
	send_bufs[cout++]=data_sum;
	
	for(i=0;i<cout;i++)
	{
	USART_sendChar(USART2,send_bufs[i]);
	}
}





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


void ANO_Send_Data_V1(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
	int8_t data_sum=0;
	int i=0;
	send_buf[0]=0xAA;
	send_buf[1]=0x01;
	send_buf[2]=0xAF;
	send_buf[3]=0x02;
	send_buf[4]=0x08;
	send_buf[5]=Temp_Target1>>8;
	send_buf[6]=Temp_Target1;
	send_buf[7]=Temp_Now1>>8;
	send_buf[8]=Temp_Now1;
	send_buf[9]=Temp_Target2>>8;
	send_buf[10]=Temp_Target2;
	send_buf[11]=Temp_Now2>>8;
	send_buf[12]=Temp_Now2;
	for(i=0;i<13;i++)
	{
	data_sum+=send_buf[i];
	}
	send_buf[13]=data_sum;
	for(i=0;i<14;i++)
	{
	USART_sendChar(USART2,send_buf[i]);
	}

}




void ANO_Send_Data_V2(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
	int8_t data_sum=0;
	int i=0,cout=0;
	send_buf[cout++]=0xAA;
	send_buf[cout++]=0x01;
	send_buf[cout++]=0xAF;
	send_buf[cout++]=0x02;
	send_buf[cout++]=0;
	send_buf[cout++]=Temp_Target1>>8;
	send_buf[cout++]=Temp_Target1;
	send_buf[cout++]=Temp_Now1>>8;
	send_buf[cout++]=Temp_Now1;
	send_buf[cout++]=Temp_Target2>>8;
	send_buf[cout++]=Temp_Target2;
	send_buf[cout++]=Temp_Now2>>8;
	send_buf[cout++]=Temp_Now2;
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


void ANO_GPIO_Init(void)
{
// 	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;    
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOD,GPIO_Pin_14);
	
	
}



