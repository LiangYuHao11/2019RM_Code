#include "user_code.h"
extern uint16_t DMA_Counter;
extern uint8_t power_state;	
extern uint8_t Devi_Offline;
extern float Chassis_Angle;
extern uint8_t SuperCup_Empty_Flag;
extern int largecounter;
extern int largecounterflag ;


void Init(void){

/*************BSP层配置*************/
	LED_GPIO_Config();
	CAN1_QuickInit();
	CAN2_QuickInit();	
/***********应用模块配置*************/
	DR16_receiverInit();  
  JudgeSystem_Init();	
	PowerBufferPoolInit();
  delay_ms(2000);
  Shoot_Init();
  Laser_Init();
  Chassis_Init();
	Cloud_Init();
	PC_Control_Init();
	Lever_Determine(1);
	SCCM_Init();
  Steering_Init();
  Bullet_Calculate_GPIOInit();
	
	delay_ms(2000);
	
/***********上位机专用串口*************/
	USART2_QuickInit(9600);
	TIM5_CounterMode(89, 9999);

	delay_ms(6000);	
	
	TIM6_CounterMode(89, 9999);
	LED_GREEN(1);
  LED_RED(1); 	

}

int PLPL = 1000;

void Test(void){
//		LED1_TOGGLE;
		  delay_ms(5);
//		TIM_SetCompare3(TIM4, PLPL);
  //2150 关
	//1500 开
	    LASER(1);

//	//大弹丸热量 小弹丸热量 
//				ANO_Send_Data_V3(ext_power_heat_data.data.shooter_heat1,   Shooting.LargeBulletHeat,  ext_power_heat_data.data.shooter_heat0,  Shooting.SmallBulletHeat);
//	//小弹丸热量 射频 射速
//				ANO_Send_Data_V3(ext_shoot_data.data.bullet_speed,  ext_shoot_data.data.bullet_freq,  ext_power_heat_data.data.shooter_heat0,  Shooting.SmallBulletHeat);	
	//大弹丸热量 射频 射速
//				ANO_Send_Data_V3(ext_shoot_data.data.bullet_speed,  ext_shoot_data.data.bullet_freq,  ext_power_heat_data.data.shooter_heat1,  Shooting.LargeBulletHeat);		
//小拨角实际值和目标值  大拨角实际值和目标值
//			ANO_Send_Data_V3(M2006s[1].targetAngle, M2006s[1].totalAngle,M3508s[4].targetAngle, M3508s[4].totalAngle);
//功率 电流 电压 缓冲
//	    ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power,ext_power_heat_data.data.chassis_current,ext_power_heat_data.data.chassis_volt,ext_power_heat_data.data.chassis_power_buffer);
//功率 功率数组 功率斜率数组 
//      ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power,Power.P_value[1],Power.P_value[2],Power.P_value[3] );	
//调跟随用
//   ANO_Send_Data_V3(Chassis.FollowYawAttitude_pid_RC.Target,Chassis.FollowYawAttitude_pid_RC.Measured\
//	 ,Chassis.FollowtargetYawLPF,M3508s[1].realSpeed);	

//     ANO_Send_Data_V3(M3508s[1].targetSpeed, M3508s[1].realSpeed,M3508s[0].targetSpeed, M3508s[0].realSpeed);
//功率池参数整定
//      ANO_Send_Data_V3(M3508s[1].realSpeed,Max_speed_counter,Power.allCurrent,ext_power_heat_data.data.chassis_current);     

//帧率检测
//        ANO_Send_Data_V3(M3508s[0].M3508InfoUpdateFrame,M3508s[1].M3508InfoUpdateFrame,M3508s[2].M3508InfoUpdateFrame,M3508s[3].M3508InfoUpdateFrame);
//			  ANO_Send_Data_V3(M6020s[0].M6020InfoUpdateFrame,M6020s[1].M6020InfoUpdateFrame,GY_6050[0].ImuInfoUpdateFrame,dr16_data.DR16InfoUpdateFrame);

//超级电容调试  （是否在使用电容， 缓冲功率，  电容电量，  电容模式）
//     ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power_buffer,ext_power_heat_data.data.chassis_current,SCCM_ReceiveData.cap_level,abs(M3508s[3].realSpeed));

//大拨盘调试
//     ANO_Send_Data_V3(M3508s[4].targetSpeed,M3508s[4].realSpeed,M3508s[4].pid.err,M3508s[4].outCurrent);

//大拨盘调试供弹
 //     ANO_Send_Data_V3(M2006s[0].dial_Angle.err,M2006s[0].dial_Angle.pwm, M2006s[0].targetAngle/1000.0f, M2006s[0].totalAngle/1000.0f);
//ANO_Send_Data_V3( Shooting.realSmallSpeed, Shooting.realLargeSpeed,0,0);
//     ANO_Send_Data_V3(Cloud.IMUtargetYawLPF, GY_6050[0].totalYaw, abs(M3508s[1].realSpeed),abs(M3508s[2].realSpeed));

// ANO_Send_Data_V3(M6020s[0].targetAngle, M6020s[0].totalAngle,M6020s[0].inneroutCurrent, GY_6050[0].Gyro.x);

//ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power_buffer,ext_power_heat_data.data.chassis_current,abs(M3508s[3].realSpeed),pid_state);
//ANO_Send_Data_V3(abs(M3508s[0].realSpeed),abs(M3508s[1].realSpeed),abs(M3508s[2].realSpeed),abs(M3508s[3].realSpeed));


   ANO_Send_Data_V3(largecounter,Shooting.realLargeSpeed,largecounterflag,largecounter);
}


