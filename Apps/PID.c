/**
  ******************************************************************************
  * @file    PID.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   PID功能函数
  ******************************************************************************
  */

#include "PID.h"

void abs_limit(float *a, float ABS_MAX){
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}

void IncrementalPID_paraReset(incrementalpid_t *pid_t, float kp, float ki, float kd, uint32_t MaxOutput, uint32_t IntegralLimit){
	pid_t->Target = 0;
	pid_t->Measured = 0;
	pid_t->err = 0;
	pid_t->err_last = 0;
	pid_t->err_beforeLast = 0;
	pid_t->Kp = kp;
	pid_t->Ki = ki;
	pid_t->Kd = kd;
	pid_t->MaxOutput = MaxOutput;
	pid_t->IntegralLimit = IntegralLimit;
	pid_t->pwm = 0; 			
}



float Incremental_PID(incrementalpid_t *pid_t, float target, float measured) {
	float p_out, i_out, d_out;
	pid_t->Target = target;
	pid_t->Measured = measured;
	pid_t->err = pid_t->Target - pid_t->Measured;
	
//	if(abs(pid_t->err)<0.1f)
//		pid_t->err = 0.0f;
		//return 0;
	
	p_out = pid_t->Kp*(pid_t->err - pid_t->err_last);
	i_out = pid_t->Ki*pid_t->err;
	d_out = pid_t->Kd*(pid_t->err - 2.0f*pid_t->err_last + pid_t->err_beforeLast);
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm += (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_beforeLast = pid_t->err_last;
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}


void PositionPID_paraReset(positionpid_t *pid_t, float kp_1, float ki_1, float kd_1, float kp_2, float ki_2, float kd_2,\
	uint32_t MaxOutput, uint32_t IntegralLimit){
	pid_t->Target = 0;
	pid_t->Measured = 0;
	pid_t->MaxOutput = MaxOutput;
	pid_t->IntegralLimit = IntegralLimit;
	pid_t->err = 0;
	pid_t->err_last = 0;
	pid_t->integral_err = 0;
	pid_t->Kp_1 = kp_1;
	pid_t->Ki_1 = ki_1;
	pid_t->Kd_1 = kd_1;
	pid_t->Kp_2 = kp_2;
	pid_t->Ki_2 = ki_2;
	pid_t->Kd_2 = kd_2;
	
	pid_t->pwm = 0; 			
}


float Position_PID(positionpid_t *pid_t, float target, float measured) {
	float p_out, i_out,d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;


	pid_t->err = pid_t->Target - pid_t->Measured;
//	pid_t->integral_err += pid_t->err;
	if(abs(pid_t->integral_err) < 300)
	{
		pid_t->integral_err += pid_t->err;
	}
	else
	{
		pid_t->integral_err = 300;
	}

	
	p_out = pid_t->Kp_1*pid_t->err;
	i_out = pid_t->Ki_1*pid_t->integral_err;
	d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);
	

	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}





/**
  * @brief  底盘跟随和扭腰PID控制函数
  * @param[in]  *pid_t		pid结构体指针
  *				       target   目标值
  *				       measured	测量值			
  * @retval pid_t->pwm
  */
float FollowSwing_Position_PID(positionpid_t *pid_t, float target, float measured) {
	float p_out, i_out,d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;


	pid_t->err = pid_t->Target - pid_t->Measured;
//	pid_t->integral_err += pid_t->err;
	if(abs(pid_t->integral_err) < 1000)
	{
		pid_t->integral_err += pid_t->err;
	}
	else
	{
		pid_t->integral_err = 1000;
	}

	
	p_out = pid_t->Kp_1*pid_t->err;
	i_out = pid_t->Ki_1*pid_t->integral_err;
	d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);
	

	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}







/**
  * @brief  云台拨盘PID控制函数
  * @param[in]  *pid_t		pid结构体指针
  *				       target   目标值
  *				       measured	测量值			
  * @retval pid_t->pwm
  */
