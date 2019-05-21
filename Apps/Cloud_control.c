/**
  ******************************************************************************
  * @file    Cloud_control.c
  * @author Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   云台控制函数接口
  ******************************************************************************
  */
  
  
#include "Cloud_control.h"

Cloud_t Cloud;
/**
  * @brief  设置M6623电机电流值（id号为5,6），M2006电机电流值（id 7）
  * @param  iqyaw 	对应yaw轴电机的电流值，范围-5000~0~5000
  *			iqpitch	对应pitch轴电机的电流值，范围-5000~0~5000
  *			iq7，拨弹电机电流值，范围-10000~0~10000
  * @retval None
  */
void Cloud_setCurrent(int16_t iqyaw, int16_t iqpitch, int16_t iq7, int16_t iq8){

	uint8_t data[8];
	
	//数据格式详见说明书P32
	data[0] = iqyaw >> 8;
	data[1] = iqyaw;
	data[2] = iqpitch >> 8;
	data[3] = iqpitch;
	data[4] = iq7 >> 8;
	data[5] = iq7;
	data[6] = iq8 >> 8;
	data[7] = iq8;
	
	CAN_SendData(CAN1, CAN_ID_STD, 0x1FF, data);
	
}	



/**
  * @brief  云台初始化，配置参数并归位云台
  * @param  None
  * @retval None
  */
void Cloud_Init(void){
	
	GY_6050[0].turnCount = 0;
	M6020s[0].turnCount = 0;
	M6020s[1].turnCount = 0;
	M6020s[2].turnCount = 0;
	
	Cloud.LpfAttFactor = 0.8f;
	
	//保存启动时刻机械角度的Yaw轴和Pitch轴
	Cloud.targetYawRaw = Cloud.targetYawLPF = M6020s[2].totalAngle;
	Cloud.targetPitchRaw = Cloud.targetPitchLPF = M6020s[0].totalAngle ;
  Cloud.targetPitchRaw = Cloud.targetPitchLPF = Cloud_Pitch_Center;
	
	//保存启动时刻陀螺仪角度的Yaw轴和Pitch轴
	Cloud.IMUtargetYawRaw = Cloud.IMUtargetYawLPF = GY_6050[0].totalYaw;

	M6020_setTargetAngle(M6020_YAW, Cloud.targetYawRaw);
	M6020_setTargetAngle(M6020_PITCH_Right, Cloud.targetPitchRaw);
	
	//设置跟随模式Yaw轴PID
/************************************陀螺仪PID***********************************/	
	PositionPID_paraReset(&Cloud.YawAttitude_pid, -350.0f, 0.0f, -200.0f, 0.0f, 0.0f, 0.0f, 8000,1000);
	PositionPID_paraReset(&Cloud.YawSpeed_pid, 7.0f, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, 29000, 1000);
	
	PositionPID_paraReset(&Cloud.PitchAttitude_pid, 18.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f,29000, 200);
	PositionPID_paraReset(&Cloud.PitchSpeed_pid, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,29000, 200);

	/************************************机械角度PID***********************************/	
	//设置机械角度模式Yaw轴PID
  PositionPID_paraReset(&M6020s[2].Attitude_pid, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,  25000, 500);
	PositionPID_paraReset(&M6020s[2].Speed_pid, 1500.0f, 0.0f, 80.0f, 0.0f, 0.0f,0.0f,  25000, 500);
  PositionPID_paraReset(&M6020s[0].Attitude_pid, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,  25000, 500);
  PositionPID_paraReset(&M6020s[0].Speed_pid, 380.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f,  25000, 500);
		


	//跟随PID	
 	PositionPID_paraReset(&Chassis.FollowYawAttitude_pid_RC, -9.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,8000, 500);	
 	PositionPID_paraReset(&Chassis.FollowYawAttitude_pid_PC, -9.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,4500, 500);	
 	PositionPID_paraReset(&Chassis.SwingAttitude_pid, -4500.0f, 0.0f, 0.0f, 0.0f,0.0f,0.0f,4500, 500);	
	
	
}	


