/**
  ******************************************************************************
  * @file    Super_Cap.h
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   超级电容
  ******************************************************************************
  */

#include "Super_Cap.h"

SCCM_ReceiveData_t SCCM_ReceiveData;
SCCM_SendData_t SCCM_SendData;
SCCM_Status_t SCCM_Status;

/**
  * @brief  电容数据接收函数
  * @param  RxMessage 
  * @retval None
  */
void SCCM_getInfo(CanRxMsg RxMessage)
{
	if(RxMessage.StdId != SCCM_RX_ID)
	{
		return;
	}
	

	 memcpy(SCCM_ReceiveData.data, RxMessage.Data, sizeof(uint8_t[8]));
	 

	//帧率
	SCCM_Status.capInfoUpdateFrame++;
  SCCM_Status.capInfoUpdateFlag = 1;

}

SCCM_SendData_t		Temp_SCCM_SendData;
/**
  * @brief  电容数据发送函数
  * @param  sccm_power  充电功率，单位：W
  *         sccm_enable 充电是否使能
  *         sccm_output 是否使用电容供电
  * @retval None
  */
void SCCM_Send(float sccm_power,uint8_t sccm_enable,uint8_t sccm_output)
{

	Temp_SCCM_SendData.charge_power = sccm_power;
	Temp_SCCM_SendData.charge_enable = sccm_enable;
	Temp_SCCM_SendData.is_cap_output = sccm_output;
 
	CAN_SendData(CAN2, CAN_ID_STD, SCCM_TX_ID, Temp_SCCM_SendData.data);
	
}	


/**
  * @brief  电容控制函数
  * @param  cap_remain 电容剩余电压 0%~100%
  *         cap_usaflag 电容是否可以进行输出
  * @retval None
  */
void SCCM_Control(int8_t cap_remain, uint8_t cap_usaflag, uint8_t mode)
{
	  //不可用时候，直接返回
	  if(SCCM_Status.sccmControlMode != GameBegin)
		{
			if(!cap_usaflag)
			{
				return;		
			}
		
		}		
	
 /************************模式选择************************/		
    //只是用超级电容供电，全功率给他充电
		if(mode == GameBegin)
		{
		  SCCM_Send(70.0f,USE,DISUSE);		  			
		}
		//使用电池供电，并给他充电
    else if(mode == BatteryChargeCap)
		{
			if(ext_power_heat_data.data.chassis_power < 80)
			{
				SCCM_Send((70-ext_power_heat_data.data.chassis_power),USE,DISUSE);	
			}
       else
		   {
				 SCCM_Send(0.0f,DISUSE,DISUSE);		
			 }				 
		}		
		//使用电池供电，不给给他充电		
    else if(mode == BatteryDisChargeCap)
		{
		   SCCM_Send(0.0f,DISUSE,DISUSE);		  	
		}		
		//只用电容，全力充电。
		else 
		{
				SCCM_Send(75.0f,USE,USE);	
		}			
					
}

/**
  * @brief  电容初始化函数
  * @param  None
  * @retval None
  */
void SCCM_Init(void)
{
  SCCM_Status.sccmControlMode = GameBegin;
}






