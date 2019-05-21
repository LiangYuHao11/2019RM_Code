/**
	|--------------------------------- Copyright --------------------------------|
	|                                                                            |
	|                      (C) Copyright 2019,海康平头哥,                         |
	|           1 Xuefu Rd, Huadu Qu, Guangzhou Shi, Guangdong Sheng, China      |
	|                           All Rights Reserved                              |
	|                                                                            |
	|           By(GCU The wold of team | 华南理工大学广州学院机器人野狼队)         |
	|                    https://github.com/GCUWildwolfteam                      |
	|----------------------------------------------------------------------------|
	|--FileName    : power_buffer_pool.c                                              
	|--Version     : v1.0                                                          
	|--Author      : 海康平头哥                                                     
	|--Date        : 2019-03-24             
	|--Libsupports : 
	|--Description :                                                     
	|--FunctionList                                                     
	|-------1. ....                                                     
	|          <version>:                                                     
	|     <modify staff>:                                                       
	|             <data>:                                                       
	|      <description>:                                                        
	|-------2. ...                                                       
	|-----------------------------declaration of end-----------------------------|
 **/
#include "power_buffer_pool.h"


#define POOL_FULL  0x01
#define POOL_PADDING 0x02
#define POOL_EMPTY  0x03
#define POOL_HIGH   0x04
#define POOL_MID    0x05
#define POOL_LOW    0x06
#define POOL_PASS 0x07


powerBufferPoolStruct powerBufferPool_t;


  /**
  * @Data    2019-03-25 00:08
  * @brief   初始化功率缓存池
  * @param   void
  * @retval  void
  */
void PowerBufferPoolInit(void)
{
//    powerBufferPool_t.pcurrentMeter_t = &currtenMeter_t;
    powerBufferPool_t.max_p = 80.0;
    powerBufferPool_t.max_w = 0.0;//无电流计 暂时不需要
    powerBufferPool_t.r_p = 0.0;//无电流计 暂时不需要
    powerBufferPool_t.r_w = 200.0;//从裁判系统读值
    powerBufferPool_t.current_mapp_coe = 0.00122;//电流映射系数
//    powerBufferPool_t.high_water_level = 20.0;//4.8;//240.0;
//    powerBufferPool_t.low_water_level = 13.3; //2.12;//106.0;
//    powerBufferPool_t.mid_water_level = 8.8;  //160.0;
	
    powerBufferPool_t.high_water_level = 40.0;//4.8;//240.0;
    powerBufferPool_t.low_water_level = 28.3; //2.12;//106.0;
    powerBufferPool_t.mid_water_level = 15.8;  //160.0;	
    powerBufferPool_t.period = 0.01;//无电流计 暂时不需要
	   powerBufferPool_t.high_current_threshold = 15.0;//裁判系统测的电流峰值9A
    powerBufferPool_t.mid_current_threshold = 11.5;//裁判系统测的电流峰值7A
    powerBufferPool_t.low_current_threshold = 6.4;//裁判系统测的电流峰值7A
    powerBufferPool_t.safe_current_threshold = 3.8;//裁判系统测的电流峰值3.5A
	
	 //5.10 爬坡数据
    powerBufferPool_t.ramp_high_water_level = 35.0;//4.8;//240.0;
    powerBufferPool_t.ramp_low_water_level = 20.3; //2.12;//106.0;
    powerBufferPool_t.ramp_mid_water_level = 10.8;  //160.0;		
    powerBufferPool_t.ramp_high_current_threshold = 23.0;//裁判系统测的电流峰值9A
    powerBufferPool_t.ramp_mid_current_threshold = 14.5;//裁判系统测的电流峰值7A
    powerBufferPool_t.ramp_low_current_threshold = 10.4;//裁判系统测的电流峰值7A
    powerBufferPool_t.ramp_safe_current_threshold = 6.2;//裁判系统测的电流峰值3.5A
	
//    powerBufferPool_t.high_current_threshold = 10.0;//A
//    powerBufferPool_t.mid_current_threshold = 6.5;//A
//    powerBufferPool_t.low_current_threshold = 4.4;//A
//    powerBufferPool_t.safe_current_threshold = 3.3;//A	
}

  /**
  * @Data    2019-03-24 22:56
  * @brief   往缓存池注入
  * @param   void
  * @retval  void
  */
uint8_t Inject(powerBufferPoolStruct* pbs)
{
	uint8_t status = 0;
  pbs->r_p =pbs->pcurrentMeter_t->current * pbs->pcurrentMeter_t->volt;
	if(pbs->r_p <= pbs->max_p)
	{
		pbs->r_w = pbs->r_w - ((pbs->r_p - pbs->max_p)*pbs->period);
		status = POOL_PADDING;
	}
	else if(pbs->r_p > pbs->max_p)
	{
		 pbs->r_w = pbs->r_w - ((pbs->r_p - pbs->max_p)*pbs->period);
		status = POOL_PASS;
	}
	if(pbs->r_w >= pbs->max_w)
	{
		pbs->r_w =  pbs->max_w;
		status = POOL_FULL;
	}
	return status;
 }
/**
	* @Data    2019-03-24 19:59
	* @brief   pid输出值映射电流
	* @param   void
	* @retval  void 单位A
	*/
