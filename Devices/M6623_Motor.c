/**
  ******************************************************************************
  * @file    M6623_Motor.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   6623无刷电机，配套C620电调驱动应用函数接口
  ******************************************************************************
  */
  
  
#include "M6623_Motor.h"

M6623s_t M6623s[2];


/**
  * @brief  6623校准函数
  * @param  None
  * @retval None
  */
void M6623_calibration(void){

	uint8_t data[8];
	
	//数据格式详见说明书P7
	data[0] = 'c';
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	data[7] = 0;
	
	CAN_SendData(CAN1, CAN_ID_STD, M6623_CALIBRATIONID, data);
	
}	



/**
  * @brief  从CAN报文中获取M6623电机信息
  * @param  *RxMessage 	CAN报文接收结构体指针
  *			**info		M6623s_t电机数组指针
  * @retval None
  */
void M6623_getInfo(CanRxMsg RxMessage){
	//报文id确认
	if((RxMessage.StdId < M6623_READID_START) || (RxMessage.StdId > M6623_READID_END))
		return;
	uint32_t StdId;
	StdId = RxMessage.StdId - M6623_READID_START;
	
	//解包数据，数据格式详见6623电调说明书P8
	M6623s[StdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8 | RxMessage.Data[1]);
	M6623s[StdId].realCurrent = (int16_t)(RxMessage.Data[2]<<8 | RxMessage.Data[3]);
	
	if(M6623s[StdId].realAngle - M6623s[StdId].lastAngle < -6000){
		M6623s[StdId].turnCount++;
	}
	
	if(M6623s[StdId].lastAngle - M6623s[StdId].realAngle < -6000){
		M6623s[StdId].turnCount--;
	}
	
	M6623s[StdId].lastAngle =  M6623s[StdId].realAngle;
	
	M6623s[StdId].totalAngle = M6623s[StdId].realAngle + (8192*M6623s[StdId].turnCount);
  M6623s[StdId].M6623InfoUpdateFrame++;
	M6623s[StdId].M6623OffLineFlag = 1;
	
}	


/**
  * @brief  设定M6623电机的目标角度
  * @param  motorName 	电机名字 @ref M6623Name_e
  *			angle		电流值，范围 0~8191 由于设置0和8191会导致电机振荡，要做个限幅
  * @retval None
  */
void M6623_setTargetAngle(M6623Name_e motorName, int32_t angle){
	M6623s[motorName].targetAngle = angle;
}

