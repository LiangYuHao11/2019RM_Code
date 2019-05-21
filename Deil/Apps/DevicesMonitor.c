/**
  ******************************************************************************
  * @file    DevicesMonitor.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief   外部设备帧率检测函数接口
  ******************************************************************************
  */
  
  
#include "DevicesMonitor.h"

//最小帧率
#define GY_IMU_FRAMEMIN		5


/**
  * @brief  外接设备帧率清零，每200ms清零一次
  * @param  None
  * @retval None
  */
void DevicesMonitor_update(void){

	//遥控器
	if(dr16_data.DR16InfoUpdateFrame < 5){
		dr16_data.DR16OffLineFlag = 1;
		
	}
	else{
		dr16_data.DR16OffLineFlag = 0;
	}
	dr16_data.DR16InfoUpdateFrame = 0;
		
	
}
