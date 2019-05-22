/**
  ******************************************************************************
  * @file    Super_Cap.h
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   ��������
  ******************************************************************************
  */

#include "Super_Cap.h"

SCCM_ReceiveData_t SCCM_ReceiveData;
SCCM_SendData_t SCCM_SendData;
SCCM_Status_t SCCM_Status;

/**
  * @brief  �������ݽ��պ���
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
	 

	//֡��
	SCCM_Status.capInfoUpdateFrame++;
  SCCM_Status.capInfoUpdateFlag = 1;

}

SCCM_SendData_t		Temp_SCCM_SendData;
/**
  * @brief  �������ݷ��ͺ���
  * @param  sccm_power  ��繦�ʣ���λ��W
  *         sccm_enable ����Ƿ�ʹ��
  *         sccm_output �Ƿ�ʹ�õ��ݹ���
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
  * @brief  ���ݿ��ƺ���
  * @param  cap_remain ����ʣ���ѹ 0%~100%
  *         cap_usaflag �����Ƿ���Խ������
  * @retval None
  */
void SCCM_Control(int8_t cap_remain, uint8_t cap_usaflag, uint8_t mode)
{
	  //������ʱ��ֱ�ӷ���
	  if(SCCM_Status.sccmControlMode != GameBegin)
		{
			if(!cap_usaflag)
			{
				return;		
			}
		
		}		
	
 /************************ģʽѡ��************************/		
    //ֻ���ó������ݹ��磬ȫ���ʸ������
		if(mode == GameBegin)
		{
		  SCCM_Send(70.0f,USE,DISUSE);		  			
		}
		//ʹ�õ�ع��磬���������
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
		//ʹ�õ�ع��磬�����������		
    else if(mode == BatteryDisChargeCap)
		{
		   SCCM_Send(0.0f,DISUSE,DISUSE);		  	
		}		
		//ֻ�õ��ݣ�ȫ����硣
		else 
		{
				SCCM_Send(75.0f,USE,USE);	
		}			
					
}

/**
  * @brief  ���ݳ�ʼ������
  * @param  None
  * @retval None
  */
void SCCM_Init(void)
{
  SCCM_Status.sccmControlMode = GameBegin;
}






