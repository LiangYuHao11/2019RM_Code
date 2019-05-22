#ifndef __SHOOT_H
#define __SHOOT_H

#include "user_common.h"

#define FRICMOTOR_SPEEDMAX		2000
#define FRICMOTOR_SPEEDMIN		1000

//#define SHOOT_WAITSLOW		74
//#define SHOOT_WAITMIN		32
//#define SHOOT_WAITFAST		5

#define M3508TargetAngle      31214

extern shootGun_t Gun;

extern const uint16_t FricSpeed[3];

void fric_Control(uint16_t snail_speed1,uint16_t snail_speed2,int mode);

void Shoot_Init(void);
void Smallshoot_processing(void);
void Large_processing(void);
void SmallfricMotor_setSpeed2(uint16_t speed1,uint16_t speed2);
void SmallfricMotor_setSpeed(uint16_t speed1,uint16_t speed2);
void LargefricMotor_setSpeed(uint16_t speed1,uint16_t speed2);
void	Bullet_Shooting(void);


extern shootGun_t Shooting;



#endif /* __SHOOT_H */










































