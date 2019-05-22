/**
  ******************************************************************************
  * @file    Steering.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   �����ʼ��
  ******************************************************************************
  */
	
#include "Steering.h"

/**
* @brief  �����ʼ��
* @param  None
* @retval None
*/
void Steering_Init(void){
	
	/*���ų�ʼ��*/
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
  
	
	/* ����ʱ���ṹ�� */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//���ö�ʱ��ʱ�ӷ�Ƶ   TIM12_PWMOutput(89, 19999,1000);
	TIM_TimeBaseStructure.TIM_Prescaler = 89;
	//�����Զ���װ�ؼĴ�����ֵ
	TIM_TimeBaseStructure.TIM_Period = 19999;
	//��������ʱ��������˲���Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//���ü���ģʽ�����ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/*��������ȽϽṹ��*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//�������ģʽ��PWM1ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//ʹ�����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//��ʹ�ܻ������
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	//���ó�ʼռ�ձ�
	TIM_OCInitStructure.TIM_Pulse = 2000;
	//����PWM���ԣ���ʼ��ƽΪ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//���ÿ���ʱ�����ƽ
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	
	//���û������PWM���ԣ���ʼ��ƽΪ��(����ҲҪ���ã�����������ܳ���)
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	//���û����������ʱ�����ƽ(����ҲҪ���ã�����������ܳ���)
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	

	//��ʼ��Ϊ��Ӧͨ���������ͨ��1
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	//ʹ�ܶ�ʱ��
	TIM_Cmd(TIM4, ENABLE);
}



