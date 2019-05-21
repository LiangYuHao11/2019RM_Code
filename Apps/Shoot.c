/**
  ******************************************************************************
  * @file    Shoot.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   弹丸发♂射控制函数接口
  ******************************************************************************
  */
  
  
#include "Shoot.h"

shootGun_t Shooting;

								 
/**
  * @brief  射击参数初始化
  * @param[in]  None
  * @retval None
  */
void Shoot_Init(void){
	
	TIM12_PWMOutput(89, 19999,1000);	
  TIM4_PWMOutput(89, 19999,1000);	
  Shooting.controlSpeed = 0.2f;
	M3508s[4].targetSpeed = 80.0f;
	
	PositionPID_paraReset(&M2006s[1].dial_Speed, 0.8, 0.0f, 0.6f,0.0f, 0.0f, 0.0f,  8000, 600);
	PositionPID_paraReset(&M2006s[1].dial_Angle, 0.6, 0.0, 0.9f,0.0f, 0.0f, 0.0f,  10000, 300);	
	PositionPID_paraReset(&M2006s[0].dial_Speed, 0.675f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 9500, 600);
	PositionPID_paraReset(&M2006s[0].dial_Angle, 0.1f, 0.0f, 0.2f,0.0f, 0.0f, 0.0f,  6000, 300);
	PositionPID_paraReset(&M2006s[2].dial_Speed, 0.675f, 0.01f, 0.0f,0.0f, 0.0f, 0.0f, 9500, 600);
	PositionPID_paraReset(&M2006s[2].dial_Angle, 0.0625f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f,  6000, 300);	
//	PositionPID_paraReset(&M3508s[4].Deil_Speed_pid, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 8000, 600);
//	PositionPID_paraReset(&M3508s[4].Dial_Angle_pid, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 8000, 300);	
	IncrementalPID_paraReset(&M3508s[4].pid, 10.0f, 0.5f, 0.0f, 8000, 1000);
}



/**
  * @brief  设置小摩擦轮电机速度
  * @param  speed 	电机速度，范围1000~2000
  * @retval None
  */
void SmallfricMotor_setSpeed(uint16_t speed1,uint16_t speed2){
			
	if(speed1 < FRICMOTOR_SPEEDMIN) speed1 = FRICMOTOR_SPEEDMIN;
	if(speed2 > FRICMOTOR_SPEEDMAX) speed2 = FRICMOTOR_SPEEDMAX;
	
	if(speed1 < FRICMOTOR_SPEEDMIN) speed1 = FRICMOTOR_SPEEDMIN;
	if(speed2 > FRICMOTOR_SPEEDMAX) speed2 = FRICMOTOR_SPEEDMAX;	
		
	TIM_SetCompare1(TIM4, speed1);
	TIM_SetCompare2(TIM4, speed2);

}	

void SmallfricMotor_setSpeed2(uint16_t speed1,uint16_t speed2){
	
	static int noral_speed1 = 1000;
	static int noral_speed2 = 1000;
	
	if(speed1 == 0) noral_speed1=1000;
	if(speed2 == 0) noral_speed2=1000;
	
	if(speed1 < FRICMOTOR_SPEEDMIN) speed1 = FRICMOTOR_SPEEDMIN;
	if(speed2 > FRICMOTOR_SPEEDMAX) speed2 = FRICMOTOR_SPEEDMAX;
	
	if(speed1 > noral_speed1) noral_speed1+=5;
	if(speed2 > noral_speed2) noral_speed2+=5;

	if(speed1 < noral_speed1) noral_speed1-=5;
	if(speed2 < noral_speed2) noral_speed2-=5;	
	
	TIM_SetCompare1(TIM4, noral_speed1);
	TIM_SetCompare2(TIM4, noral_speed2);

}	

/**
  * @brief  设置大摩擦轮电机速度
  * @param  speed 	电机速度，范围1000~2000
  * @retval None
  */
void LargefricMotor_setSpeed(uint16_t speed1,uint16_t speed2){
	if(speed1 < FRICMOTOR_SPEEDMIN) speed1 = FRICMOTOR_SPEEDMIN;
	if(speed2 > FRICMOTOR_SPEEDMAX) speed2 = FRICMOTOR_SPEEDMAX;
	

	TIM_SetCompare1(TIM12, speed1);
	TIM_SetCompare2(TIM12, speed2);
}


