#ifndef __CHIASSIS_CONTROL_H
#define __CHIASSIS_CONTROL_H

#include "user_common.h"

#define Chassis_LpfAttFactor 0.02f
#define test_speed        7000.0f
#define WheelMaxSpeed     10000		//单轮最大速度

#define Chassis_MaxSpeed_Y	test_speed		//底盘前后最快速度
#define Chassis_MaxSpeed_X	test_speed		//底盘平移最快速度
#define Chassis_MaxSpeed_W	test_speed		//底盘自旋最快速度

#define S_Chassis_MaxSpeed_Y	8500		//底盘前后最快速度
#define S_Chassis_MaxSpeed_X	8500		//底盘平移最快速度
#define S_Chassis_MaxSpeed_W	8500		//底盘自旋最快速度

#define Chassis_SpeedNormal		test_speed
#define Chassis_SpeedFast		  test_speed
#define Chassis_SpeedSlow		  test_speed

#define Chassis_SpeedPowerOverflow		3000.0f
#define Chassis_SpeedSupplyMode			3800.0f

#define Chassis_SwingSpeed_Normal		3900
#define Chassis_SwingSpeed_Slow			2200

#define WheelCount 			4

#define Chassis_Mode_Normal			0
#define Chassis_Mode_Follow			1

#define  Angle_transformation(ang)			ang/22.75f
#define  Radian_transformation(ang)			ang*(PI/180.0f)
			


extern Chassis_t Chassis;
 
void Chassis_Init(void);
void MecanumCalculate(float Vx, float Vy, float VOmega, int16_t *Speed);
void Chassis_processing(float Vx, float Vy, float VOmega, uint8_t left_mode,uint8_t right_mode);
void Chassis_powerLimit(M3508s_t *p_M3508s, float CurrentLimit);
void Ramp_MecanumCalculate(float Vx, float Vy, float VOmega, int16_t *Speed);
 
#endif /* __CHIASSIS_CONTROL_H */



