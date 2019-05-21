/**
  ******************************************************************************
  * @file    M2006_Motor.c
  * @author   Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   M2006无刷电机(拨弹用)，配套C610电调驱动应用函数接口
  ******************************************************************************
  */
  
  
#include "Clound_Motor.h"

M2006s_t M2006s[3];
M6020s_t M6020s[3];

/**
  * @brief  设置M2006和M3508电机电流值
  * @param  iqx (x:5) 对应id号电机的电流值，范围-10000~0~10000
  * @retval None
  */
void DeilMoto_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3){

	uint8_t data[8];
	
	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;

	CAN_SendData(CAN1, CAN_ID_STD, DeilMoto_SENDID, data);

}	
/**
  * @brief  设置M6020电机电流值
  * @param  iqx (x:5) 对应id号电机的电流值，范围-10000~0~10000
  * @retval None
  */
void CloundMoto_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3){

	uint8_t data[8];
	
	data[0] = iq1 >> 8;
	data[1] = iq1;
	data[2] = iq2 >> 8;
	data[3] = iq2;
	data[4] = iq3 >> 8;
	data[5] = iq3;

	CAN_SendData(CAN1, CAN_ID_STD, CloundMoto_SENDID, data);

}

/**
  * @brief  从CAN报文中获取M2006电机信息
  * @param[in]  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void Clound_Motor_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < DeilMoto_READID_START) || (RxMessage.StdId > CloundMoto_READID_END))
		return;
	uint32_t Deil_StdId;
	uint32_t Clound_StdId;	
	/*  xxx-201  
	大拨盘电机1:M2006[0] = 0x201
	小拨盘电机:M2006[1] = 0x202
	大拨盘电机2:M2006[2] = 0x203	
  大拨盘电机:M3508[4] = 0x204
	
	云台电机左:M6020[0] = 0x205
	云台电机右:M6020[1] = 0x206
  云台电机上:M6020[2] = 0x207
	*/
	Deil_StdId = RxMessage.StdId - DeilMoto_READID_START;
	
	Clound_StdId = RxMessage.StdId - CloundMoto_READID_START;
	
	if(Deil_StdId < 2){
		  //解包M2006的数据
			M2006s[Deil_StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
			M2006s[Deil_StdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
			M2006s[Deil_StdId].realTorque = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
			
			if(M2006s[Deil_StdId].realAngle - M2006s[Deil_StdId].lastAngle < -6000){
			   M2006s[Deil_StdId].turnCount++;
			 }
		
			if(M2006s[Deil_StdId].lastAngle - M2006s[Deil_StdId].realAngle < -6000){
				 M2006s[Deil_StdId].turnCount--;
			}
			
			M2006s[Deil_StdId].totalAngle = M2006s[Deil_StdId].realAngle + (8191*M2006s[Deil_StdId].turnCount);
			M2006s[Deil_StdId].lastAngle =  M2006s[Deil_StdId].realAngle;
			M2006s[Deil_StdId].M2006InfoUpdateFrame++;
			M2006s[Deil_StdId].M2006InfoUpdateFlag = 1;
	
	}
	
		if(Deil_StdId == 2){
		  //解包M2006的数据
			M3508s[4].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
			M3508s[4].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
			M3508s[4].realCurrent = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
			
			if(M3508s[4].realAngle - M3508s[4].lastAngle < -6000){
			   M3508s[4].turnCount++;
			 }
		
			if(M3508s[4].lastAngle - M3508s[4].realAngle < -6000){
				 M3508s[4].turnCount--;
			}
			
			M3508s[4].totalAngle = M3508s[4].realAngle + (8191*M3508s[4].turnCount);
			M3508s[4].lastAngle =  M3508s[4].realAngle;
			M3508s[4].M3508InfoUpdateFrame++;
			M3508s[4].M3508InfoUpdateFlag = 1;	
	}
		
		if(Clound_StdId < 3){
			M6020s[Clound_StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
			M6020s[Clound_StdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
			M6020s[Clound_StdId].realTorque = (int16_t)(RxMessage.Data[4]<<8 | RxMessage.Data[5]);
			M6020s[Clound_StdId].realTemperature = (int16_t)(RxMessage.Data[6]);

			if(M6020s[Clound_StdId].realAngle - M6020s[Clound_StdId].lastAngle < -6000){
				M6020s[Clound_StdId].turnCount++;
			}

			if(M6020s[Clound_StdId].lastAngle - M6020s[Clound_StdId].realAngle < -6000){
				M6020s[Clound_StdId].turnCount--;
			}

			M6020s[Clound_StdId].lastAngle =  M6020s[Clound_StdId].realAngle;

			M6020s[Clound_StdId].totalAngle = M6020s[Clound_StdId].realAngle + (8192*M6020s[Clound_StdId].turnCount);
			M6020s[Clound_StdId].M6020InfoUpdateFrame++;
			M6020s[Clound_StdId].M6020InfoUpdateFlag = 1;		
		}
	
}	


/**
  * @brief  设定M6020电机的目标角度
  * @param  motorName 	电机名字 @ref M6623Name_e
  *			angle		电流值，范围 0~8191 由于设置0和8191会导致电机振荡，要做个限幅
  * @retval None
  */
void M6020_setTargetAngle(M6020Name_e motorName, int32_t angle){
	M6020s[motorName].targetAngle = angle;
}