/**
  * @brief  选择大小摩擦轮电机的状态
  * @param  snail_spee 	电机速度，范围1000~2000
  *   @param  	TurnOnLarge
	*             TurnOffLarge 
	*             SetLarge
	*						  TurnOnSmall
	*						  TurnOffSmall
	*						  SetSmall
	*						  ResetSmall
	*						  ResetLarge   选择摩擦轮的类型和模式
  * @retval None
  */
void fric_Control(uint16_t snail_speed1,uint16_t snail_speed2,int mode)
{
	if((ControlStatus.ControlLeft == ControlMode_Low)||(dr16_data.DR16OffLineFlag))
	{
	   	LargefricMotor_setSpeed(1000,1000);
		  SmallfricMotor_setSpeed2(0,0);
	}
	
	static uint16_t SmallBulletCount = 0;
	static uint16_t LargeBulletCount = 0;
  switch(mode)
	{	
				case TurnOnLarge:				
							if(LargeBulletCount < 20)
							{
								LargefricMotor_setSpeed(1000,1000);
								LargeBulletCount +=1; 
							}
				break;
				
				case TurnOffLarge:
					
            	LargefricMotor_setSpeed(1000,1000);
				break;
				
				case SetLarge:
					
               LargefricMotor_setSpeed(snail_speed1,snail_speed2);
				break;
				
				case TurnOnSmall:				
							if(SmallBulletCount < 20)
							{
								SmallfricMotor_setSpeed(1000,1000);
								SmallBulletCount +=1;								
							}
				break;
				
				case TurnOffSmall:
					
            	SmallfricMotor_setSpeed(1000,1000);
				break;
				
				case SetSmall:
					
               SmallfricMotor_setSpeed2(snail_speed1,snail_speed2);
				break;
				
				case ResetSmall:
					
               SmallBulletCount = 0;
				break;
				
				case ResetLarge:
					
               LargeBulletCount = 0;
				break;
				
	}
}




/**
  * @brief  完成小摩擦轮拨弹射击的动作
  * @param  None
  * @retval None
  */
