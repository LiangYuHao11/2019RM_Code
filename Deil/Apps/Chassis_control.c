/**
  ******************************************************************************
  * @file    Chassis_control.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   底盘控制函数接口
  ******************************************************************************
  */
  
  
#include "Chassis_control.h"

Chassis_t Chassis;


/**
  * @brief  底盘初始化，配置参数
  * @param  None
  * @retval None
  */
void Chassis_Init(void){
	
  Chassis.LpfAttFactor = Chassis_LpfAttFactor;
	Chassis.speedLimit = Chassis_SpeedNormal;
	Chassis.mode = Chassis_Mode_Normal;
	Chassis.PowerOverflowFlag = 1;
	Chassis.swingSpeed = Chassis_SwingSpeed_Normal;
	
	/*-----正常使用PID-----*/
	//底盘电机PID
	for(int i = 0; i < 4; i++){
	IncrementalPID_paraReset(&M3508s[i].pid, 2.5f, 0.8f, 0.0f, 8000, 1000);
	}
}	

/**
  * @brief  麦克纳姆轮速度解算
  * @param[in]  Vx		x轴速度
  *				Vy		y轴速度
  *				VOmega	自转速度
  * @param[out]	Speed	速度
  * @retval None
  */
void MecanumCalculate(float Vx, float Vy, float VOmega, int16_t *Speed)
{
  float tempSpeed[4];
	float MaxSpeed = 0.0f;
	float Param = 1.0f;
	
	//速度限制
	VAL_LIMIT(Vx, -Chassis_MaxSpeed_X, Chassis_MaxSpeed_X);  
	VAL_LIMIT(Vy, -Chassis_MaxSpeed_Y, Chassis_MaxSpeed_Y);  
	VAL_LIMIT(VOmega, -Chassis_MaxSpeed_W, Chassis_MaxSpeed_W);  
	
	//四轮速度分解
	tempSpeed[0] = Vx - Vy + VOmega;
	tempSpeed[1] = Vx + Vy + VOmega;
	tempSpeed[2] = -Vx + Vy + VOmega;
	tempSpeed[3] = -Vx - Vy + VOmega;
	
    //寻找最大速度
    for(uint8_t i = 0; i < 4; i++)
    {
        if(abs(tempSpeed[i]) > MaxSpeed)
        {
            MaxSpeed = abs(tempSpeed[i]);
        }
    }
	
	
	//速度分配
    if(MaxSpeed > WheelMaxSpeed)
    {
        Param = (float)WheelMaxSpeed / MaxSpeed;
    }
	
	Speed[0] = tempSpeed[0] * Param;
	Speed[1] = tempSpeed[1] * Param;
	Speed[2] = tempSpeed[2] * Param;
	Speed[3] = tempSpeed[3] * Param;
	
}

/**
  * @brief  电流检测
  * @param[in]  M3508s_t  M3508结构体指针
  *				CurrentLimit  限制地电流		
  * @param[out]	None
  * @retval None
  */
void Chassis_powerLimit(M3508s_t *p_M3508s, float CurrentLimit){
	float Current_Sum = 0.0f;
	
	//计算电流和
	Current_Sum = abs(p_M3508s[0].outCurrent) + abs(p_M3508s[1].outCurrent) \
					+ abs(p_M3508s[2].outCurrent) + abs(p_M3508s[3].outCurrent);
	
	//功率分配
	if(Current_Sum > CurrentLimit){
		p_M3508s[0].outCurrent = CurrentLimit * p_M3508s[0].outCurrent / Current_Sum;
		p_M3508s[1].outCurrent = CurrentLimit * p_M3508s[1].outCurrent / Current_Sum;
		p_M3508s[2].outCurrent = CurrentLimit * p_M3508s[2].outCurrent / Current_Sum;
		p_M3508s[3].outCurrent = CurrentLimit * p_M3508s[3].outCurrent / Current_Sum;
	}

	
	
}
float CurrentLimit = 0.0f;

/**
  * @brief  底盘控制处理-跟随云台
  * @param[in]  Vx		x轴速度
  *				Vy		y轴速度
  *				Omega	偏向角
*				mode	模式 - 除Status_ControlOFF外，其他正常控制
  * @retval None
  */
void Chassis_processing(float Vx, float Vy, float VOmega, uint8_t mode){
	
	
}






