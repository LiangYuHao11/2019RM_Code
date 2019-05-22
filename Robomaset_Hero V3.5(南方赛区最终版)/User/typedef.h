#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>

#pragma anon_unions
  



/* ������ */
typedef struct {
		float x;                 //������X�᷽������
		float y;                 //������Y�᷽������
		float z;                 //������Z�᷽������
}Vector_t;

typedef struct {
	float Roll;                 //ROLL�᷽��
	float Pitch;                //PITCH�᷽��
	float Yaw;                  //YAW�᷽��
}Eular_t;

typedef struct{
	Vector_t Gyro;               //����������          
	Eular_t  EularDate;          //ŷ��������     
	float lastYaw;               //��һ��YAW������    
	float totalYaw;
	int16_t  turnCount;
	
	uint8_t  ImuDevStatus;
	uint8_t  ImuInfoUpdateFlag;
	uint16_t ImuInfoUpdateFrame;
	uint8_t  ImuOffLineFlag;
}GY_IMU_t;





/* PID���� */
typedef struct{
	float Target; 			        //�趨Ŀ��ֵ
	float Measured; 				    //����ֵ
	float err; 						      //����ƫ��ֵ
	float err_last; 				    //��һ��ƫ��
	float err_beforeLast; 			//���ϴ�ƫ��
	float Kp, Ki, Kd;				    //Kp, Ki, Kd����ϵ��
	float pwm; 						      //pwm���
	uint32_t MaxOutput;				  //����޷�
  uint32_t IntegralLimit;			//�����޷� 
}incrementalpid_t;

typedef struct{
	float Target; 					    //�趨Ŀ��ֵ
	float last_Target; 					   //�趨Ŀ��ֵ
	float Measured; 				    //����ֵ
	float last_Measured; 				//�ϴβ���ֵ	
	float err; 						      //����ƫ��ֵ
	float err_last; 				    //��һ��ƫ��
	float integral_err; 			  //����ƫ��֮��
	float Kp_1, Ki_1, Kd_1;			//Kp_1, Ki_1, Kd_1����ϵ��
	float Kp_2, Ki_2, Kd_2;	    //Kp_2, Ki_2, Kd_2����ϵ��
	float pwm; 						      //pwm���
	uint32_t MaxOutput;				  //����޷�
  uint32_t IntegralLimit;			//�����޷� 
}positionpid_t;




/*��������*/
typedef union{
  uint8_t data[8];
  struct{
    float charge_power;    /* ��繦�ʣ���λ��W ,��Χ 0-80W */
    uint8_t charge_enable;  /* ���ʹ�� */
    uint8_t is_cap_output;  /* ʹ�õ��ݹ��� */
  };
} SCCM_SendData_t;


typedef union{
  uint8_t data[8];
   struct{
    float chassis_power;    /* ���̹��ʣ���λ��W */
    uint8_t chassis_buff;   /* ���̹��ʻ��� */
    uint8_t cap_usable;    /* ���ݿ��Խ������ */
	  int8_t cap_level;      /* ����ʣ�����������ָ��� */
  };
	 
} SCCM_ReceiveData_t;


typedef struct
{
	uint8_t  sccmControlMode;
	uint8_t	capInfoUpdateFlag;		 //��Ϣ��ȡ���±�־
	uint16_t	capInfoUpdateFrame;	 //֡��
	uint8_t	capOffLineFlag;		     //�豸���߱�־		 
}SCCM_Status_t;


typedef enum{
	
	OnlyUseSuperCap = 1,            //ֻ�ó������ݹ���
	BatteryChargeCap = 2,           //ʹ�õ�ع��磬���������
	BatteryDisChargeCap = 3,        //ʹ�õ�ع��磬�����������		
  GameBegin = 4,
	
}SCCMMode_e;

typedef struct
{
	uint8_t data_buff;
	float volt;
	float current;
	float power;
	float power_buffer;
}currentMeterStruct;

