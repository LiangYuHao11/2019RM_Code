#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>

#pragma anon_unions
  



/* 陀螺仪 */
typedef struct {
		float x;                 //浮点数X轴方向向量
		float y;                 //浮点数Y轴方向向量
		float z;                 //浮点数Z轴方向向量
}Vector_t;

typedef struct {
	float Roll;                 //ROLL轴方向
	float Pitch;                //PITCH轴方向
	float Yaw;                  //YAW轴方向
}Eular_t;

typedef struct{
	Vector_t Gyro;               //陀螺仪数据          
	Eular_t  EularDate;          //欧拉角数据     
	float lastYaw;               //上一次YAW轴数据    
	float totalYaw;
	int16_t  turnCount;
	
	uint8_t  ImuDevStatus;
	uint8_t  ImuInfoUpdateFlag;
	uint16_t ImuInfoUpdateFrame;
	uint8_t  ImuOffLineFlag;
}GY_IMU_t;





/* PID参数 */
typedef struct{
	float Target; 			        //设定目标值
	float Measured; 				    //测量值
	float err; 						      //本次偏差值
	float err_last; 				    //上一次偏差
	float err_beforeLast; 			//上上次偏差
	float Kp, Ki, Kd;				    //Kp, Ki, Kd控制系数
	float pwm; 						      //pwm输出
	uint32_t MaxOutput;				  //输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}incrementalpid_t;

typedef struct{
	float Target; 					    //设定目标值
	float last_Target; 					   //设定目标值
	float Measured; 				    //测量值
	float last_Measured; 				//上次测量值	
	float err; 						      //本次偏差值
	float err_last; 				    //上一次偏差
	float integral_err; 			  //所有偏差之和
	float Kp_1, Ki_1, Kd_1;			//Kp_1, Ki_1, Kd_1控制系数
	float Kp_2, Ki_2, Kd_2;	    //Kp_2, Ki_2, Kd_2控制系数
	float pwm; 						      //pwm输出
	uint32_t MaxOutput;				  //输出限幅
  uint32_t IntegralLimit;			//积分限幅 
}positionpid_t;




/*超级电容*/
typedef union{
  uint8_t data[8];
  struct{
    float charge_power;    /* 充电功率，单位：W ,范围 0-80W */
    uint8_t charge_enable;  /* 充电使能 */
    uint8_t is_cap_output;  /* 使用电容供电 */
  };
} SCCM_SendData_t;


typedef union{
  uint8_t data[8];
   struct{
    float chassis_power;    /* 底盘功率，单位：W */
    uint8_t chassis_buff;   /* 底盘功率缓冲 */
    uint8_t cap_usable;    /* 电容可以进行输出 */
	  int8_t cap_level;      /* 电容剩余电量，会出现负数 */
  };
	 
} SCCM_ReceiveData_t;


typedef struct
{
	uint8_t  sccmControlMode;
	uint8_t	capInfoUpdateFlag;		 //信息读取更新标志
	uint16_t	capInfoUpdateFrame;	 //帧率
	uint8_t	capOffLineFlag;		     //设备离线标志		 
}SCCM_Status_t;


typedef enum{
	
	OnlyUseSuperCap = 1,            //只用超级电容供电
	BatteryChargeCap = 2,           //使用电池供电，并给他充电
	BatteryDisChargeCap = 3,        //使用电池供电，不给给他充电		
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
	float max_p;//最大的功率单位：瓦特
	float max_w;//最大的功率单位：焦耳
	float r_w;//实时的功率单位：焦耳
  float r_p;//实时的功率单位：瓦特
	float current_mapp_coe;//电流映射系数
	float period;//运行周期，单位/s
//第一套参数
	float high_water_level;
	float low_water_level;
	float mid_water_level;
	float high_current_threshold;//A
  float mid_current_threshold;//A
  float low_current_threshold;//A
  float safe_current_threshold;//A
//第二套参数
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


/* 底盘电机 */
typedef struct{
	
	uint16_t realAngle;			        //读回来的机械角度
	int16_t  realSpeed;			        //读回来的速度
	int16_t  realCurrent;		        //读回来的实际电流
	uint8_t  temperture;            //读回来的电机温度
	
	int16_t  targetCurrent;			    //目标电流
	int16_t  targetSpeed;			      //目标速度
	int32_t  targetAngle;			      //目标角度
	int16_t  outCurrent;				    //输出电流
	
	uint16_t lastAngle;			//上次的角度
	int32_t  totalAngle;			//累积总共角度
	int16_t  turnCount;			//转过的圈数
	
	incrementalpid_t pid;		        //电机pid
	
	positionpid_t Deil_Speed_pid;		        //电机pid
	positionpid_t Dial_Angle_pid;		//电机速度pid
	
	uint8_t  M3508InfoUpdateFlag;		//信息读取更新标志
	uint16_t M3508InfoUpdateFrame;	//帧率
	uint8_t  M3508OffLineFlag;		  //设备离线标志

}M3508s_t;



//拨盘电机
typedef struct{
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realTorque;			//读回来的实际转矩

	int16_t targetSpeed;			//目标速度
	int32_t targetAngle;			//目标角度
	
	uint16_t lastAngle;			//上次的角度
	int32_t  totalAngle;			//累积总共角度
	int16_t  turnCount;			//转过的圈数

	int16_t outCurrent;				//输出电流
	int16_t inneroutCurrent;				//输出电流
	positionpid_t dial_Speed;		//电机速度pid
	positionpid_t dial_Angle;		//电机角度pid
	
	uint8_t  M2006InfoUpdateFlag;		//信息读取更新标志
	uint16_t M2006InfoUpdateFrame;	//帧率
	uint8_t  M2006OffLineFlag;		  //设备离线标志
}M2006s_t;




/* 云台电机 */
typedef struct{
	
	uint16_t realAngle;			//读回来的机械角度
	int16_t realSpeed;			//读回来的速度
	int16_t realTorque;			//读回来的实际转矩
	int16_t realTemperature;   //读回来的温度
	
	int16_t targetCurrent;		      //目标电流	
	int16_t targetAngle;		      //目标角度
	int32_t last_targetAngle;		
	int32_t targetSpeed;			    //目标速度
	int16_t outCurrent;				    //输出电流
  int16_t inneroutCurrent;
	
	uint16_t lastAngle;		          //上次的角度
	int16_t  turnCount;			        //转过的圈数
	int32_t totalAngle;			        //累积总共角度
	
	positionpid_t Attitude_pid;			  //云台yaw轴姿态pid
  positionpid_t Speed_pid;	
	
	uint8_t  M6020InfoUpdateFlag;		//信息读取更新标志
	uint16_t M6020InfoUpdateFrame;	//帧率
	uint8_t  M6020OffLineFlag;		  //设备离线标志

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
  
	float chassisSpeedcoe;            //底盘速度参数，按下时为2倍
	float cloundSpeedcoe_x;           //云台X轴速度系数
  float cloundSpeedcoe_y;           //云台y轴速度系数
	uint8_t normalControlCounter;	
	uint8_t suppiyControl;
	uint8_t supplyFlag;
	uint8_t fricSpeedFlag;
	uint8_t fricSpeedCounter;	
	uint8_t userVisionLight;
	uint8_t my_robot_id;
	uint8_t setUpSuperCap;
	
}PCMode_t;

/* 底盘运动 */
typedef struct{
	float LpfAttFactor;		//底盘滤波系数

	float targetXRaw;		//底盘x轴原始数据
	float targetYRaw;		//底盘y轴原始数据
	float targetZRaw;		//底盘z轴原始数据
	
	float targetXLPF;		//底盘x轴滤波后数据
	float targetYLPF;		//底盘y轴滤波后数据
	float targetZLPF;		//底盘z轴滤波后数据
	
	float speedLimit;		//底盘速度限制
	float speedUp;
	float FollowtargetYawRaw;	//底盘目标Yaw轴跟随云台原始数据
	float FollowtargetYawLPF;	//底盘Yaw轴跟随云台滤波后数据
		
	positionpid_t FollowYawAttitude_pid_RC;		//底盘Yaw轴跟随云台pid
	positionpid_t FollowYawAttitude_pid_PC;		//底盘Yaw轴跟随云台pid
  positionpid_t SwingAttitude_pid;		//底盘Yaw轴跟随云台pid
	
	float swingMeasured;
	float swingTarget;
	float swingSpeed;
	float swingRange;
	float swingStep;
	
	
}Chassis_t;




/* 云台运动 */
typedef struct{
	float LpfAttFactor;			//云台滤波系数

	float targetYawRaw;			//云台目标yaw轴原始数据
	float last_targetYawRaw;
	float targetPitchRaw;		//云台目标pitch轴原始数据
	float IMUtargetYawRaw;		//云台目标yaw轴原始数据	
	float IMUtargetPitchRaw;	//云台目标pitch轴原始数据
	
	float targetYawLPF;			//云台yaw轴滤波后数据
	float targetPitchLPF;		//云台pitch轴滤波后数据
	float IMUtargetYawLPF;		//云台yaw轴滤波后数据
  float last_IMUtargetYawLPF;		//云台yaw轴滤波后数据
	float IMUtargetPitchLPF;	//云台pitch轴滤波后数据
	
	float IMUYawAngleMax;		//云台IMU最大角度(右)
	float IMUYawAngleMin;		//云台IMU最小角度(左)
	float IMUPitchAngleMax;		//云台IMU最大角度(下)
	float IMUPitchAngleMin;		//云台IMU最小角度(上)
	
	positionpid_t YawAttitude_pid;			//云台yaw轴姿态pid
	positionpid_t YawSpeed_pid;			//云台yaw轴速度pid
	positionpid_t PitchAttitude_pid;			//云台roll轴姿态pid
	positionpid_t PitchSpeed_pid;			//云台roll轴姿态pid
	
	uint8_t Mode;						//云台控制模式
}Cloud_t;


typedef enum{
	Cloud_Stable = 1,
	Cloud_Change= 2,
	Cloud_Interfere= 3,
	Cloud_Up = 4,
	Cloud_Down = 5,
}CloundMode_e;


/* 射击运动 */
typedef struct{
	uint8_t shootFlagSmall;			  //我要点鼠标射击小弹丸啦
	uint8_t shootFlagLarge;       //我要点鼠标射击大弹丸啦
	
	uint8_t shootReadySmall;			//小弹丸射击准备标志啦
	uint8_t shootReadyLarge;			//大弹丸射击准备标志啦

	float coolingValueSmall;      //大弹丸每秒冷却值
	float coolingValueLarge;      //小弹丸每秒冷却值
	float coolingValueBuff;      //小弹丸每秒冷却值	
	float heatLimitSmall;         //小弹丸热量上限
	float heatLimitLarge;         //大弹丸热量上限
  float LargeBulletHeat;        //大弹丸当前热量
  float SmallBulletHeat;	      //小弹丸当前热量
	
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
	
	uint8_t heroLevel;			      //英雄等级
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
 
 /* 遥控 */
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
	
	uint16_t DR16InfoUpdateFrame;	//帧率
	uint8_t DR16OffLineFlag;		//设备离线标志
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



/************************************************裁判系统结构体*******************************************************/
typedef struct{
	uint16_t infoUpdateFrame;	//帧率
	uint8_t offLineFlag;		//设备离线标志
}Judge_Monitor_t;



//1.比赛机器人状态，频率 10Hz 推送 CmdID 0x0001
typedef struct{
	union {
		uint8_t dataBuff[3];
		__packed struct{
			uint8_t game_type : 4;             //比赛类型
			uint8_t game_progress : 4;         //当前比赛阶段
			uint16_t stage_remain_time;        //当前阶段剩余时间
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_state_t;




//2.比赛结果数据：0x0002。发送频率：比赛结束后发送
typedef struct{
	union {
		uint8_t dataBuff[1];
		__packed struct{
			uint8_t winner;          //0 平局 1 红方胜利 2 蓝方胜利
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_result_t;



//3.机器人存活数据：0x0003。发送频率：1Hz
typedef struct{
	union {
		uint8_t dataBuff[2];
		__packed struct{
			uint16_t robot_legion;      //对应的 bit 数值置 1 代表机器人存活
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_survivors_t;



//4.场地事件数据：0x0101。发送频率：事件改变后发送
typedef struct{
	union {
		uint8_t dataBuff[4];
		__packed struct{
			uint32_t event_type;             //详情见裁判系统协议P8
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_event_data_t;




//5.补给站动作标识：0x0102。发送频率：动作改变后发送
typedef struct{
	union {
		uint8_t dataBuff[4];
		__packed struct{
			uint8_t supply_projectile_id;         //补给站口 ID：
			uint8_t supply_robot_id;              //补弹机器人 ID
			uint8_t supply_projectile_step;       //出弹口开闭状态
			uint8_t supply_projectile_num;        //补弹数量
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_supply_projectile_action_t;




//6.请求补给站补弹子弹：cmd_id (0x0103)。发送频率：上限 10Hz。RM 对抗赛尚未开放
typedef struct{
	union {
		uint8_t dataBuff[3];
		__packed struct{
			uint8_t supply_projectile_id;       //补给站补弹口 ID
			uint8_t supply_robot_id;            //补弹机器人 ID
			uint8_t supply_num;                 //补弹数目
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_supply_projectile_booking_t;



//7.比赛机器人状态：0x0201。发送频率：10Hztypedef struct{
typedef struct{
  union {
		uint8_t dataBuff[15];
		__packed struct{
			uint8_t robot_id;                           //机器人 ID
			uint8_t robot_level;                        //机器人等级         
			uint16_t remain_HP;                         //机器人剩余血量
			uint16_t max_HP;                            //机器人上限血量
			uint16_t shooter_heat0_cooling_rate;        //机器人 17mm 枪口每秒冷却值
			uint16_t shooter_heat0_cooling_limit;       //机器人 17mm 枪口热量上限
			uint16_t shooter_heat1_cooling_rate;        //机器人 42mm 枪口每秒冷却值
			uint16_t shooter_heat1_cooling_limit;       //机器人 42mm 枪口热量上限
			uint8_t mains_power_gimbal_output : 1;      //主控电源输出情况
			uint8_t mains_power_chassis_output : 1;
			uint8_t mains_power_shooter_output : 1;
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_state_t;




//8.实时功率热量数据：0x0202。发送频率：50Hz
typedef struct{
	union {
		uint8_t dataBuff[14];
		__packed struct{
			uint16_t chassis_volt;                   //底盘输出电压 单位 毫伏
			uint16_t chassis_current;                //底盘输出电流 单位 毫安
			float chassis_power;                     //底盘输出功率 单位 W 瓦
			int16_t chassis_power_buffer;           //底盘功率缓冲 单位 J 焦耳
			uint16_t shooter_heat0;                  //17mm 枪口热量
			uint16_t shooter_heat1;                  //42mm 枪口热量
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_power_heat_data_t;



//9. 机器人位置：0x0203。发送频率：10Hz
typedef struct{
	union {
		uint8_t dataBuff[16];
		__packed struct{
			float x;                //位置 x 坐标，单位 m
			float y;                //位置 y 坐标，单位 m
			float z;                //位置 z 坐标，单位 m
			float yaw;              //位置枪口，单位度
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_game_robot_pos_t;




//10.机器人增益：0x0204。发送频率：状态改变后发送
typedef struct{
	union {
		uint8_t dataBuff[1];
		__packed struct{
			uint8_t power_rune_buff;      //详情见裁判系统协议P13
		};
	}data;
	uint8_t infoUpdateFlag;
}ext_buff_musk_t;



//11. 空中机器人能量状态：0x0205。发送频率：10Hz
typedef struct{
	union {
		uint8_t dataBuff[2];
		__packed struct{
			uint8_t energy_point;      //积累的能量点
      uint8_t attack_time;       //可攻击时间 单位 s。50s 递减至 0
		};
	}data;
	uint8_t infoUpdateFlag;
} aerial_robot_energy_t;



//12. 伤害状态：0x0206。发送频率：伤害发生后发送
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



//13. 实时射击信息：0x0207。发送频率：射击后发送
typedef struct{
	union {
		uint8_t dataBuff[6];
		__packed struct{
			uint8_t bullet_type;              //子弹类型
			uint8_t bullet_freq;              //子弹射频
			float bullet_speed;               //子弹射速
		};
	}data;
	uint8_t infoUpdateFlag;
} ext_shoot_data_t;


//参赛队自定义数据，用于显示在操作界面，限频 10Hz CmdID 0x0100
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
