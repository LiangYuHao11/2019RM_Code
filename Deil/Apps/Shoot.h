#ifndef __SHOOT_H
#define __SHOOT_H

#include "user_common.h"

#define FRICMOTOR_SPEEDMAX		2000
#define FRICMOTOR_SPEEDMIN		1000

void SmallfricMotor_setSpeed2(uint16_t speed1,uint16_t speed2);
void SmallfricMotor_setSpeed(uint16_t speed1,uint16_t speed2);
void LargefricMotor_setSpeed(uint16_t speed1,uint16_t speed2);
void fric_Control(uint16_t snail_speed1,uint16_t snail_speed2,int mode);

void Shoot_Init(void);
void Smallshoot_processing(void);
void Largeshoot_processing(void);



extern int Small_Deil_Counter;
extern int Small_Deil_Flag;
extern int Large_Deil_Counter;
extern int Large_Deil_Flag;


#endif /* __SHOOT_H */










































