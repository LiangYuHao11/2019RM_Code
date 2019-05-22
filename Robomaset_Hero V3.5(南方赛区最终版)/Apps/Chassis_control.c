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
//车身角度判断
extern float Chassis_Angle;


/**
  * @brief  底盘初始化，配置参数
  * @param  None
  * @retval None
  */
void Chassis_Init(void){
	
  Chassis.LpfAttFactor = 0.2;
	Chassis.speedLimit = 6000;
	Chassis.speedUp = 1;
	Chassis.swingSpeed = 0;
	Chassis.swingStep = PI/100;
	Chassis.swingRange = 40;
	/*-----正常使用PID-----*/
	//底盘电机PID

	IncrementalPID_paraReset(&M3508s[0].pid, 11.0f, 3.5f, 0.0f, 8000, 1000);
	IncrementalPID_paraReset(&M3508s[1].pid, 11.0f, 3.5f, 0.0f, 8000, 1000);
	IncrementalPID_paraReset(&M3508s[2].pid, 11.0f, 3.5f, 0.0f, 8000, 1000);
	IncrementalPID_paraReset(&M3508s[3].pid, 11.0f, 3.5f, 0.0f, 8000, 1000);


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
		
	//四轮速度分解
	//平地模式
	if(Chassis_Angle > -10.0f )
	{
		//速度限制
		VAL_LIMIT(Vx, -Chassis_MaxSpeed_X, Chassis_MaxSpeed_X);  
		VAL_LIMIT(Vy, -Chassis_MaxSpeed_Y, Chassis_MaxSpeed_Y);  
		VAL_LIMIT(VOmega, -Chassis_MaxSpeed_W, Chassis_MaxSpeed_W);  		
		
		tempSpeed[0] = Vx - Vy + VOmega;
		tempSpeed[1] = Vx + Vy + VOmega;
		tempSpeed[2] = (-Vx + Vy + VOmega);
		tempSpeed[3] = (-Vx - Vy + VOmega);
	}
	//斜坡模式
	else
	{
		//速度限制
		VAL_LIMIT(Vx, -Ramp_Chassis_MaxSpeed_X, Ramp_Chassis_MaxSpeed_X);  
		VAL_LIMIT(Vy, -Ramp_Chassis_MaxSpeed_Y, Ramp_Chassis_MaxSpeed_Y);  
		VAL_LIMIT(VOmega, -Ramp_Chassis_MaxSpeed_W, Ramp_Chassis_MaxSpeed_W); 		
		
		tempSpeed[0] = Vx - Vy + VOmega;
		tempSpeed[1] = Vx + Vy + VOmega;
		tempSpeed[2] = -Vx + Vy*5.0f + VOmega;
		tempSpeed[3] = -Vx - Vy*5.0f + VOmega;
	}
	
	
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



/**
  * @brief  扭腰函数
  * @param[in]  Cloud_Yaw_Center - M6020s[2].totalAngle之间的差值		
  * @retval swing_output 此时应该输出的的电流值
  */
float Chassis_Swing(float swing_measure)
{
	float swing_output;

	swing_measure = Angle_transformation(swing_measure);   	    //角度转化，将8192转为360度
	swing_measure = Radian_transformation(swing_measure);   	  //弧度转化，将360度转为弧度单位
	
	Chassis.swingTarget = Radian_transformation(Chassis.swingRange)*sin(Chassis.swingSpeed);    
	Chassis.swingSpeed += Chassis.swingStep;
	
	if( Chassis.swingStep > 2*PI)
	{
    Chassis.swingStep	= 0;
	}
	
	swing_output = FollowSwing_Position_PID(&Chassis.SwingAttitude_pid, Chassis.swingTarget, swing_measure);
  return swing_output;
	
}


/**
  * @brief  底盘控制处理-跟随云台
  * @param[in]  Vx		x轴速度
  *				Vy		y轴速度
  *				Omega	偏向角
*				mode	模式 - 除Status_ControlOFF外，其他正常控制
  * @retval None
  */
void Chassis_processing(float Vx, float Vy, float VOmega, uint8_t left_mode,uint8_t right_mode){
	
	
	if((left_mode == ControlMode_OFF)||(dr16_data.DR16OffLineFlag)){
		M3508_setCurrent(0, 0, 0, 0);
		return;  
	}
		
	
	  //平滑处理	
		Filter_IIRLPF(&Vx, &Chassis.targetXLPF,Chassis.LpfAttFactor);
		Filter_IIRLPF(&Vy, &Chassis.targetYLPF, Chassis.LpfAttFactor);
		Filter_IIRLPF(&VOmega, &Chassis.targetZLPF, Chassis.LpfAttFactor);	
	
		float tempVOmega = 0.0f;

  
	if(!dr16_data.DR16OffLineFlag){
		if(ControlMode_ByRC == left_mode)
	  {					
			if(abs(Cloud_Yaw_Center - M6020s[2].totalAngle) > 30.0f)
			{
				Chassis.FollowtargetYawRaw = FollowSwing_Position_PID(&Chassis.FollowYawAttitude_pid_RC, 0.0f, (Cloud_Yaw_Center - M6020s[2].totalAngle));
				Chassis.FollowtargetYawLPF = Chassis.FollowtargetYawRaw;
        //由于不是跟随模式，云台不在中点而导致自旋值有输出
			  if(GY_6050[0].ImuOffLineFlag)
				{
			    Chassis.FollowtargetYawLPF = 0.0f;					
				}
				tempVOmega = Chassis.targetZLPF + Chassis.FollowtargetYawLPF;	
			}
			
		}
		
		if(ControlMode_ByPC == left_mode)
	  {	
			if(abs(Cloud_Yaw_Center - M6020s[2].totalAngle) > 30.0f)
			{			
			  Chassis.FollowtargetYawRaw = FollowSwing_Position_PID(&Chassis.FollowYawAttitude_pid_PC, 0.0f, (Cloud_Yaw_Center - M6020s[2].totalAngle));
			  Filter_IIRLPF(&Chassis.FollowtargetYawRaw, &Chassis.FollowtargetYawLPF, 1.0f);
        //由于不是跟随模式，云台不在中点而导致自旋值有输出				
			  if((GY_6050[0].ImuOffLineFlag)||(PCMode.suppiyControl))				
			  { 
			    Chassis.FollowtargetYawLPF = 0.0f;
				}
			  tempVOmega = Chassis.targetZLPF + Chassis.FollowtargetYawLPF;		
			}				
			}
		}
	
	int16_t speed[4];
  int16_t pid_out[4];
	
	//麦轮解算
  MecanumCalculate(Chassis.targetXLPF, Chassis.targetYLPF,tempVOmega, speed);

	for(int i = 0; i<4; i++){
    if(M3508s[i].M3508InfoUpdateFlag == 1){
			
			M3508s[i].targetSpeed = speed[i];
			//PID计算
			M3508s[i].outCurrent = Incremental_PID(&M3508s[i].pid, M3508s[i].targetSpeed, M3508s[i].realSpeed); 
			pid_out[i] = M3508s[i].outCurrent;
			//清标志位
			M3508s[i].M3508InfoUpdateFlag = 0;
		}
	}
	
  //功率处理
	SetInPut(&powerBufferPool_t,pid_out,4);
	
	for(int i = 0; i<4; i++){	
	  M3508s[i].outCurrent = pid_out[i] ;
	}


	
	//设定电机电流值
	M3508_setCurrent(M3508s[0].outCurrent, M3508s[1].outCurrent, M3508s[2].outCurrent, M3508s[3].outCurrent);	

}
	





