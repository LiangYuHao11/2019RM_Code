/**
  ******************************************************************************
  * @file    Robot_status.c
  * @author Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   用于计算当前等级各种数据
  ******************************************************************************
  */
	
#include "Robot_status.h"

/**
  * @brief  当前英雄等级计算
  * @param[in]  英雄当前等级
  * @retval None
  */								 
void Lever_Determine(uint8_t lever){
	
	Shooting.heroLevel = lever;
	
	if(Shooting.lastHeroLevel == Shooting.heroLevel)
		return;

	
  if(One_Level == lever)
	{
		Shooting.coolingValueSmall = 4;
		Shooting.coolingValueLarge = 2;
    Shooting.heatLimitSmall = 240;
    Shooting.heatLimitLarge = 150;
	  Shooting.fricLowSmasllSpeed = 1125;     //  12m/s
	  Shooting.fricHighSmasllSpeed = 1250;    //  24m/s
	  Shooting.fricLargeSpeed = 1500;
	}
	
	if(Two_Level == lever)
	{
		Shooting.coolingValueSmall = 6;
		Shooting.coolingValueLarge = 4;
		Shooting.heatLimitSmall = 360;
		Shooting.heatLimitLarge = 250;
	  Shooting.fricLowSmasllSpeed = 1125;     //  12m/s
	  Shooting.fricHighSmasllSpeed = 1250;    //  24m/s
	  Shooting.fricLargeSpeed = 1500;		
		
	}
	
	if(Three_Level == lever)
	{
		Shooting.coolingValueSmall = 8;
		Shooting.coolingValueLarge = 6;
    Shooting.heatLimitSmall = 480;
    Shooting.heatLimitLarge = 400;
	  Shooting.fricLowSmasllSpeed = 1125;     //  12m/s
	  Shooting.fricHighSmasllSpeed = 1250;    //  24m/s
	  Shooting.fricLargeSpeed = 1500;
	}
	
  Shooting.lastHeroLevel = Shooting.heroLevel;

}





/**
  * @brief  热量计算
  * @param  None
  * @retval None
  */
/*
	一级英雄：发射间隔时间 2.5S 5S 5S 5S....（循环）
	二级英雄：发射间隔时间 连续打2颗 2S 2S 3S 2S 2S 3S....（循环）
	三级英雄：发射间隔时间 连续打4颗 2S 2S 1S 2S 2S 1S....（循环）
*/
void Heat_Calculation(void){
		
	float nowSmallHeat =0;
	float nowLargeHeat =0;

	if(Shooting.shootbuff)
	{
	  Shooting.coolingValueBuff = 5;
	}
	else
		 Shooting.coolingValueBuff = 1;

		
	//小弹丸热量
	if(Shooting.realSmallSpeed != Shooting.lastSmallSpeed)
	{
		Shooting.SmallBulletHeat = Shooting.SmallBulletHeat + Shooting.realSmallSpeed - Shooting.coolingValueSmall*Shooting.coolingValueBuff ;
		if(Shooting.SmallBulletHeat < 0) 
		{
			Shooting.SmallBulletHeat = 0;
    }	
  }
	else
	{
		Shooting.SmallBulletHeat = Shooting.SmallBulletHeat - Shooting.coolingValueSmall*Shooting.coolingValueBuff;
		if(Shooting.SmallBulletHeat < 0) 
		{
			Shooting.SmallBulletHeat = 0;
    }
	}
	
	
	if(Shooting.realLargeSpeed != Shooting.lastLargeSpeed)
	{
		Shooting.largeCheckFlag = 1;
		Shooting.LargeBulletHeat = Shooting.LargeBulletHeat + 100 - Shooting.coolingValueLarge*Shooting.coolingValueBuff;
		if(Shooting.LargeBulletHeat < 0) 
		{
			Shooting.LargeBulletHeat = 0;
    }	
  }
	else
	{
		Shooting.LargeBulletHeat = Shooting.LargeBulletHeat - Shooting.coolingValueLarge*Shooting.coolingValueBuff;
		if(Shooting.LargeBulletHeat < 0) 
		{
			Shooting.LargeBulletHeat = 0;
    }
	}
	
	 nowSmallHeat = max(Shooting.SmallBulletHeat,ext_power_heat_data.data.shooter_heat0);
	 nowLargeHeat = max(Shooting.LargeBulletHeat,ext_power_heat_data.data.shooter_heat1);
	 
	 Shooting.shootReadySmall =(nowSmallHeat < Shooting.heatLimitSmall-50)?1:0;
	 Shooting.shootReadyLarge =(nowLargeHeat < Shooting.heatLimitLarge-100)?1:0;

	 Shooting.lastSmallSpeed = Shooting.realSmallSpeed;
	 Shooting.lastLargeSpeed = Shooting.realLargeSpeed;
		
}






