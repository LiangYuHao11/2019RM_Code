#ifndef __RM2019_JudgeSystem_H
#define __RM2019_JudgeSystem_H

#include "user_common.h"


#define JUDGESYSTEM_PACKSIZE 		    180u		//裁判系统包大小
#define JUDGESYSTEM_FRAMEHEADER    	0xA5        //帧头


#define JudgeInfoType_Game_state                 0x0001
#define JudgeInfoType_Game_result	               0x0002
#define JudgeInfoType_Game_robot_survivors       0x0003
#define JudgeInfoType_Event_data                 0x0101
#define JudgeInfoType_Supply_projectile_action   0x0102
#define JudgeInfoType_Supply_projectile_booking  0x0103
#define JudgeInfoType_Game_robot_state           0x0201
#define JudgeInfoType_Power_heat_data            0x0202
#define JudgeInfoType_Game_robot_pos             0x0203
#define JudgeInfoType_Buff_musk                  0x0204
#define JudgeInfoType_Robot_energy               0x0205
#define JudgeInfoType_Robot_hurt                 0x0206
#define JudgeInfoType_Shoot_data                 0x0207


//整个数据帧的长度，FrameHeader(5-Byte)+CmdID(2-Byte)+Data(n-Byte)+FrameTail(2-Byte, CRC16, 整包校验)
#define JudgeInfoLength_Game_state                 12
#define JudgeInfoLength_Game_result	               10
#define JudgeInfoLength_Game_robot_survivors       11
#define JudgeInfoLength_Event_data                 13
#define JudgeInfoLength_Supply_projectile_action   12
#define JudgeInfoLength_Supply_projectile_booking  11
#define JudgeInfoLength_Game_robot_state           24
#define JudgeInfoLength_Power_heat_data            23
#define JudgeInfoLength_Game_robot_pos             25
#define JudgeInfoLength_Buff_musk                  10
#define JudgeInfoLength_Robot_energy               12
#define JudgeInfoLength_Robot_hurt                 10
#define JudgeInfoLength_Shoot_data                 15






extern  ext_game_state_t  ext_game_state;
extern ext_game_result_t  ext_game_result;
extern ext_game_robot_survivors_t  ext_game_robot_survivors;
extern ext_event_data_t  ext_event_data;
extern ext_supply_projectile_action_t  ext_supply_projectile_action;
extern ext_supply_projectile_booking_t  ext_supply_projectile_booking;
extern ext_game_robot_state_t  ext_game_robot_state;
extern ext_power_heat_data_t  ext_power_heat_data;
extern ext_game_robot_pos_t  ext_game_robot_pos;
extern ext_buff_musk_t  ext_buff_musk;
extern aerial_robot_energy_t  aerial_robot_energy;
extern ext_robot_hurt_t  ext_robot_hurt;
extern ext_shoot_data_t  ext_shoot_data;
extern Judge_Monitor_t Judge_Monitor;
extern Judge_SendPC_t Judge_SendData;
extern Judge_SendRobot_t Judge_SendRobot;



void JudgeSystem_Init(void);
void Judge_getInfo(uint16_t dataLength);
void Judge_sendPC(float Userdata1,float Userdata2,float Userdata3,uint8_t Userdata4,uint8_t robot_id);
void Judge_sendRobot(uint16_t cmd_id,uint8_t data);




#endif /* __MC01_JUDGESYSTEM_H */
