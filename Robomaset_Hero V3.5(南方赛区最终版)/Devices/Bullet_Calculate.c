/**
  ******************************************************************************
  * @file    Bullet_Calculate.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   ���跢������ƺ����ӿ�
  ******************************************************************************
  */

#include "Bullet_Calculate.h"

Bullet_t Bullet;


/**
  * @brief  ���ö�ȡ���㵯���IO��
  * @param  None
  * @retval None
  */
void Bullet_Calculate_GPIOInit(void)
{
	
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;    
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}



















