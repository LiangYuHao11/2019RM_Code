#ifndef _CHASSIS_POWER_H
#define _CHASSIS_POWER_H

#include "user_common.h"

#define Power_slopeMax 30
#define Buffer_Max 60

void Power_control(float power);
int8_t Chassis_Judge(float Vx, float Vy);
int16_t Constant_Power_Setup(int16_t outCurrent,int16_t all_outCurrent);


extern Power_t Power;


#endif