typedef struct
{
	currentMeterStruct* pcurrentMeter_t;	
	float max_p;//���Ĺ��ʵ�λ������
	float max_w;//���Ĺ��ʵ�λ������
	float r_w;//ʵʱ�Ĺ��ʵ�λ������
  float r_p;//ʵʱ�Ĺ��ʵ�λ������
	float current_mapp_coe;//����ӳ��ϵ��
	float period;//�������ڣ���λ/s
//��һ�ײ���
	float high_water_level;
	float low_water_level;
	float mid_water_level;
	float high_current_threshold;//A
  float mid_current_threshold;//A
  float low_current_threshold;//A
  float safe_current_threshold;//A
//�ڶ��ײ���
	float ramp_high_water_level;
	float ramp_low_water_level;
	float ramp_mid_water_level;
	float ramp_high_current_threshold;//A
  float ramp_mid_current_threshold;//A
  float ramp_low_current_threshold;//A
  float ramp_safe_current_threshold;//A
	
}powerBufferPoolStruct;

typedef enum{
	 Poll_Full = 1,
	 Poll_High = 2,
	 Poll_Mid = 3,
	 Poll_Low = 4,
	 Poll_Empty = 5,
}PoolStatus_e;


typedef enum{
	Chassis_static = 1,
	Chassis_start = 2,
	Chassis_run = 3,
	Chassis_change = 4,
	Chassis_end = 5,
}ChassisMode_e;


/* ���̵�� */
typedef struct{
	
	uint16_t realAngle;			        //�������Ļ�е�Ƕ�
	int16_t  realSpeed;			        //���������ٶ�
	int16_t  realCurrent;		        //��������ʵ�ʵ���
	uint8_t  temperture;            //�������ĵ���¶�
	
	int16_t  targetCurrent;			    //Ŀ�����
	int16_t  targetSpeed;			      //Ŀ���ٶ�
	int32_t  targetAngle;			      //Ŀ��Ƕ�
	int16_t  outCurrent;				    //�������
	
	uint16_t lastAngle;			//�ϴεĽǶ�
	int32_t  totalAngle;			//�ۻ��ܹ��Ƕ�
	int16_t  turnCount;			//ת����Ȧ��
	
	incrementalpid_t pid;		        //���pid
	
	positionpid_t Deil_Speed_pid;		        //���pid
	positionpid_t Dial_Angle_pid;		//����ٶ�pid
	
	uint8_t  M3508InfoUpdateFlag;		//��Ϣ��ȡ���±�־
	uint16_t M3508InfoUpdateFrame;	//֡��
	uint8_t  M3508OffLineFlag;		  //�豸���߱�־

}M3508s_t;



//���̵��
typedef struct{
	uint16_t realAngle;			//�������Ļ�е�Ƕ�
	int16_t realSpeed;			//���������ٶ�
	int16_t realTorque;			//��������ʵ��ת��

	int16_t targetSpeed;			//Ŀ���ٶ�
	int32_t targetAngle;			//Ŀ��Ƕ�
	
	uint16_t lastAngle;			//�ϴεĽǶ�
	int32_t  totalAngle;			//�ۻ��ܹ��Ƕ�
	int16_t  turnCount;			//ת����Ȧ��

	int16_t outCurrent;				//�������
	int16_t inneroutCurrent;				//�������
	positionpid_t dial_Speed;		//����ٶ�pid
	positionpid_t dial_Angle;		//����Ƕ�pid
	
	uint8_t  M2006InfoUpdateFlag;		//��Ϣ��ȡ���±�־
	uint16_t M2006InfoUpdateFrame;	//֡��
	uint8_t  M2006OffLineFlag;		  //�豸���߱�־
}M2006s_t;




