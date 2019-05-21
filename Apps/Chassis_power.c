#include "Chassis_power.h"

static int Power_counter = 0;
Power_t Power;


int i = 0;
void Power_control(float power)
{
	
	for(i = 9;i >= 0 ; i--)
  {
		Power.P_slope[i] = Power.P_value[i] - Power.P_value[i+1];
    Power.P_value[i+1] =  Power.P_value[i] ;			 
	}
  Power.P_value[0] = power;
	
	
	for(int k = 0;k < 8;k++)
	{
		if((Power.P_slope[k]) > 0)
		{
			if(Power.P_slope[k] > Power_slopeMax)
			{
				Power_counter++;
			}
		}
		
		if(Power_counter >7)
		{
      Chassis.PowerOverflowFlag = 1;
		}

	}

}

int8_t Chassis_Judge(float Vx, float Vy)
{
  int8_t 	Temp_Chassismode = 0;
	
	if((abs(M3508s[0].realSpeed)<20)&&(abs(M3508s[1].realSpeed)<20)&&(abs(M3508s[2].realSpeed)<20)&&\
		(abs(M3508s[3].realSpeed)<20)&&((Vx <20)||(Vy < 20)))
	{
	  Temp_Chassismode = Chassis_static;
	}

	if((Power.Last_Chassismode==Chassis_static)&&((Vx > 50)||(Vy > 50)))  
	{
	  Temp_Chassismode = Chassis_start;
		Power.Chassissetup_flag = 1;
	}
	
	Power.Last_Chassismode = Temp_Chassismode;
	return Temp_Chassismode;

}
int KKKKK =4096;
int16_t Constant_Power_Setup(int16_t outCurrent,int16_t all_outCurrent)
{	
  int16_t Constant_Power_outCurrent = 0;
  int16_t Param = 0;
	Param = KKKKK/all_outCurrent;
  Constant_Power_outCurrent = outCurrent*Param+10;
  return Constant_Power_outCurrent;
	
}

/**
  * @brief  获取缓存池现在的状态
  * @param[in]  Temp_buffer		当前缓冲能量
  * @retval None
  */
void  Buffer_Status(uint16_t Temp_buffer)
{
   if(Temp_buffer >= Buffer_Max)
	 {
     Power.PollStatus = Poll_Full;
		 return;
	 }
	 
   if((Temp_buffer < Buffer_Max)&&(Temp_buffer >= Power.high_water_level))
   {
	   Power.PollStatus = Poll_High;
	 }
	 
   if((Temp_buffer < Power.high_water_level)&&(Temp_buffer >= Power.mid_water_level))
   {
	   Power.PollStatus = Poll_Mid;	 
	 }

   if((Temp_buffer < Power.mid_water_level)&&(Temp_buffer >= Power.low_water_level))
   {
	   Power.PollStatus = Poll_Low;	 	   
	 }	 
	 
   if(Temp_buffer < Power.low_water_level)
	 {
	   Power.PollStatus = Poll_Empty;	 	    
	 }

}


































