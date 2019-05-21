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

uint8_t Devi_Offline = 0;
uint8_t JKJK = 0;
/**
  * @brief  外接设备帧率清零，每200ms清零一次
  * @param  None
  * @retval None
  */
void DevicesMonitor_update(void){
	
  static uint8_t Devi_counter_low = 0;
  static uint8_t Devi_counter_fast = 0;
  static uint8_t Devi_counter_LED;

	
	Devi_counter_low++;
	Devi_counter_fast++;
	Devi_counter_LED++;
  JKJK = Devi_counter_low;	
	//CAN发送频率较快，50毫秒检测一次
	if(Devi_counter_fast > 5)
	{
		//M3508
			for(int i = 0; i<5; i++){
				if(M3508s[i].M3508InfoUpdateFrame < 5){
					M3508s[i].M3508OffLineFlag = 1;
					Devi_Offline |= 0x01; 
				}
				else{
					M3508s[i].M3508OffLineFlag = 0;
					Devi_Offline &= ~1; 					
				}
				M3508s[i].M3508InfoUpdateFrame = 0;
			}

			for(int i = 0; i<3; i++){
				if(M2006s[i].M2006InfoUpdateFrame < 5){
					M2006s[i].M2006OffLineFlag = 1;
					Devi_Offline |= 0x02; 
				}
				else{
					M2006s[i].M2006OffLineFlag = 0;
					Devi_Offline &= ~(1<<1);
				}
				M2006s[i].M2006InfoUpdateFrame = 0;
			}	

			for(int i = 0; i<3; i++){
				if(M6020s[i].M6020InfoUpdateFrame < 5){
					M6020s[i].M6020OffLineFlag = 1;
					Devi_Offline |= 0x04; 
				}
				else{
					M6020s[i].M6020OffLineFlag = 0;
					Devi_Offline &= ~(1<<2);
					
				}
				M6020s[i].M6020InfoUpdateFrame = 0;
			}	
			
			//计算器清零
			Devi_counter_fast = 0;
		}
	
		//速度较慢，300毫秒检测一次
		if(Devi_counter_low>20)
		{
			if(GY_6050[0].ImuInfoUpdateFrame < 4){
					GY_6050[0].ImuOffLineFlag = 1;
					Devi_Offline |= 0x08; 
			}
			else{
				GY_6050[0].ImuOffLineFlag = 0;
				Devi_Offline &= ~(1<<3);
			}
			if(GY_6050[0].ImuInfoUpdateFrame > 2)
			{
				GY_6050[0].ImuInfoUpdateFrame = 0;
		  }
		
			if(dr16_data.DR16InfoUpdateFrame < 5){
				dr16_data.DR16OffLineFlag = 1;
        Devi_Offline |= 0x10; 				
			}
			else{
				dr16_data.DR16OffLineFlag = 0;
				Devi_Offline &= ~(1<<4);
				
			}
			dr16_data.DR16InfoUpdateFrame = 0;
			
			//计算器清零			
		  Devi_counter_low = 0;
		}			
		

		
		
//		DevicesLED(Devi_Offline);

		
}

void LEDRED_Twinkle(uint8_t Twinkle)
{
//	for(int i=0; i < Twinkle; )
//	{
//		if(ledcounter>50)
//		{  
//			LEDRED_TOGGLE;
//			ledcounter = 0;
//			i++;
//		}
//	}
}	

void DevicesLED(uint8_t Device_mode)
{
	if(Device_mode == 0)
	{
	  LED_GREEN(0);
	}
	
  if(Device_mode &= 1)
	{
	  LEDRED_Twinkle(1);
	}
	
  if(Device_mode &= (1<<1))
	{
	  LEDRED_Twinkle(2);
	}

	if(Device_mode &= (1<<2))
	{
		LEDRED_Twinkle(3);
	}
	
  if(Device_mode &= (1<<3))
	{
		LEDRED_Twinkle(4);
	  
	}

	if(Device_mode &= (1<<4))
	{
		LEDRED_Twinkle(5);	  
	}
}