/* ��̨��� */
typedef struct{
	
	uint16_t realAngle;			//�������Ļ�е�Ƕ�
	int16_t realSpeed;			//���������ٶ�
	int16_t realTorque;			//��������ʵ��ת��
	int16_t realTemperature;   //���������¶�
	
	int16_t targetCurrent;		      //Ŀ�����	
	int16_t targetAngle;		      //Ŀ��Ƕ�
	int32_t last_targetAngle;		
	int32_t targetSpeed;			    //Ŀ���ٶ�
	int16_t outCurrent;				    //�������
  int16_t inneroutCurrent;
	
	uint16_t lastAngle;		          //�ϴεĽǶ�
	int16_t  turnCount;			        //ת����Ȧ��
	int32_t totalAngle;			        //�ۻ��ܹ��Ƕ�
	
	positionpid_t Attitude_pid;			  //��̨yaw����̬pid
  positionpid_t Speed_pid;	
	
	uint8_t  M6020InfoUpdateFlag;		//��Ϣ��ȡ���±�־
	uint16_t M6020InfoUpdateFrame;	//֡��
	uint8_t  M6020OffLineFlag;		  //�豸���߱�־

}M6020s_t;

typedef enum{
  M6020_PITCH_Right = 0,
	M6020_PITCH_Left,
	M6020_YAW,	
}M6020Name_e;

typedef struct{
  
	int16_t official_LargeCounter;
	int16_t official_SmallCounter;
	int16_t LargeCounter;
	int8_t bulletStatus;
	int8_t lastBulletStatus;
	
	
}Bullet_t;


typedef struct{
  
	float chassisSpeedcoe;            //�����ٶȲ���������ʱΪ2��
	float cloundSpeedcoe_x;           //��̨X���ٶ�ϵ��
  float cloundSpeedcoe_y;           //��̨y���ٶ�ϵ��
	uint8_t normalControlCounter;	
	uint8_t suppiyControl;
	uint8_t supplyFlag;
	uint8_t fricSpeedFlag;
	uint8_t fricSpeedCounter;	
	uint8_t userVisionLight;
	uint8_t my_robot_id;
	uint8_t setUpSuperCap;
	
}PCMode_t;

/* �����˶� */
typedef struct{
	float LpfAttFactor;		//�����˲�ϵ��

	float targetXRaw;		//����x��ԭʼ����
	float targetYRaw;		//����y��ԭʼ����
	float targetZRaw;		//����z��ԭʼ����
	
	float targetXLPF;		//����x���˲�������
	float targetYLPF;		//����y���˲�������
	float targetZLPF;		//����z���˲�������
	
	float speedLimit;		//�����ٶ�����
	float speedUp;
	float FollowtargetYawRaw;	//����Ŀ��Yaw�������̨ԭʼ����
	float FollowtargetYawLPF;	//����Yaw�������̨�˲�������
		
	positionpid_t FollowYawAttitude_pid_RC;		//����Yaw�������̨pid
	positionpid_t FollowYawAttitude_pid_PC;		//����Yaw�������̨pid
  positionpid_t SwingAttitude_pid;		//����Yaw�������̨pid
	
	float swingMeasured;
	float swingTarget;
	float swingSpeed;
	float swingRange;
	float swingStep;
	
	
}Chassis_t;




