/**
  ******************************************************************************
  * @file    Bullet_Calculate.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   车身姿态检测
  ******************************************************************************
  */
	
#include "Ramp_Posture.h"

float Temp_Ramp_angle = 0;
float Real_Ramp_angle = 0;

	/**
  * @brief  车身判断函数
  * @param 	 None
  * @retval  None
  */ 
int Ramp_Detection(void)
{
	 static int Ramp_Counter = 0;

   if(Cloud_Pitch_Center != M6020s[1].realAngle)
	 {
		   Temp_Ramp_angle = GY_6050[0].EularDate.Pitch - ((Cloud_Pitch_Center - M6020s[0].realAngle)/22.75f);

		 if(Temp_Ramp_angle>8)
		 {
		   Ramp_Counter++;
		 }
	 }

	 if(Ramp_Counter > 30)
	 { 
			 Real_Ramp_angle = Temp_Ramp_angle;
	 }
	 if(Temp_Ramp_angle< 8)
	 {
	   Real_Ramp_angle = 0;
	 }
	 
	 return Real_Ramp_angle;
 
}










