#include "user_code.h"
extern uint16_t DMA_Counter;
extern uint8_t power_state;	
extern uint8_t Devi_Offline;
extern float Chassis_Angle;
extern uint8_t SuperCup_Empty_Flag;
extern int largecounter;
extern int largecounterflag ;


void Init(void){

/*************BSP������*************/
	LED_GPIO_Config();
	CAN1_QuickInit();
	CAN2_QuickInit();	
/***********Ӧ��ģ������*************/
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
	
/***********��λ��ר�ô���*************/
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
  //2150 ��
	//1500 ��
	    LASER(1);

//	//�������� С�������� 
//				ANO_Send_Data_V3(ext_power_heat_data.data.shooter_heat1,   Shooting.LargeBulletHeat,  ext_power_heat_data.data.shooter_heat0,  Shooting.SmallBulletHeat);
//	//С�������� ��Ƶ ����
//				ANO_Send_Data_V3(ext_shoot_data.data.bullet_speed,  ext_shoot_data.data.bullet_freq,  ext_power_heat_data.data.shooter_heat0,  Shooting.SmallBulletHeat);	
	//�������� ��Ƶ ����
//				ANO_Send_Data_V3(ext_shoot_data.data.bullet_speed,  ext_shoot_data.data.bullet_freq,  ext_power_heat_data.data.shooter_heat1,  Shooting.LargeBulletHeat);		
//С����ʵ��ֵ��Ŀ��ֵ  �󲦽�ʵ��ֵ��Ŀ��ֵ
//			ANO_Send_Data_V3(M2006s[1].targetAngle, M2006s[1].totalAngle,M3508s[4].targetAngle, M3508s[4].totalAngle);
//���� ���� ��ѹ ����
//	    ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power,ext_power_heat_data.data.chassis_current,ext_power_heat_data.data.chassis_volt,ext_power_heat_data.data.chassis_power_buffer);
//���� �������� ����б������ 
//      ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power,Power.P_value[1],Power.P_value[2],Power.P_value[3] );	
//��������
//   ANO_Send_Data_V3(Chassis.FollowYawAttitude_pid_RC.Target,Chassis.FollowYawAttitude_pid_RC.Measured\
//	 ,Chassis.FollowtargetYawLPF,M3508s[1].realSpeed);	

//     ANO_Send_Data_V3(M3508s[1].targetSpeed, M3508s[1].realSpeed,M3508s[0].targetSpeed, M3508s[0].realSpeed);
//���ʳز�������
//      ANO_Send_Data_V3(M3508s[1].realSpeed,Max_speed_counter,Power.allCurrent,ext_power_heat_data.data.chassis_current);     

//֡�ʼ��
//        ANO_Send_Data_V3(M3508s[0].M3508InfoUpdateFrame,M3508s[1].M3508InfoUpdateFrame,M3508s[2].M3508InfoUpdateFrame,M3508s[3].M3508InfoUpdateFrame);
//			  ANO_Send_Data_V3(M6020s[0].M6020InfoUpdateFrame,M6020s[1].M6020InfoUpdateFrame,GY_6050[0].ImuInfoUpdateFrame,dr16_data.DR16InfoUpdateFrame);

//�������ݵ���  ���Ƿ���ʹ�õ��ݣ� ���幦�ʣ�  ���ݵ�����  ����ģʽ��
//     ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power_buffer,ext_power_heat_data.data.chassis_current,SCCM_ReceiveData.cap_level,abs(M3508s[3].realSpeed));

//���̵���
//     ANO_Send_Data_V3(M3508s[4].targetSpeed,M3508s[4].realSpeed,M3508s[4].pid.err,M3508s[4].outCurrent);

//���̵��Թ���
 //     ANO_Send_Data_V3(M2006s[0].dial_Angle.err,M2006s[0].dial_Angle.pwm, M2006s[0].targetAngle/1000.0f, M2006s[0].totalAngle/1000.0f);
//ANO_Send_Data_V3( Shooting.realSmallSpeed, Shooting.realLargeSpeed,0,0);
//     ANO_Send_Data_V3(Cloud.IMUtargetYawLPF, GY_6050[0].totalYaw, abs(M3508s[1].realSpeed),abs(M3508s[2].realSpeed));

// ANO_Send_Data_V3(M6020s[0].targetAngle, M6020s[0].totalAngle,M6020s[0].inneroutCurrent, GY_6050[0].Gyro.x);

//ANO_Send_Data_V3(ext_power_heat_data.data.chassis_power_buffer,ext_power_heat_data.data.chassis_current,abs(M3508s[3].realSpeed),pid_state);
//ANO_Send_Data_V3(abs(M3508s[0].realSpeed),abs(M3508s[1].realSpeed),abs(M3508s[2].realSpeed),abs(M3508s[3].realSpeed));


   ANO_Send_Data_V3(largecounter,Shooting.realLargeSpeed,largecounterflag,largecounter);
}


