// PID.h

#ifndef __PID_H
#define __PID_H


#include "user_common.h"


void PositionPID_paraReset(positionpid_t *pid_t, float kp_1, float ki_1, float kd_1, float kp_2, float ki_2, float kd_2,\
uint32_t MaxOutput, uint32_t IntegralLimit);
float Position_PID(positionpid_t *pid_t, float target, float measured);
float FollowSwing_Position_PID(positionpid_t *pid_t, float target, float measured);

void IncrementalPID_paraReset(incrementalpid_t *pid_t, float kp, float ki, float kd, uint32_t MaxOutput, uint32_t IntegralLimit);
float Incremental_PID(incrementalpid_t *pid_t, float target, float measured);

float Pitch_Position_PID(positionpid_t *pid_t, float target, float measured);
float Yaw_Position_PID(positionpid_t *pid_t, float target, float measured);
float Following_Yaw_Position_PID(positionpid_t *pid_t, float target, float measured);
float Dial_PositionPID(positionpid_t *pid_t, float target, float measured);

	
	#endif

