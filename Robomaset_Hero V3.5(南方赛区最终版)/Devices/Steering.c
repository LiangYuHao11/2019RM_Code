/**
  ******************************************************************************
  * @file    Steering.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   舵机初始化
  ******************************************************************************
  */
	
#include "Steering.h"

/**
* @brief  舵机初始化
* @param  None
* @retval None
*/
void Steering_Init(void){
	
	/*引脚初始化*/
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TIM4_CH3_GPIO_CLK , ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	
	GPIO_InitStructure.GPIO_Pin = TIM4_CH3_Pin;
	GPIO_Init(TIM4_CH3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM4_CH3_GPIO_PORT,TIM4_CH3_PINSOURCE, GPIO_AF_TIM4);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
	
	/* 配置时基结构体 */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//配置定时器时钟分频   TIM12_PWMOutput(89, 19999,1000);
	TIM_TimeBaseStructure.TIM_Prescaler = 89;
	//配置自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Period = 19999;
	//配置死区时间与采样滤波分频，不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//配置计数模式，向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/*配置输出比较结构体*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//配置输出模式，PWM1模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//使能输出
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//不使能互补输出
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	//设置初始占空比
	TIM_OCInitStructure.TIM_Pulse = 2000;
	//设置PWM极性，初始电平为高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//设置空闲时输出电平
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	//设置互补输出PWM极性，初始电平为高(不用也要配置，否则输出可能出错)
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	//设置互补输出空闲时输出电平(不用也要配置，否则输出可能出错)
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	

	//初始化为对应通道的输出，通道1
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	//使能定时器
	TIM_Cmd(TIM4, ENABLE);
}