float OutMapCurrent(float coe,int16_t input)
{
	//-16384 ~ 0 ~ 16384,3508
	return (float)(coe*input);
}
/**
	* @Data    2019-03-24 21:14
	* @brief   电流映射pid输出值
	* @param   void
	* @retval  void
	*/
	int16_t CurrentMapOut(float coe,float current)
	{
		return (int16_t)(current / (float)(coe));
	}
/**
	* @Data    2019-03-24 20:18
	* @brief   缓存池出水口 //待完善
	* @param   input 输出量
	* @retval  void  
	*/
int16_t cur;//单位A
float time_coe=0.001;//单位ms
uint8_t power_state;	
uint8_t pid_state;
float Chassis_Angle = 0;
int16_t GetOutlet(powerBufferPoolStruct* pbs,int16_t input)
{
/******************传入的参数：1.功率池结构体指针  2.电流总和**********************/
	//获取现在的状态
  uint8_t state;
	Chassis_Angle = GY_6050[0].EularDate.Pitch - ((Cloud_Pitch_Center - M6020s[0].realAngle)/22.75f);
	state = GetPowerPoolState(pbs,abs(Chassis_Angle));
	
	if(abs(Chassis_Angle) < 10.0f)
	{
	  pid_state = 1;
	}
	else
	  pid_state = 2;
		
	power_state =  state;
	//将-16384~16384的电流值转换为-20A~20A
  cur = OutMapCurrent(pbs->current_mapp_coe,input);
	
	
	//取反，都采用正值计算
  if(cur<0)
  {
    pbs->current_mapp_coe = -pbs->current_mapp_coe;
  }
	
	//选择状态
	if(abs(Chassis_Angle) < 10.0f)
	{
		switch (state) 
			{
				case POOL_HIGH:
				if(abs(cur) > pbs->high_current_threshold)
				{
					//将-20A~20A的电流值转换为-16384~16384
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->high_current_threshold);
				}
					break;
				
				case POOL_MID:
				if(abs(cur) > pbs->mid_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->mid_current_threshold);
				}
					break;
				
				case POOL_LOW:
				if(abs(cur) > pbs->low_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->low_current_threshold);
				}
					break;
				
				case POOL_EMPTY:
				if(abs(cur) > pbs->safe_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->safe_current_threshold);
				}
					break;
				
				default:
					break;
			}
		}
	else
	{
	switch (state) 
			{
				case POOL_HIGH:
				if(abs(cur) > pbs->ramp_high_current_threshold)
				{
					//将-20A~20A的电流值转换为-16384~16384
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->ramp_high_current_threshold);
				}
					break;
				
				case POOL_MID:
				if(abs(cur) > pbs->ramp_mid_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->ramp_mid_current_threshold);
				}
					break;
				
				case POOL_LOW:
				if(abs(cur) > pbs->ramp_low_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->ramp_low_current_threshold);
				}
					break;
				
				case POOL_EMPTY:
				if(abs(cur) > pbs->ramp_safe_current_threshold)
				{
					input = CurrentMapOut(pbs->current_mapp_coe,pbs->ramp_safe_current_threshold);
				}
					break;
				
				default:
					break;
			}
	
	}
	
	
	
	
	
	
  pbs->current_mapp_coe  = abs(pbs->current_mapp_coe);

	return input;
}
  /**
  * @Data    2019-03-24 23:23
  * @brief   获取缓存池状态
  * @param   void
  * @retval  void
  */
  uint8_t GetPowerPoolState(powerBufferPoolStruct* pbs,float angle)
  {
			if(Chassis_Angle < 10.0f)
			{
				if(pbs->r_w >= pbs->high_water_level)
				return POOL_HIGH;
				else if(pbs->r_w >= pbs->mid_water_level)
				return POOL_MID;
				else if(pbs->r_w >= pbs->low_water_level)
				return POOL_LOW;
				else return POOL_EMPTY;
						
			}
			else
			{
				if(pbs->r_w >= pbs->ramp_high_water_level)
				return POOL_HIGH;
				else if(pbs->r_w >= pbs->ramp_mid_water_level)
				return POOL_MID;
				else if(pbs->r_w >= pbs->ramp_low_water_level)
				return POOL_LOW;
				else return POOL_EMPTY;	
			}
//#define POOL_FULL  0x01
//#define POOL_PADDING 0x02
//#define POOL_EMPTY  0x03
//#define POOL_HIGH   0x04
//#define POOL_MID    0x05
//#define POOL_LOW    0x06
//#define POOL_PASS 0x07	
  }
  /**
  * @Data    2019-03-24 23:23
  * @brief   功率处理总函数
  * @param   void
  * @retval  void
  */
void SetInPut(powerBufferPoolStruct* pbs,int16_t *input,uint8_t len)
{
	//传入参数：功率池结构体指针  每个轮子PID计算出来的值   轮子数量
	//计算总电流量的形参
  int16_t suminput =0;   
  uint8_t i=0;     
  //系数	
  float coe[len];        
	//计算总电流
  for(i=0;i < len;i++)
  {
	  suminput +=abs(input[i]);
  }
	//计算每个电流占总电流的百分比
	for(i=0;i < len;i++)
  {
		coe[i] = ((float)(input[i]))/((float)(suminput));
	}
	//返回此时应输出的最大电流
  suminput = GetOutlet(pbs,suminput);
	//按照百分比分配最大电流
  for(i=0;i < len;i++)
  {
		input[i] = (int16_t)(suminput *coe[i]);
	}
}

/*-----------------------------------file of end------------------------------*/