/**********************************6623电机顺时针旋转时角度值减少************************************/
/**
  * @brief  获取云台Yaw旋转角度，以右边为极坐标的x轴，实际轴和x轴之间的夹角
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getYawAngleWithRight(void){
	 return (M6020s[2].totalAngle - Cloud_Yaw_Min)/22.75f;
}

/**
  * @brief  获取云台Pitch旋转角度（和电机的安装有关
）
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getPitchAngleWithDown(void){
 	return (M6020s[0].realAngle - Cloud_Pitch_Min)/22.75f;
}

/**
  * @brief  获取云台Yaw旋转角度，以中间为极坐标的x轴，实际轴和x轴之间的夹角
  * @param[in]  None
  * @retval 角度
  */
float Cloud_getYawAngleWithCenter(void){
	return (M6020s[2].totalAngle - Cloud_Yaw_Center)/22.75f;
}


/**********************************设置云台机械坐标和云台IMU坐标************************************/
/**
  * @brief  强制设置云台机械坐标（绕过缓冲区）
  * @param[in]  posYaw
  *				posPitch
  * @retval None
  */
void Cloud_setAbsPosForced(float posYaw, float posPitch){
	Cloud.targetYawLPF = Cloud.targetYawRaw = posYaw;
	Cloud.targetPitchLPF = Cloud.targetPitchRaw = posPitch;
}

/**
  * @brief  强制设置云台IMU坐标（绕过缓冲）
  * @param[in]  posYaw
  *				posPitch
  * @retval None
  */
void Cloud_setIMUPosForced(float posYaw, float posPitch){
	Cloud.IMUtargetYawLPF = Cloud.IMUtargetYawRaw = posYaw;
	Cloud.IMUtargetPitchLPF = Cloud.IMUtargetPitchRaw = posPitch;
}


void Cloud_static(float temp_targetyaw, float delta_pitch,uint8_t mode)
{
	Cloud_setIMUPosForced(GY_6050[0].totalYaw, GY_6050[0].EularDate.Roll);
	
	if((mode == ControlMode_OFF)||(dr16_data.DR16OffLineFlag)){
		Cloud_setAbsPosForced(M6020s[2].totalAngle, M6020s[0].totalAngle);
		Cloud_setCurrent(0, 0, 0, 0);
		return;
	}
	
	  Cloud.targetPitchRaw -= delta_pitch;
    Cloud.targetYawRaw = temp_targetyaw;
	
	
	//云台限幅begin
	if(Cloud.targetPitchRaw > Cloud_Pitch_Max){
		Cloud.targetPitchRaw = Cloud_Pitch_Max;
	}
	if(Cloud.targetPitchRaw < Cloud_Pitch_Min) {
		Cloud.targetPitchRaw = Cloud_Pitch_Min;
	}
	
	//云台限幅end
	
	//平滑处理begin
	Filter_IIRLPF(&Cloud.targetYawRaw, &Cloud.targetYawLPF, Cloud.LpfAttFactor);
	Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, Cloud.LpfAttFactor);
	//平滑处理end
	
	M6020_setTargetAngle(M6020_YAW, Cloud.targetYawLPF);
	M6020_setTargetAngle(M6020_PITCH_Right, Cloud.targetPitchLPF);
		
	/*************PID计算****************/
		//YAW轴PID
  	M6020s[2].inneroutCurrent = Position_PID(&M6020s[2].Attitude_pid, M6020s[2].targetAngle, M6020s[2].totalAngle);	
  	M6020s[2].outCurrent = Position_PID(&M6020s[2].Speed_pid, M6020s[2].inneroutCurrent , M6020s[2].realSpeed );	
	
		
		//PITCH轴PID	
		M6020s[0].inneroutCurrent = Pitch_Position_PID(&M6020s[0].Attitude_pid, M6020s[0].targetAngle, M6020s[0].totalAngle);
	  M6020s[0].outCurrent = Pitch_Position_PID(&M6020s[0].Speed_pid, M6020s[0].inneroutCurrent, M6020s[0].realSpeed);
    M6020s[1].outCurrent = -M6020s[0].outCurrent;
	
	  //清标志位
		M6020s[0].M6020InfoUpdateFlag = 0;				
		M6020s[1].M6020InfoUpdateFlag = 0;				
		M6020s[2].M6020InfoUpdateFlag = 0;		


		//设定电机电流值

    CloundMoto_setCurrent(M6020s[0].outCurrent,M6020s[1].outCurrent,M6020s[2].outCurrent);



}


