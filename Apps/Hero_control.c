/**
  ******************************************************************************
  * @file    Hero_control.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   弹丸发♂射控制函数接口
  ******************************************************************************
  */

#include "Hero_control.h"

//底盘云台参数
float Temp_Chassis_Vx = 0.0f, Temp_Chassis_Vy = 0.0f, Temp_Chassis_VOmega = 0.0f;
float Temp_Cloud_Vx = 0.0f,Temp_Cloud_Vy = 0.0f;
//按键参数
ramp_t fb_ramp = RAMP_GEN_DAFAULT;
ramp_t lr_ramp = RAMP_GEN_DAFAULT;
#define KEY_ACC_TIME     900  //ms

int small_shootcounter = 0;
int large_shootcounter = 0;
int Max_speed_counter = 0;
int FKFK = 1450;
int YKYK = 1500;
void PC_control(void)
{				
	/*
	键盘
	F:转换小弹丸射速
	R：进补给站
	SHIFT：加速
	Q E:自旋
  
	六盏灯
	1.是否为高速
	2.是否补给站模式
	3.射速
	
	
	三个数据
	1.弹药
	2.子弹弹量
	
	*/
	
	  /*******************************************射击begin**************************************************/
	      
	      if(dr16_mouseL_isPressedJumpKey(MOUSEKEY_LEFT,KEY_HAVE_MATCH))
				{
					Shooting.shootFlagSmall = 1;
				}
				else
				{
					Shooting.shootFlagSmall = 0;
				}				
				
				
				if(dr16_mouseR_isPressedJumpKey(MOUSEKEY_LEFT,KEY_HAVE_MATCH))
				{
					Shooting.shootFlagLarge = 1;

				}
				else
				{
					Shooting.shootFlagLarge = 0;
				}	
				
				
			 if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_F,KEY_HAVE_MATCH))
			 {
	        PCMode.fricSpeedCounter++;
				  //一开始为高速
				  if((PCMode.fricSpeedCounter%2) == 0) 
					{
            PCMode.fricSpeedFlag = 0;					  
					}	
				  if((PCMode.fricSpeedCounter%2) == 1)
					{
	           PCMode.fricSpeedFlag = 1;		
					}						
			 }	
/*******************************************射击end**************************************************/
				
/*******************************************参数切换begin**************************************************/
       //底盘速度切换
			 if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_SHIFT,KEY_HAVE_MATCH))
			 {
			    PCMode.chassisSpeedcoe = 2.0f;
			 }
			 if(dr16_keyboard_isReleaseJumpKey(KEYBOARD_PRESSED_SHIFT,KEY_HAVE_MATCH))
			 {
			    PCMode.chassisSpeedcoe = 1.0f;
			 }
			 
	     //云台速度切换		 
			 if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_CTRL,KEY_HAVE_MATCH))
			 {
	        PCMode.cloundSpeedcoe_x = 0.01f;
	        PCMode.cloundSpeedcoe_y = 0.1f;			 
			 }
			 if(dr16_keyboard_isReleaseJumpKey(KEYBOARD_PRESSED_CTRL,KEY_HAVE_MATCH))
			 {
					PCMode.cloundSpeedcoe_x = 0.025;
					PCMode.cloundSpeedcoe_y = 0.5f;
			 }			 
			 
			 //补给站模式切换
			 if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_R,KEY_HAVE_MATCH))
			 {
	        PCMode.normalControlCounter++;
				  PCMode.supplyFlag = 1;
				  if((PCMode.normalControlCounter%2) == 0)
					{
            PCMode.suppiyControl = 0;					  
					}	
				  if((PCMode.normalControlCounter%2) == 1)
					{
	           PCMode.suppiyControl = 1;		
					}						
			 }			 
	/*******************************************参数切换end**************************************************/
		 
				 
/*******************************************前后左右begin**************************************************/
				
			//前进后退	
       if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_W, KEY_HAVE_MATCH))
				{
					pcRocker.y = Chassis.speedLimit*PCMode.chassisSpeedcoe;

				}
			 else if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_S, KEY_HAVE_MATCH))
				{
					pcRocker.y = -Chassis.speedLimit*PCMode.chassisSpeedcoe;
				}
			 else
				{
					pcRocker.y = 0;
				}
				
			//左右平移	
			if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_D, KEY_HAVE_MATCH))
				{
					pcRocker.x = Chassis.speedLimit*PCMode.chassisSpeedcoe;
				}
			else if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_A, KEY_HAVE_MATCH))
				{
					 pcRocker.x = -Chassis.speedLimit*PCMode.chassisSpeedcoe;
				}
			else
				{
					pcRocker.x = 0;
				}
			
      //自旋				
			if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_Q, KEY_HAVE_MATCH))
				{
				  if(PCMode.suppiyControl)
			    {
						pcRocker.degrees = -5;
					}
					else
					  pcRocker.degrees = -100;
				}
			else if(dr16_keyboard_isPressedKey(KEYBOARD_PRESSED_E, KEY_HAVE_MATCH))
				{
					if(PCMode.suppiyControl)
			    {
						pcRocker.degrees = 5;
					}
					else
					 pcRocker.degrees = 100;
				}
			else
				{
					pcRocker.degrees = 0;
				}
				
				
