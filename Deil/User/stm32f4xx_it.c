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
		M2006_getInfo(RxMessage);
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


void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 
	{	
		fric_Control(0,0,TurnOnSmall);
		fric_Control(0,0,TurnOnLarge);
	  //拨盘频率计算 
		
		switch(ControlStatus.ControlLeft)
		{
			case ControlMode_ByRC:
			 fric_Control(1500,1500,SetSmall);			

				 Small_Deil_Counter++;
				//挡位选择
				if(ControlStatus.ControlRight == ControlMode_High)
				{					
					if(Small_Deil_Counter > 200)
					{
						Small_Deil_Flag = 1;
						Small_Deil_Counter = 0;
					}
					else
					{
						Small_Deil_Flag = 0;
					}
				}
				if(ControlStatus.ControlRight == ControlMode_Mid)
				{				
					if(Small_Deil_Counter > 50)
					{
						Small_Deil_Flag = 1;
						Small_Deil_Counter = 0;
					}
					else
					{
						Small_Deil_Flag = 0;
					}
				}
				if(ControlStatus.ControlRight == ControlMode_Low)
				{
					if(Small_Deil_Counter > 10)
					{
						Small_Deil_Flag = 1;
						Small_Deil_Counter = 0;
					}
					else
					{
						Small_Deil_Flag = 0;
					}					
					
				}

				break;

		  case ControlMode_ByPC:
//				Large_Deil_Counter++;				
//			 if(Large_Deil_Counter > 200)
//				{
//					Large_Deil_Flag = 1;
//					Large_Deil_Counter = 0;
//				}
//				else
//				{
//					Large_Deil_Flag = 0;
//				}				
//      	//挡位选择
//				if(ControlStatus.ControlRight == ControlMode_High)
//				{					
//				  fric_Control(1200,1200,SetSmall);			
//				  fric_Control(1100,1100,SetLarge);								
//				}
//				if(ControlStatus.ControlRight == ControlMode_Mid)
//				{				
//			    fric_Control(0,0,TurnOffSmall);	
//			    fric_Control(0,0,TurnOffLarge);		
//					
//				}
//				if(ControlStatus.ControlRight == ControlMode_Low)
//				{
//					
//				}				
				 fric_Control(0,0,TurnOffSmall);						

				break;
			
			 case ControlMode_OFF:
						
				 fric_Control(0,0,TurnOffSmall);						

			  break;
		}
		
		
		if(!(ControlStatus.ControlLeft == ControlMode_High))
		{
			Smallshoot_processing();
		}
		if(!(ControlStatus.ControlLeft == ControlMode_Mid))
		{		
      Largeshoot_processing();
		}
		
		TIM_ClearITPendingBit(TIM6 , TIM_IT_Update);  		 
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