/**
  * @brief  云台控制处理
  * @param[in]  delta_yaw		航向角变化量
  *				delta_pitch		俯仰角变化量
  *				shoot	射击使能
  *				mode	模式 - 除Status_ControlOFF外，其他正常控制
  * @retval None
  */
void Cloud_processing(float delta_yaw, float delta_pitch, uint8_t mode){

	Cloud_setIMUPosForced(GY_6050[0].totalYaw, GY_6050[0].EularDate.Roll);
	
	if((mode == ControlMode_OFF)||(dr16_data.DR16OffLineFlag)){
		Cloud_setAbsPosForced(M6020s[2].totalAngle, M6020s[0].totalAngle);
		Cloud_setCurrent(0, 0, 0, 0);
		return;
	}
	
	if(dr16_data.DR16OffLineFlag){
		delta_yaw = delta_pitch = 0.0f;
	}
	
		Cloud.targetYawRaw -= delta_yaw;
	  Cloud.targetPitchRaw -= delta_pitch;
	
	//云台限幅begin
	if(Cloud.targetYawRaw > Cloud_Yaw_Max){
		Cloud.targetYawRaw = Cloud_Yaw_Max;
	}
	if(Cloud.targetYawRaw < Cloud_Yaw_Min){
		Cloud.targetYawRaw = Cloud_Yaw_Min;
	}
	if(Cloud.targetPitchRaw > Cloud_Pitch_Max){
		Cloud.targetPitchRaw = Cloud_Pitch_Max;
	}
	if(Cloud.targetPitchRaw < Cloud_Pitch_Min) {
		Cloud.targetPitchRaw = Cloud_Pitch_Min;
	}
	//云台限幅end
	
	//平滑处理begin
	Filter_IIRLPF(&Cloud.targetYawRaw, &Cloud.targetYawLPF, Cloud.LpfAttFactor);
	Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, Cloud.LpfAttFactor);
	//平滑处理end
	
	M6020_setTargetAngle(M6020_YAW, Cloud.targetYawLPF);
	M6020_setTargetAngle(M6020_PITCH_Right, Cloud.targetPitchLPF);
		
	/*************PID计算****************/
		//YAW轴PID
  	M6020s[2].inneroutCurrent = Position_PID(&M6020s[2].Attitude_pid, M6020s[2].targetAngle, M6020s[2].totalAngle);	
  	M6020s[2].outCurrent = Position_PID(&M6020s[2].Speed_pid, M6020s[2].inneroutCurrent , M6020s[2].realSpeed );	
	
	  //清标志位
		
		//PITCH轴PID
		M6020s[0].inneroutCurrent = Pitch_Position_PID(&M6020s[0].Attitude_pid, M6020s[0].targetAngle, M6020s[0].totalAngle);
	  M6020s[0].outCurrent = Pitch_Position_PID(&M6020s[0].Speed_pid, M6020s[0].inneroutCurrent, M6020s[0].realSpeed);
    M6020s[1].outCurrent = -M6020s[0].outCurrent;
	
	  //清标志位
		M6020s[0].M6020InfoUpdateFlag = 0;				
		M6020s[2].M6020InfoUpdateFlag = 0;				
	
		//设定电机电流值

    CloundMoto_setCurrent(M6020s[0].outCurrent,M6020s[1].outCurrent,M6020s[2].outCurrent);
}





/**
  * @brief  云台控制处理-带IMU模式
  * @param[in]  delta_yaw		航向角变化量
  *				delta_pitch		俯仰角变化量
  *				shoot	射击使能
  *				mode	模式 - 除Status_ControlOFF外，其他正常控制
  * @retval None
  */