/* ��̨�˶� */
typedef struct{
	float LpfAttFactor;			//��̨�˲�ϵ��

	float targetYawRaw;			//��̨Ŀ��yaw��ԭʼ����
	float last_targetYawRaw;
	float targetPitchRaw;		//��̨Ŀ��pitch��ԭʼ����
	float IMUtargetYawRaw;		//��̨Ŀ��yaw��ԭʼ����	
	float IMUtargetPitchRaw;	//��̨Ŀ��pitch��ԭʼ����
	
	float targetYawLPF;			//��̨yaw���˲�������
	float targetPitchLPF;		//��̨pitch���˲�������
	float IMUtargetYawLPF;		//��̨yaw���˲�������
  float last_IMUtargetYawLPF;		//��̨yaw���˲�������
	float IMUtargetPitchLPF;	//��̨pitch���˲�������
	
	float IMUYawAngleMax;		//��̨IMU���Ƕ�(��)
	float IMUYawAngleMin;		//��̨IMU��С�Ƕ�(��)
	float IMUPitchAngleMax;		//��̨IMU���Ƕ�(��)
	float IMUPitchAngleMin;		//��̨IMU��С�Ƕ�(��)
	
	positionpid_t YawAttitude_pid;			//��̨yaw����̬pid
	positionpid_t YawSpeed_pid;			//��̨yaw���ٶ�pid
	positionpid_t PitchAttitude_pid;			//��̨roll����̬pid
	positionpid_t PitchSpeed_pid;			//��̨roll����̬pid
	
	uint8_t Mode;						//��̨����ģʽ
}Cloud_t;


typedef enum{
	Cloud_Stable = 1,
	Cloud_Change= 2,
	Cloud_Interfere= 3,
	Cloud_Up = 4,
	Cloud_Down = 5,
}CloundMode_e;


/* ����˶� */
typedef struct{
	uint8_t shootFlagSmall;			  //��Ҫ��������С������
	uint8_t shootFlagLarge;       //��Ҫ��������������
	
	uint8_t shootReadySmall;			//С�������׼����־��
	uint8_t shootReadyLarge;			//�������׼����־��

	float coolingValueSmall;      //����ÿ����ȴֵ
	float coolingValueLarge;      //С����ÿ����ȴֵ
	float coolingValueBuff;      //С����ÿ����ȴֵ	
	float heatLimitSmall;         //С������������
	float heatLimitLarge;         //������������
  float LargeBulletHeat;        //���赱ǰ����
  float SmallBulletHeat;	      //С���赱ǰ����
	
	float realSmallSpeed;			
	float lastSmallSpeed;			
	float realLargeSpeed;			
	float lastLargeSpeed;
		
	uint8_t shootbuff;
	uint8_t largeCheckFlag;
	
	uint16_t fricLowSmasllSpeed;
	uint16_t fricHighSmasllSpeed;
	uint16_t fricLargeSpeed;
		
	uint16_t bulletNumber;	
	
	uint8_t heroLevel;			      //Ӣ�۵ȼ�
	uint8_t lastHeroLevel;

}shootGun_t;



typedef enum{
	One_Level = 1,
	Two_Level = 2,
	Three_Level = 3,
}RobotLever_e;

typedef enum{
	
	TurnOnLarge = 1,
	TurnOffLarge = 2,
	SetLarge = 3,
	TurnOnSmall = 4,
	TurnOffSmall = 5,
	SetSmall = 6,
	ResetSmall = 7,
  ResetLarge = 8,
	LowShootSpeed = 9,
	HighShootSpeed = 10,
	
 }ShootingMode;
 
 
 
 typedef enum{
  M3508Offline = 0,
	M6020Offline,
	M2006Offline,
  IMUOffline,
  Dr16Offline,	 
}OffLine;
 
 /* ң�� */
typedef struct{
  union{
    uint8_t data;
    struct{
      uint8_t KeyStatus:1;
      uint8_t lastKeyStatus:1;
      uint8_t keyPressedJump:1;
      uint8_t keyReleaseJump:1;
      uint8_t longPressed:1;
    };
  }flag;
	uint16_t timeCounter;
}key_t;

typedef struct{
  uint16_t KeyStatus;
  uint16_t lastKeyStatus;
  uint16_t keyPressedJump;
  uint16_t keyReleaseJump;
  uint16_t longPressed;
	uint16_t timeCounter[16];
}keyBoard_t;

