/**
  ******************************************************************************
  * @file    GY_6050.h.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   
  ******************************************************************************
  */


#include "GY_6050.h"

GY_IMU_t GY_6050[2];

bno055_data_u bno055_data[2];

/**
  * @brief  从CAN报文中获取云台陀螺仪信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void GY6050_getCloundInfo(CanRxMsg RxMessage){
	//报文id确认
	if(RxMessage.StdId == GY6050_SENDID)
	{	
			memcpy(bno055_data[0].dataBuff, RxMessage.Data, sizeof(uint8_t[8]));
			GY_6050[0].EularDate.Yaw = (float)bno055_data[0].yaw/100.0f;
			GY_6050[0].Gyro.z = bno055_data[0].gyro_z;
			GY_6050[0].EularDate.Pitch = (float)bno055_data[0].pitch/100.0f;
			GY_6050[0].Gyro.x = bno055_data[0].gyro_x;
		
			if(abs(GY_6050[0].Gyro.z)<2)
			{
				GY_6050[0].Gyro.z = bno055_data[0].gyro_z=0;
			}
		
			if(GY_6050[0].EularDate.Yaw - GY_6050[0].lastYaw < -300){
			  GY_6050[0].turnCount++;
			}	
			if(GY_6050[0].lastYaw - GY_6050[0].EularDate.Yaw  < -300){
				GY_6050[0].turnCount--;
			}

			
			GY_6050[0].totalYaw = GY_6050[0].EularDate.Yaw + (360*GY_6050[0].turnCount);
			
			GY_6050[0].lastYaw = GY_6050[0].EularDate.Yaw;
			
			GY_6050[0].ImuInfoUpdateFrame++;
			GY_6050[0].ImuInfoUpdateFlag = 1;
	 }

}	


/**
  * @brief  从CAN报文中获取底盘陀螺仪信息
  * @param  RxMessage 	CAN报文接收结构体
  * @retval None
  */
void GY6050_getChassisInfo(CanRxMsg RxMessage){
	//报文id确认
	if(RxMessage.StdId == GY6050_SENDID)
	{	
			memcpy(bno055_data[1].dataBuff, RxMessage.Data, sizeof(uint8_t[8]));
			GY_6050[1].EularDate.Yaw = bno055_data[1].yaw;
			GY_6050[1].Gyro.z = bno055_data[1].gyro_z;
		
			if(GY_6050[1].EularDate.Yaw - GY_6050[1].lastYaw < -300){
			  GY_6050[1].turnCount++;
			}	
			if(GY_6050[1].lastYaw - GY_6050[1].EularDate.Yaw  < -300){
				GY_6050[1].turnCount--;
			}
			
			GY_6050[1].totalYaw = GY_6050[1].EularDate.Yaw + (360*GY_6050[1].turnCount);
			
			GY_6050[1].lastYaw = GY_6050[1].EularDate.Yaw;
			
			GY_6050[1].ImuInfoUpdateFrame++;
			GY_6050[1].ImuInfoUpdateFlag = 1;
	 }

}	