void Cloud_processing_WithIMU(float delta_yaw, float delta_pitch, uint8_t mode){
	   
	  //读取Yaw轴机械角度，防止切换时候炸鸡
	  Cloud.targetYawLPF = Cloud.targetYawRaw = M6020s[2].totalAngle;
	  
		if(mode == ControlMode_OFF ||(dr16_data.DR16OffLineFlag)){
		//以当前位置为下次启动位置，防炸
		Cloud_setIMUPosForced(GY_6050[0].totalYaw, 0/*单轴陀螺仪，没有这个值*/);
		Cloud.targetPitchLPF = Cloud.targetPitchRaw = M6020s[1].totalAngle;
		Cloud_setCurrent(0, 0, 0, 0);
		return;
	  }

/*************传入增加量****************/
		Cloud.IMUtargetYawRaw += delta_yaw;
	  Cloud.targetPitchRaw += delta_pitch;
		
		
		GY_6050[0].ImuInfoUpdateFlag = 0;

/*************左右限幅****************/
		//计算云台IMU最小角度(左)
		Cloud.IMUYawAngleMin = GY_6050[0].totalYaw - (M6020_AngleToDEG(Cloud_Yaw_delta) - Cloud_getYawAngleWithRight());
    //计算云台IMU最大角度(右)		
	  Cloud.IMUYawAngleMax = GY_6050[0].totalYaw + Cloud_getYawAngleWithRight();
/*************云台限幅begin****************/

		if(Cloud.IMUtargetYawRaw > Cloud.IMUYawAngleMax)
		{
      Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMax;
		}
		if(Cloud.IMUtargetYawRaw < Cloud.IMUYawAngleMin)
		{
      Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMin;
		}
		if(Cloud.targetPitchRaw > Cloud_Pitch_Max){
			Cloud.targetPitchRaw = Cloud_Pitch_Max;
		}
		if(Cloud.targetPitchRaw < Cloud_Pitch_Min) {
			Cloud.targetPitchRaw = Cloud_Pitch_Min;
		}		
		
				
/*************云台限幅end****************/
		
/*************平滑处理****************/
		Filter_IIRLPF(&Cloud.IMUtargetYawRaw, &Cloud.IMUtargetYawLPF, 0.4);
//		Cloud.IMUtargetYawLPF = Cloud.IMUtargetYawRaw;
		Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, 0.1);
		
		M6020_setTargetAngle(M6020_PITCH_Right, Cloud.targetPitchLPF);
		
/*************PID计算****************/
		//YAW轴PID
		M6020s[2].inneroutCurrent = Following_Yaw_Position_PID(&Cloud.YawAttitude_pid, Cloud.IMUtargetYawLPF, GY_6050[0].totalYaw );
		//速度PID计算(内环)
		M6020s[2].outCurrent = Following_Yaw_Position_PID(&Cloud.YawSpeed_pid, M6020s[2].inneroutCurrent, GY_6050[0].Gyro.z);	
		//清标志位
		M6020s[2].M6020InfoUpdateFlag = 0;
		
		//角度PID计算(外环)
		//PITCH轴PID
		M6020s[0].inneroutCurrent = Pitch_Position_PID(&Cloud.PitchAttitude_pid, M6020s[0].targetAngle, M6020s[0].totalAngle);
		M6020s[0].outCurrent = Pitch_Position_PID(&Cloud.PitchSpeed_pid, M6020s[0].inneroutCurrent, GY_6050[0].Gyro.x);
	
    M6020s[1].outCurrent = -M6020s[0].outCurrent;
	
	  //清标志位
		M6020s[0].M6020InfoUpdateFlag = 0;				
		M6020s[1].M6020InfoUpdateFlag = 0;
		
//    Cloud.last_targetYawRaw = Cloud.IMUtargetYawLPF;

	CloundMoto_setCurrent(M6020s[0].outCurrent,M6020s[1].outCurrent,M6020s[2].outCurrent);

}


/**
  * @brief  云台Yaw轴跟随模式状态判断
  * @param[in]  None
  * @retval cloundmode 当前Yaw轴状态
  */
