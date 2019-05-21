#ifndef __CLOUD_CONTROL_H
#define __CLOUD_CONTROL_H

#include "user_common.h"

#define Cloud_Mode_Normal		0
#define Cloud_Mode_WithIMU		1


#define Cloud_Yaw_Min			4206.0f
#define Cloud_Yaw_Max			5400.0f
#define Cloud_Yaw_Center	4800.0f

#define Cloud_Pitch_Max			2700.0f
#define Cloud_Pitch_Min			1800.0f
#define Cloud_Pitch_Center	2100.0f

#define M6020_AngleToDEG(ang)	ang/22.75f
#define Cloud_Yaw_delta			(Cloud_Yaw_Max - Cloud_Yaw_Min)
#define Cloud_Pitch_delta		(Cloud_Pitch_Max - Cloud_Pitch_Min)


void Cloud_setCurrent(int16_t iqyaw, int16_t iqpitch, int16_t iq7, int16_t iq8);
void Cloud_Init(void);
void Cloud_processing(float delta_yaw, float delta_pitch, uint8_t mode);
void Cloud_static(float temp_targetyaw, float delta_pitch,uint8_t mode);
void Cloud_processing_WithIMU(float delta_yaw, float delta_pitch, uint8_t mode);
void Cloud_setIMUPosForced(float posYaw, float posPitch);
float Cloud_getYawAngleWithRight(void);
float Cloud_getPitchAngleWithDown(void);
float Cloud_getYawAngleWithCenter(void);
void Cloud_setAbsPosForced(float posYaw, float posPitch);
int Yawmode_Judge(void);
int Pitchmode_Judge(int32_t temp_err);


extern Cloud_t Cloud;

#endif /* __CLOUD_CONTROL_H */