typedef struct {
	struct{
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
		int16_t ch4;		
		uint8_t s_left;
		uint8_t s_right;
	}rc;
	
	struct{
		int16_t x;
		int16_t y;
		int16_t z;
	
		uint8_t keyLeft;
		uint8_t keyRight;
		
	}mouse;
	
	union {
		uint16_t key_code;
		struct{
			bool press_W:1;
			bool press_S:1;
			bool press_A:1;
			bool press_D:1;
			bool press_Shift:1;
			bool press_Ctrl:1;
			bool press_Q:1;
			bool press_E:1;
			
			bool press_R:1;
			bool press_F:1;
			bool press_G:1;
			bool press_Z:1;
			bool press_X:1;
			bool press_C:1;
			bool press_V:1;
			bool press_B:1;
		};
	}keyBoard;
	
	uint16_t DR16InfoUpdateFrame;	//֡��
	uint8_t DR16OffLineFlag;		//�豸���߱�־
}DR16_DBUS_DATA_t;


typedef enum{
	ControlMode_ByRC = 3,
	ControlMode_ByPC = 1,
	ControlMode_OFF = 2,
}ControlMode_e;

typedef enum{
	ControlMode_Mid = 3,
	ControlMode_High = 1,
	ControlMode_Low = 2,
}Mode_e;


typedef struct{
	uint8_t ControlLeft;
	uint8_t ControlRight;	
  uint8_t isCombineKey;
}ControlStatus_t;

typedef struct {
	float x;
	float y;
	float radian;
	float degrees;
	float distance;
}rocker_t;

typedef struct{
  int32_t count;
  int32_t scale;
  float   out;
}ramp_t;



/************************************************����ϵͳ�ṹ��*******************************************************/
typedef struct{
	uint16_t infoUpdateFrame;	//֡��
	uint8_t offLineFlag;		//�豸���߱�־
}Judge_Monitor_t;



