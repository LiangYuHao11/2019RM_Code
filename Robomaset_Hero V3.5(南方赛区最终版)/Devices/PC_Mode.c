#include "PC_Mode.h"

PCMode_t PCMode;


	/**
  * @brief   按键参数初始化
  * @param 	 None
  * @retval  None
  */ 
void PC_Control_Init(void){
  
	PCMode.chassisSpeedcoe = 1;
	PCMode.cloundSpeedcoe_x = 0.025;
	PCMode.cloundSpeedcoe_y = 0.5f;
  PCMode.suppiyControl = 0;
	PCMode.normalControlCounter = 0;
	PCMode.userVisionLight = 0;
}

/*
	六盏灯
	1.是否为高速
	2.是否补给站模式
	3.射速
	
	
	三个数据
	1.弹药
	2.子弹弹量
*/

	/**
  * @brief   操作手界面返回指示灯函数
  * @param 	 None
  * @retval  None
  */ 
void PC_Control_Light(void)
{
	//是否为高速
  if(PCMode.chassisSpeedcoe == 2)
	{
		PCMode.userVisionLight |= 1<<0;
	}
	else
		PCMode.userVisionLight &= ~(1<<0);
	
	//是否补给站模式
  if(PCMode.suppiyControl )
	{
		PCMode.userVisionLight |= 1<<1;
	}
	else
		PCMode.userVisionLight &= ~(1<<1);
	
	//射速
  if(PCMode.fricSpeedFlag )
	{
		PCMode.userVisionLight |= 1<<2;
	}
	else
		PCMode.userVisionLight &= ~(1<<2);
	
}