int Yawmode_Judge(void)
{
	int cloundmode;
  if((abs(Cloud.IMUtargetYawLPF - GY_6050[0].totalYaw) < 10)&&((Cloud.IMUtargetYawLPF == Cloud.last_IMUtargetYawLPF)))
	{
      cloundmode = Cloud_Stable;
	}
	if((abs(Cloud.IMUtargetYawLPF - GY_6050[0].totalYaw) > 1)&&((Cloud.IMUtargetYawLPF != Cloud.last_IMUtargetYawLPF)))
	{
      cloundmode = Cloud_Change;
	}
	if((abs(Cloud.IMUtargetYawLPF - GY_6050[0].totalYaw) > 10)&&((Cloud.IMUtargetYawLPF == Cloud.last_IMUtargetYawLPF)))
	{
      cloundmode = Cloud_Interfere;              
	}
	return cloundmode;
	
	
	//	if(Chassis.changeDirectionFlag == 1)
//	{
//		change_IMUtargetYawRaw = Cloud.last_targetYawRaw + 180.0f;
//		Cloud.targetPitchRaw += delta_pitch;
//				
///*************左右限幅****************/
//		//计算云台IMU最小角度(左)
//		Cloud.IMUYawAngleMin = GY_6050[0].totalYaw - (M6020_AngleToDEG(Cloud_Yaw_delta) - Cloud_getYawAngleWithRight());
//    //计算云台IMU最大角度(右)		
//	  Cloud.IMUYawAngleMax = GY_6050[0].totalYaw + Cloud_getYawAngleWithRight();
///*************云台限幅begin****************/

//		if(Cloud.IMUtargetYawRaw > Cloud.IMUYawAngleMax)
//		{
//      Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMax;
//		}
//		if(Cloud.IMUtargetYawRaw < Cloud.IMUYawAngleMin)
//		{
//      Cloud.IMUtargetYawRaw = Cloud.IMUYawAngleMin;
//		}
//		if(Cloud.targetPitchRaw > Cloud_Pitch_Max){
//			Cloud.targetPitchRaw = Cloud_Pitch_Max;
//		}
//		if(Cloud.targetPitchRaw < Cloud_Pitch_Min) {
//			Cloud.targetPitchRaw = Cloud_Pitch_Min;
//		}	
//		
///*************平滑处理****************/
//		Cloud.IMUtargetYawLPF = change_IMUtargetYawRaw;
//		Filter_IIRLPF(&Cloud.targetPitchRaw, &Cloud.targetPitchLPF, 0.1);
//		
//		M6020_setTargetAngle(M6020_PITCH_Right, Cloud.targetPitchLPF);
//		
///*************PID计算****************/
//		//YAW轴PID
//		M6020s[2].inneroutCurrent = Following_Yaw_Position_PID(&Cloud.YawAttitude_pid, Cloud.IMUtargetYawLPF, GY_6050[0].totalYaw );
//		//速度PID计算(内环)
//		M6020s[2].outCurrent = Following_Yaw_Position_PID(&Cloud.YawSpeed_pid, M6020s[2].inneroutCurrent, GY_6050[0].Gyro.z);	
//		//清标志位
//		M6020s[2].M6020InfoUpdateFlag = 0;
//		
//		//角度PID计算(外环)
//		//PITCH轴PID
//		M6020s[0].inneroutCurrent = Pitch_Position_PID(&Cloud.PitchAttitude_pid, M6020s[0].targetAngle, M6020s[0].totalAngle);
//		M6020s[0].outCurrent = Pitch_Position_PID(&Cloud.PitchSpeed_pid, M6020s[0].inneroutCurrent, GY_6050[0].Gyro.x);
//	
//    M6020s[1].outCurrent = -M6020s[0].outCurrent;
//	
//	  //清标志位
//		M6020s[0].M6020InfoUpdateFlag = 0;				
//		M6020s[1].M6020InfoUpdateFlag = 0;
//		
////    last_IMUtargetYawRaw = Cloud.IMUtargetYawLPF;

//    CloundMoto_setCurrent(M6020s[0].outCurrent,M6020s[1].outCurrent,M6020s[2].outCurrent);
//		
//		if(abs(change_IMUtargetYawRaw - GY_6050[0].totalYaw) < 2)
//		{
//		 Chassis.changeDirectionFlag  = 0;
//		 change_IMUtargetYawRaw = 0;
//		}
//		
//		return;

//	}

}	



/**
  * @brief  云台Pitch轴跟随模式状态判断
  * @param[in]  None
  * @retval cloundmode 当前Yaw轴状态
  */
int Pitchmode_Judge(int32_t temp_err)
{
	int cloundmode;
  static int last_cloundmode;
	
	if( temp_err > 0)
	{
		cloundmode = Cloud_Up;
	}
	if( temp_err < 0)
	{
		cloundmode = Cloud_Down;
	}
	if( temp_err == 0)
	{
		cloundmode = last_cloundmode;
	}
	
	last_cloundmode = cloundmode;

	return cloundmode;

}









