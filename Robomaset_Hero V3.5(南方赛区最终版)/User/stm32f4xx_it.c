/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "user_common.h"


#define KEY_ACC_TIME     500  //ms


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/



/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
   /**/
}

void CAN1_RX0_IRQHandler(void){
	//接收中断
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0)){
		CanRxMsg RxMessage;	
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		Clound_Motor_getInfo(RxMessage);	
		/*printf("id %x, data %x, T%x, IL%x, IH%x, SL%x, SH%x, AL%x, AH%x\n", RxMessage.StdId, RxMessage.Data[7],
																				RxMessage.Data[6],
																				RxMessage.Data[5],
																				RxMessage.Data[4],
																				RxMessage.Data[3],
																				RxMessage.Data[2],
																				RxMessage.Data[1],
																				RxMessage.Data[0]);*/
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
	}
	
}

void CAN2_RX0_IRQHandler(void){
	//接收中断
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP0)){
		CanRxMsg RxMessage;	
		CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
    M3508_getInfo(RxMessage); 		
		GY6050_getCloundInfo(RxMessage);
		SCCM_getInfo(RxMessage);			
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
	}
	
}

/**
  * @brief  USART1中断服务函数，DR遥控器
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void){
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		//关闭DMA
		DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
		
		//printf("%d\n",DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM));
		//获取DMAbuff剩余大小，是否匹配
		if (DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM) == 2)
		{
			//printf("%s\n", DR16_rxBuff);
			
			DR16_dataProcess(DR16_rxBuff);
		}
		
		//设置传输数据长度，+2保证安全
    DMA_SetCurrDataCounter(USART1_RX_DMA_STREAM,DR16_DBUS_PACKSIZE+2);
		//打开DMA
		DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
		
		
		//清除空闲中断标志位
		(void)USART1->DR;
		(void)USART1->SR;
		//串口空闲中断标志位并不能通过下面的函数清除
		//USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}

uint16_t DMA_Counter;
void USART6_IRQHandler(void){
	
	if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
	{
		//关闭DMA
		DMA_Cmd(USART6_RX_DMA_STREAM, DISABLE);
				
		DMA_Counter = DMA_GetCurrDataCounter(USART6_RX_DMA_STREAM);
		
		Judge_getInfo(JUDGESYSTEM_PACKSIZE-DMA_Counter);
		
		//设置传输数据长度
    DMA_SetCurrDataCounter(USART6_RX_DMA_STREAM,JUDGESYSTEM_PACKSIZE);
		//打开DMA
		DMA_Cmd(USART6_RX_DMA_STREAM, ENABLE);
		
		
		//清除空闲中断标志位
		(void)USART6->DR;
		(void)USART6->SR;
		//串口空闲中断标志位并不能通过下面的函数清除
		//USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	}
}


void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 
	{	
		  Hero_control();
		  TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);  		 
	}		 	
}


//闪灯的计数器
void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{	
		
		static uint8_t heat_counter = 0;


		//裁判系统周期：10HZ，即为100ms检测一次，
		//客户端发送频率
		heat_counter++;

		PC_Control_Light();
		if(heat_counter>10)
		{
			Judge_sendPC(SCCM_ReceiveData.cap_level,Shooting.LargeBulletHeat,-GY_6050[0].EularDate.Pitch,PCMode.userVisionLight,PCMode.my_robot_id);
		  Heat_Calculation();
		  heat_counter = 0;
		}
		
		
    //帧率检测
		DevicesMonitor_update();
    DevicesLED_Mode();

    fric_Control(0,0,TurnOnLarge);
    fric_Control(0,0,TurnOnSmall);

		TIM_ClearITPendingBit(TIM5 , TIM_IT_Update);  		 

	}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