/*******************************************前后左右end**************************************************/

	   //遥控模式，上拨杆	
		if(ControlStatus.ControlRight == ControlMode_High)
		{				
      //小发射速度选择			
			if(PCMode.fricSpeedFlag)
			{
				fric_Control(Shooting.fricLowSmasllSpeed,Shooting.fricLowSmasllSpeed,SetSmall);	
			}
			else
				fric_Control(Shooting.fricHighSmasllSpeed,Shooting.fricHighSmasllSpeed,SetSmall);					
      
			//大发射	
//       fric_Control(Shooting.fricLargeSpeed,Shooting.fricLargeSpeed,SetLarge);					
		}
		if(ControlStatus.ControlRight == ControlMode_Mid)
		{				
			fric_Control(0,0,SetSmall);
			fric_Control(0,0,TurnOffLarge);
		}
		if(ControlStatus.ControlRight == ControlMode_Low)
		{     
			    //1100 9
			    //1125 12
			    //1150 15
			    //1200 20
			    //1250 24
			    //1275 26
			    //1300 30
					fric_Control(1250,1250,SetSmall);	
		}
		
		Temp_Chassis_Vx = pcRocker.x;
		Temp_Chassis_Vy = pcRocker.y;
		Temp_Cloud_Vy = -dr16_data.mouse.y*PCMode.cloundSpeedcoe_y;
		Temp_Cloud_Vx = dr16_data.mouse.x*PCMode.cloundSpeedcoe_x  + pcRocker.degrees ;


}

void RC_control(void)
{
				Temp_Chassis_Vx = dr16_rocker_L.x*15.0f;
				Temp_Chassis_Vy = dr16_rocker_L.y*15.0f;	
				Temp_Cloud_Vy = dr16_rocker_R.y/10.0f;
				Temp_Cloud_Vx = dr16_rocker_R.x/250.0f;
	
		    Shooting.shootFlagSmall = 0;					
        Shooting.shootFlagLarge = 0;				

	      //遥控模式，上拨杆
				if(ControlStatus.ControlRight == ControlMode_High)
				{					
          small_shootcounter++;
					if(small_shootcounter > 10)
					{
					  Shooting.shootFlagSmall = 1;
						small_shootcounter = 0;
					}
					else
					{
					  Shooting.shootFlagSmall = 0;
					}
					 fric_Control(1125,1125,SetSmall);	
				 }

	      //遥控模式，中拨杆				 
				else if(ControlStatus.ControlRight == ControlMode_Mid)
				{				
					fric_Control(0,0,TurnOffLarge);		
					fric_Control(0,0,SetSmall);		
//          Bullet.LargeCounter	= 0;				
				}
				
	      //遥控模式，下拨杆				 			
				else
				{
					
//					small_shootcounter++;
//					if(small_shootcounter > 50)
//					{
//					  Shooting.shootFlagSmall = 1;
//						small_shootcounter = 0;
//					}
//					else
//					{
//					  Shooting.shootFlagSmall = 0;
//					}
//					 fric_Control(1450,1500,SetSmall);		
          large_shootcounter++;
					if(large_shootcounter > 200)
					{
					  Shooting.shootFlagLarge = 1;
						large_shootcounter = 0;
					}
					else
					{
						
					  Shooting.shootFlagLarge = 0;

					}
					 fric_Control(1280,1280,SetLarge);		
				}	
				
				
				
				

				

}


void Hero_control(void){
	
	 	switch(ControlStatus.ControlLeft)
		{
			case ControlMode_ByRC:
		    RC_control();
				break;

		   case ControlMode_ByPC:			
			  PC_control();
			 break;
		}
		
		
		if(M3508s[1].realSpeed == 0)
		{
		  Max_speed_counter = 0;
		}
		else
			Max_speed_counter++;
		
		
		//射击控制函数
   	Bullet_Shooting();
		
			  	

			
		//陀螺仪离线
		if(GY_6050[0].ImuOffLineFlag)
		{
			//云台固定
			Cloud_static(Cloud_Yaw_Center, -Temp_Cloud_Vy, ControlStatus.ControlLeft);
			Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Cloud_Vx*3750.0f, ControlStatus.ControlLeft,ControlStatus.ControlRight);					
		}
		//陀螺仪没有离线		
		else
		{
		  if(PCMode.suppiyControl)
			{
				//补给站模式
				Cloud_static(Cloud_Yaw_Center, -Temp_Cloud_Vy, ControlStatus.ControlLeft);
				Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Cloud_Vx*1000.0f, ControlStatus.ControlLeft,ControlStatus.ControlRight);							
			  if(PCMode.supplyFlag)
		    {
					TIM_SetCompare3(TIM4, 1300);
					PCMode.supplyFlag = 0;
				}
				
			}
			else
			{						
			  if(PCMode.supplyFlag)
		    {
					TIM_SetCompare3(TIM4, 2000);
					PCMode.supplyFlag = 0;
				}			
				
				Cloud_processing_WithIMU(Temp_Cloud_Vx, Temp_Cloud_Vy, ControlStatus.ControlLeft);
				//底盘分解
				float YawRad = Cloud_getYawAngleWithCenter()*DEG_TO_RAD;
				float temp_Vx = Temp_Chassis_Vx;
				Temp_Chassis_Vx = Temp_Chassis_Vx*cos(YawRad) - Temp_Chassis_Vy*sin(YawRad);
				Temp_Chassis_Vy = Temp_Chassis_Vy*cos(YawRad) + temp_Vx*sin(YawRad);
				
				Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Chassis_VOmega, ControlStatus.ControlLeft,ControlStatus.ControlRight);					
			}			
		}


}