int NKNK = 0;
float Small_Lasttarget = 0.0f;
int Small_Shoottime = 0;
int Small_ShootCheck = 1;
void Smallshoot_processing(void){
	
	//遥控器离线时卡死拨盘来防止车跑飞乱发射子弹
	if(dr16_data.DR16OffLineFlag == 1)
	{
		M2006s[1].targetAngle = Small_Lasttarget;
	}
	
	//当有射弹请求时拨盘开始转动
	if(Shooting.shootFlagSmall && Small_ShootCheck && Shooting.shootReadySmall)
	{
  	M2006s[1].targetAngle += SMALL_LOADANGLE;
		NKNK ++;
	}
	//当此状态保持50毫秒判断为卡弹
	if((M2006s[1].dial_Angle.err > 8192)&& (abs(M2006s[1].realSpeed) <= 50)&& (abs(M2006s[1].dial_Angle.pwm) > 9000))
	{
		//当不处于关闭摩擦轮状态才进行计时加加
		  Small_Shoottime ++;
			Small_ShootCheck = 0;
	}
	else /*if((abs(M2006s[1].targetAngle-M2006s[1].totalAngle) < 100)&&(M2006s[1].realSpeed) < 50)*/
	{
		Small_Shoottime = 0;
		Small_ShootCheck = 1;
	}
	
	if(Small_Shoottime >= 5)
	{
		M2006s[1].targetAngle -= 3*SMALL_LOADANGLE;
	}
	

	Small_Lasttarget = M2006s[1].targetAngle;
	//当遥控器右边拨到2时可以手动调节拨盘位置
  if(M2006s[1].M2006InfoUpdateFlag){
	M2006s[1].inneroutCurrent = Position_PID(&M2006s[1].dial_Angle, M2006s[1].targetAngle, M2006s[1].totalAngle);
	//速度PID计算(内环)
	M2006s[1].outCurrent = Position_PID(&M2006s[1].dial_Speed, M2006s[1].inneroutCurrent, M2006s[1].realSpeed);
	}

	if(abs(M2006s[1].turnCount) >= 100)
	{
		M2006s[1].targetAngle -= (8192 * M2006s[1].turnCount);
		M2006s[1].turnCount = 0;
	}
	
	M2006s[1].M2006InfoUpdateFlag = 0;
}
int largecounter = 20;
int largecounterflag = 1;
float Large_Lasttarget = 0.0f;
int Large_Shoottime = 0;
int Large_ShootCheck = 1;
int Large_Deiltime = 0;
void Large_processing(void)
{
	
/****************************************防止大弹丸连续被按****************************************/	
	  if(largecounterflag)
		{
		  largecounter++;
		}
		
		if(largecounter > 20)
		{
			if((Shooting.shootFlagLarge)&&(Shooting.shootReadyLarge)&&Large_ShootCheck)
			{
						M2006s[0].targetAngle -= LARGE_LOADANGLE;		
						largecounterflag = 1;
				    largecounter = 0;
			}
	  }
		
	
	if((abs(M2006s[0].dial_Angle.pwm) > 4500)&& (abs(M2006s[0].realSpeed) <= 10))
	{
		//当不处于关闭摩擦轮状态才进行计时加加
		  Large_Shoottime ++;
			Large_ShootCheck = 0;
	}
	else 
	{
		Large_Shoottime = 0;
		Large_ShootCheck = 1;
	}
	
  if(Large_Shoottime >= 50)
	{
		M2006s[0].targetAngle += LARGE_LOADANGLE;
	}
	
	if(M2006s[0].M2006InfoUpdateFlag == 1){
	M2006s[0].inneroutCurrent = Dial_PositionPID(&M2006s[0].dial_Angle, M2006s[0].targetAngle, M2006s[0].totalAngle);
  //速度PID计算(内环)
	M2006s[0].outCurrent = Dial_PositionPID(&M2006s[0].dial_Speed, M2006s[0].inneroutCurrent, M2006s[0].realSpeed);	

	M2006s[0].M2006InfoUpdateFlag = 0;
	}
	
	if(abs(M2006s[0].turnCount) >= 100)
	{
		M2006s[0].targetAngle -= (8192 * M2006s[0].turnCount);
		M2006s[0].turnCount = 0;	
	}
	
	
/************************************************大拨盘旋转函数************************************************/
	Bullet.bulletStatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	
	if(Bullet.bulletStatus != Bullet.lastBulletStatus)
	{
		if(Bullet.bulletStatus == 0)
		{
			Bullet.LargeCounter++;	
		}
	}
	
	if(Shooting.largeCheckFlag)
	{
		
	  Bullet.LargeCounter--;	
    Shooting.largeCheckFlag = 0;
	}
	
	if((abs(M3508s[4].outCurrent) > 7000) && (abs(M3508s[4].realSpeed) < 10))
	{
	  Large_Deiltime++;
	}
	
	if(Large_Deiltime > 50)
	{
	  M3508s[4].targetSpeed = -M3508s[4].targetSpeed;
		Large_Deiltime = 0;
	}
		
	
	if(Bullet.LargeCounter < 3)
	{
		M3508s[4].outCurrent = Incremental_PID(&M3508s[4].pid, M3508s[4].targetSpeed, M3508s[4].realSpeed); 
	}
	else
		M3508s[4].outCurrent = Incremental_PID(&M3508s[4].pid, 0.0F, M3508s[4].realSpeed);
	
	Bullet.lastBulletStatus = Bullet.bulletStatus;
	
}

/**
  * @brief  完成大拨盘拨弹的动作
  * @param  None
  * @retval None
  */
void Bullet_Shooting(void){
 	
	
	if(ControlStatus.ControlLeft == ControlMode_OFF  || (dr16_data.DR16OffLineFlag))
	{
		M3508s[4].turnCount = 0;
		M3508s[4].targetAngle = M3508s[4].totalAngle = M3508s[4].realAngle;
		M2006s[1].turnCount = 0;
		M2006s[1].targetAngle = M2006s[1].totalAngle = M2006s[1].realAngle;
		M2006s[0].turnCount = 0;
		M2006s[0].targetAngle = M2006s[0].totalAngle = M2006s[0].realAngle;
		DeilMoto_setCurrent(0,0,0);

	} 
	else
	{
		Smallshoot_processing();
    Large_processing();		
		DeilMoto_setCurrent(M2006s[0].outCurrent, M2006s[1].outCurrent,M3508s[4].outCurrent);
	}
	
}


