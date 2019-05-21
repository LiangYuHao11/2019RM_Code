#include "user_code.h"

extern int Small_Shoottime;

void Init(void){

	LED_GPIO_Config();
  KEY_GPIO_Config();
	CAN1_QuickInit();
	DR16_receiverInit();   
  delay_ms(2000);
	USART6_QuickInit(9600);
	TIM6_CounterMode(89, 9999);
  Shoot_Init();
}


void Test(void){
	
//		ANO_Send_Data_V1(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);
//		ANO_Send_Data_V2(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);
//		ANO_Send_Data_V3(-M3508s[0].targetSpeed, -M3508s[0].realSpeed,M3508s[1].targetSpeed,M3508s[1].realSpeed);

   	ANO_Send_Data_V3( M2006s[0].targetAngle, M2006s[0].totalAngle,\
	  M2006s[0].dial_Speed.pwm, M2006s[0].dial_Angle.err);
	
}


