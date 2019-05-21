#ifndef __CLOUND_MOTOR_H
#define __CLOUND_MOTOR_H

#include "user_common.h"

#define DeilMoto_READID_START	0x201
#define DeilMoto_READID_END	0x203
#define CloundMoto_READID_START	0x205
#define CloundMoto_READID_END	0x207

#define DeilMoto_SENDID		0x200
#define CloundMoto_SENDID		0x1FF


#define SMALL_LOADANGLE		49152		/* 电机拨一个弹需要转的角度数  4.5*8191 （8孔拨弹）*/
#define LARGE_LOADANGLE	  147438			/* 电机拨一个弹需要转的角度数  4.5*8191 （8孔拨弹）*/
#define M2006_FIRSTANGLE		4119		/* 电机初始位置 */

extern M2006s_t M2006s[3];
extern M6020s_t M6020s[3];


//M2006与6623共用发送函数
void DeilMoto_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3);
void CloundMoto_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3);
void Clound_Motor_getInfo(CanRxMsg RxMessage);
void M6020_setTargetAngle(M6020Name_e motorName, int32_t angle);


#endif /* __M2006_MOTOR_H */