//1.����������״̬��Ƶ�� 10Hz ���� CmdID 0x0001
typedef struct{
	union {
		uint8_t dataBuff[3];
		__packed struct{
			uint8_t game_type : 4;             //��������
			uint8_t game_progress : 4;         //��ǰ�����׶�
			uint16_t stage_remain_time;        //��ǰ�׶�ʣ��ʱ��
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_state_t;




//2.����������ݣ�0x0002������Ƶ�ʣ�������������
typedef struct{
	union {
		uint8_t dataBuff[1];
		__packed struct{
			uint8_t winner;          //0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_result_t;



//3.�����˴�����ݣ�0x0003������Ƶ�ʣ�1Hz
typedef struct{
	union {
		uint8_t dataBuff[2];
		__packed struct{
			uint16_t robot_legion;      //��Ӧ�� bit ��ֵ�� 1 ��������˴��
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_survivors_t;



//4.�����¼����ݣ�0x0101������Ƶ�ʣ��¼��ı����
typedef struct{
	union {
		uint8_t dataBuff[4];
		__packed struct{
			uint32_t event_type;             //���������ϵͳЭ��P8
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_event_data_t;




//5.����վ������ʶ��0x0102������Ƶ�ʣ������ı����
typedef struct{
	union {
		uint8_t dataBuff[4];
		__packed struct{
			uint8_t supply_projectile_id;         //����վ�� ID��
			uint8_t supply_robot_id;              //���������� ID
			uint8_t supply_projectile_step;       //�����ڿ���״̬
			uint8_t supply_projectile_num;        //��������
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_supply_projectile_action_t;




//6.���󲹸�վ�����ӵ���cmd_id (0x0103)������Ƶ�ʣ����� 10Hz��RM �Կ�����δ����
typedef struct{
	union {
		uint8_t dataBuff[3];
		__packed struct{
			uint8_t supply_projectile_id;       //����վ������ ID
			uint8_t supply_robot_id;            //���������� ID
			uint8_t supply_num;                 //������Ŀ
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_supply_projectile_booking_t;



//7.����������״̬��0x0201������Ƶ�ʣ�10Hztypedef struct{
typedef struct{
  union {
		uint8_t dataBuff[15];
		__packed struct{
			uint8_t robot_id;                           //������ ID
			uint8_t robot_level;                        //�����˵ȼ�         
			uint16_t remain_HP;                         //������ʣ��Ѫ��
			uint16_t max_HP;                            //����������Ѫ��
			uint16_t shooter_heat0_cooling_rate;        //������ 17mm ǹ��ÿ����ȴֵ
			uint16_t shooter_heat0_cooling_limit;       //������ 17mm ǹ����������
			uint16_t shooter_heat1_cooling_rate;        //������ 42mm ǹ��ÿ����ȴֵ
			uint16_t shooter_heat1_cooling_limit;       //������ 42mm ǹ����������
			uint8_t mains_power_gimbal_output : 1;      //���ص�Դ������
			uint8_t mains_power_chassis_output : 1;
			uint8_t mains_power_shooter_output : 1;
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_state_t;




//8.ʵʱ�����������ݣ�0x0202������Ƶ�ʣ�50Hz
typedef struct{
	union {
		uint8_t dataBuff[14];
		__packed struct{
			uint16_t chassis_volt;                   //���������ѹ ��λ ����
			uint16_t chassis_current;                //����������� ��λ ����
			float chassis_power;                     //����������� ��λ W ��
			int16_t chassis_power_buffer;           //���̹��ʻ��� ��λ J ����
			uint16_t shooter_heat0;                  //17mm ǹ������
			uint16_t shooter_heat1;                  //42mm ǹ������
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_power_heat_data_t;



//9. ������λ�ã�0x0203������Ƶ�ʣ�10Hz
typedef struct{
	union {
		uint8_t dataBuff[16];
		__packed struct{
			float x;                //λ�� x ���꣬��λ m
			float y;                //λ�� y ���꣬��λ m
			float z;                //λ�� z ���꣬��λ m
			float yaw;              //λ��ǹ�ڣ���λ��
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_pos_t;




//10.���������棺0x0204������Ƶ�ʣ�״̬�ı����
typedef struct{
	union {
		uint8_t dataBuff[1];
		__packed struct{
			uint8_t power_rune_buff;      //���������ϵͳЭ��P13
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_buff_musk_t;



//11. ���л���������״̬��0x0205������Ƶ�ʣ�10Hz
typedef struct{
	union {
		uint8_t dataBuff[2];
		__packed struct{
			uint8_t energy_point;      //���۵�������
      uint8_t attack_time;       //�ɹ���ʱ�� ��λ s��50s �ݼ��� 0
		};
	}data;
	uint8_t infoUpdateFlag;
} aerial_robot_energy_t;



//12. �˺�״̬��0x0206������Ƶ�ʣ��˺���������
typedef struct{
	union {
		uint8_t dataBuff[1];
		__packed struct{
			uint8_t armor_id : 4;            
			uint8_t hurt_type : 4;           
		};
	}data;
	uint8_t infoUpdateFlag;
} ext_robot_hurt_t;



//13. ʵʱ�����Ϣ��0x0207������Ƶ�ʣ��������
typedef struct{
	union {
		uint8_t dataBuff[6];
		__packed struct{
			uint8_t bullet_type;              //�ӵ�����
			uint8_t bullet_freq;              //�ӵ���Ƶ
			float bullet_speed;               //�ӵ�����
		};
	}data;
	uint8_t infoUpdateFlag;
} ext_shoot_data_t;


//�������Զ������ݣ�������ʾ�ڲ������棬��Ƶ 10Hz CmdID 0x0100
typedef struct{
	union {
		uint8_t dataBuff[13];
		__packed struct{
			float data1;
			float data2;
			float data3;
			uint8_t mask;
		};
	}data;
	uint8_t infoUpdateFlag;
	
}Judge_SendPC_t;

typedef struct{
	union {
		uint8_t dataBuff[13];
		struct{
			float data1;
		};
	}data;
	uint8_t infoUpdateFlag;
	
}Judge_SendRobot_t;




#endif /* __TYPEDEFS_H */
