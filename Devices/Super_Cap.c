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
	
//	SCCM_Status.capInfoUpdateFrame++;
//  SCCM_Status.capInfoUpdateFlag = 1;

}
  uint8_t data[8];
	uint32_t Charge_power;
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
//	uint32_t Charge_power = (uint32_t)sccm_power;
	
//	data[0] = Charge_power >>24;   //��繦�ʣ���λ��W
//	data[1] = Charge_power >>16;   
//	data[2] = Charge_power >>8;
//	data[3] = Charge_power;
//	data[0] = Charge_power ;   //��繦�ʣ���λ��W
//	data[1] = Charge_power >>8;
//	data[2] = Charge_power >>16;
//	data[3] = Charge_power >>24;		
//	data[4] = sccm_enable;         //���ʹ��
//	data[5] = sccm_output;         //�Ƿ�ʹ�õ��ݹ���
//	data[6] = 0;
//	data[7] = 0;

	Temp_SCCM_SendData.charge_power = sccm_power;
	Temp_SCCM_SendData.charge_enable = sccm_enable;
	Temp_SCCM_SendData.is_cap_output = sccm_output;
 
//	memcpy(Temp_SCCM_SendData.data, data, sizeof(uint8_t[8]));

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
					
		//��С��һ����ѹֵʱ�����������磬���������
			if(cap_remain < SCCM_Status.capChargeLeve)
			{
				//��繦��  ���ʹ��  �Ƿ�ʹ�õ��ݹ���
				SCCM_Send(70.0f,USE,DISUSE);
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
			 SCCM_Send(20.0f,USE,DISUSE);		  	
		}		
		//ʹ�õ�ع��磬�����������		
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