float Dial_PositionPID(positionpid_t *pid_t, float target, float measured) {
	float p_out, i_out,d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;


	pid_t->err = pid_t->Target - pid_t->Measured;

	pid_t->integral_err += pid_t->err;

	
	p_out = pid_t->Kp_1*pid_t->err;
	i_out = pid_t->Ki_1*pid_t->integral_err;
	d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);	

	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}








/**
  * @brief  云台Pitch轴PID控制函数
  * @param[in]  *pid_t		pid结构体指针
  *				       target   目标值
  *				       measured	测量值			
  * @retval pid_t->pwm
  */
float Pitch_Position_PID(positionpid_t *pid_t, float target, float measured) {
	float p_out, i_out,d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;


	pid_t->err = pid_t->Target - pid_t->Measured;
//	pid_t->integral_err += pid_t->err;
	if(abs(pid_t->integral_err) < 300)
	{
		pid_t->integral_err += pid_t->err;
	}
	else
	{
		pid_t->integral_err = 300;
	}

	
	p_out = pid_t->Kp_1*pid_t->err;
	i_out = pid_t->Ki_1*pid_t->integral_err;
	d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);
	

	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;
}




/**
  * @brief  云台Yaw轴跟随PID控制函数
  * @param[in]  *pid_t		pid结构体指针
  *				       target   目标值
  *				       measured	测量值			
  * @retval pid_t->pwm
  */
float Yaw_Position_PID(positionpid_t *pid_t, float target, float measured) {
	
	//声明变量和赋值运算
	float p_out, i_out,d_out;
	static int pitch_time = 0;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;
	
	//判断是否进入了稳态
	if(pid_t->last_Target == pid_t->Target)
	{
//		if(abs(pid_t->err) < 50 )
//	     pitch_time++;
//	  else 
//		   pitch_time = 0;
      pitch_time++;		
	}
	else 
		pitch_time = 0;
	
	//计算误差
	pid_t->err = pid_t->Target - pid_t->Measured;
	
	//积分误差限幅
	if(abs(pid_t->integral_err) <= 600)
	{
		pid_t->integral_err += pid_t->err;
	}
	else
	{
		pid_t->integral_err = 600;
	}
	
  
	//根据现在的状态判断采用的p,Ki,Kd
	if(pitch_time>100)
	{
		p_out = pid_t->Kp_1*pid_t->err;
		i_out = pid_t->Ki_1*pid_t->integral_err;
		d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);
	}
	else
		p_out = pid_t->Kp_2*pid_t->err;
		i_out = pid_t->Ki_2*pid_t->integral_err;
		d_out = pid_t->Kd_2*(pid_t->err - pid_t->err_last);

	//清零防炸
	if(pitch_time>1000)
	{
	 pitch_time  = 101;
	}
	
	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	pid_t->err_last = pid_t->err;
  pid_t->last_Target = pid_t->Target;   
	
	return pid_t->pwm;

}






/**
  * @brief  云台Yaw轴跟随PID控制函数
  * @param[in]  *pid_t		pid结构体指针
  *				       target   目标值
  *				       measured	测量值			
  * @retval pid_t->pwm
  */
float Following_Yaw_Position_PID(positionpid_t *pid_t, float target, float measured) {
	float p_out, i_out,d_out;
	pid_t->Target = (float)target;
	pid_t->Measured = (float)measured;


	pid_t->err = pid_t->Target - pid_t->Measured;
//	pid_t->integral_err += pid_t->err;
	if(abs(pid_t->integral_err) < 300)
	{
		pid_t->integral_err += pid_t->err;
	}
	else
	{
		pid_t->integral_err = 300;
	}

	
	p_out = pid_t->Kp_1*pid_t->err;
	i_out = pid_t->Ki_1*pid_t->integral_err;
	d_out = pid_t->Kd_1*(pid_t->err - pid_t->err_last);
	

	//积分限幅
	abs_limit(&i_out, pid_t->IntegralLimit);
	
	pid_t->pwm = (p_out + i_out + d_out);
	
	//输出限幅
	abs_limit(&pid_t->pwm, pid_t->MaxOutput);
	
	pid_t->err_last = pid_t->err;

	return pid_t->pwm;	

}





