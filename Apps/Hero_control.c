/**
  ******************************************************************************
  * @file    Hero_control.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   ���跢������ƺ����ӿ�
  ******************************************************************************
  */

#include "Hero_control.h"

//������̨����
float Temp_Chassis_Vx = 0.0f, Temp_Chassis_Vy = 0.0f, Temp_Chassis_VOmega = 0.0f;
float Temp_Cloud_Vx = 0.0f,Temp_Cloud_Vy = 0.0f;
//��������
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
	����
	F:ת��С��������
	R��������վ
	SHIFT������
	Q E:����
  
	��յ��
	1.�Ƿ�Ϊ����
	2.�Ƿ񲹸�վģʽ
	3.����
	
	
	��������
	1.��ҩ
	2.�ӵ�����
	
	*/
	
	  /*******************************************���begin**************************************************/
	      
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
				  //һ��ʼΪ����
				  if((PCMode.fricSpeedCounter%2) == 0) 
					{
            PCMode.fricSpeedFlag = 0;					  
					}	
				  if((PCMode.fricSpeedCounter%2) == 1)
					{
	           PCMode.fricSpeedFlag = 1;		
					}						
			 }	
/*******************************************���end**************************************************/
				
/*******************************************�����л�begin**************************************************/
       //�����ٶ��л�
			 if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_SHIFT,KEY_HAVE_MATCH))
			 {
			    PCMode.chassisSpeedcoe = 2.0f;
			 }
			 if(dr16_keyboard_isReleaseJumpKey(KEYBOARD_PRESSED_SHIFT,KEY_HAVE_MATCH))
			 {
			    PCMode.chassisSpeedcoe = 1.0f;
			 }
			 
	     //��̨�ٶ��л�		 
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
			 
			 //����վģʽ�л�
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
	/*******************************************�����л�end**************************************************/
		 
				 
/*******************************************ǰ������begin**************************************************/
				
			//ǰ������	
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
				
			//����ƽ��	
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
			
      //����				
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
				
				
/*******************************************ǰ������end**************************************************/

	   //ң��ģʽ���ϲ���	
		if(ControlStatus.ControlRight == ControlMode_High)
		{				
      //С�����ٶ�ѡ��			
			if(PCMode.fricSpeedFlag)
			{
				fric_Control(Shooting.fricLowSmasllSpeed,Shooting.fricLowSmasllSpeed,SetSmall);	
			}
			else
				fric_Control(Shooting.fricHighSmasllSpeed,Shooting.fricHighSmasllSpeed,SetSmall);					
      
			//����	
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

	      //ң��ģʽ���ϲ���
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

	      //ң��ģʽ���в���				 
				else if(ControlStatus.ControlRight == ControlMode_Mid)
				{				
					fric_Control(0,0,TurnOffLarge);		
					fric_Control(0,0,SetSmall);		
//          Bullet.LargeCounter	= 0;				
				}
				
	      //ң��ģʽ���²���				 			
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
		
		
		//������ƺ���
   	Bullet_Shooting();
		
			  	

			
		//����������
		if(GY_6050[0].ImuOffLineFlag)
		{
			//��̨�̶�
			Cloud_static(Cloud_Yaw_Center, -Temp_Cloud_Vy, ControlStatus.ControlLeft);
			Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Cloud_Vx*3750.0f, ControlStatus.ControlLeft,ControlStatus.ControlRight);					
		}
		//������û������		
		else
		{
		  if(PCMode.suppiyControl)
			{
				//����վģʽ
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
				//���̷ֽ�
				float YawRad = Cloud_getYawAngleWithCenter()*DEG_TO_RAD;
				float temp_Vx = Temp_Chassis_Vx;
				Temp_Chassis_Vx = Temp_Chassis_Vx*cos(YawRad) - Temp_Chassis_Vy*sin(YawRad);
				Temp_Chassis_Vy = Temp_Chassis_Vy*cos(YawRad) + temp_Vx*sin(YawRad);
				
				Chassis_processing(Temp_Chassis_Vx, Temp_Chassis_Vy, Temp_Chassis_VOmega, ControlStatus.ControlLeft,ControlStatus.ControlRight);					
			}			
		}


}





