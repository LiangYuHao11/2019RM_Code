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
	
//	SCCM_Status.capInfoUpdateFrame++;
//  SCCM_Status.capInfoUpdateFlag = 1;

}
  uint8_t data[8];
	uint32_t Charge_power;
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
//	uint32_t Charge_power = (uint32_t)sccm_power;
	
//	data[0] = Charge_power >>24;   //充电功率，单位：W
//	data[1] = Charge_power >>16;   
//	data[2] = Charge_power >>8;
//	data[3] = Charge_power;
//	data[0] = Charge_power ;   //充电功率，单位：W
//	data[1] = Charge_power >>8;
//	data[2] = Charge_power >>16;
//	data[3] = Charge_power >>24;		
//	data[4] = sccm_enable;         //充电使能
//	data[5] = sccm_output;         //是否使用电容供电
//	data[6] = 0;
//	data[7] = 0;

	Temp_SCCM_SendData.charge_power = sccm_power;
	Temp_SCCM_SendData.charge_enable = sccm_enable;
	Temp_SCCM_SendData.is_cap_output = sccm_output;
 
//	memcpy(Temp_SCCM_SendData.data, data, sizeof(uint8_t[8]));

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
					
		//当小于一个电压值时，不用它供电，并给他充电
			if(cap_remain < SCCM_Status.capChargeLeve)
			{
				//充电功率  充电使能  是否使用电容供电
				SCCM_Send(70.0f,USE,DISUSE);
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
			 SCCM_Send(20.0f,USE,DISUSE);		  	
		}		
		//使用电池供电，不给给他充电		
    else if(mode == BatteryDisChargeCap)
		{
		   SCCM_Send(0.0f,DISUSE,DISUSE);		  	
		}		
		else 
		  SCCM_Send(70.0f,USE,USE);		  
			
			
}


void SCCM_Init(void)
{
  SCCM_Status.sccmControlMode = GameBegin;
  SCCM_Status.capChargeLeve = 20;
}






