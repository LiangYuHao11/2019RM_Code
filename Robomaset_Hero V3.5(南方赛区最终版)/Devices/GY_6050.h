#ifndef __GY_6050_H
#define __GY_6050_H

#include "user_common.h"


#define GY6050_SENDID		    0x413
#define Conversion ratio

#define M3508_CurrentRatio	819.2f	//16384/20A = 819.2->1A

typedef union{
	struct{
		uint16_t yaw;
		int16_t gyro_z;
		int16_t pitch;
		int16_t gyro_x;
	};
	uint8_t dataBuff[8];
}bno055_data_u;



void GY6050_getCloundInfo(CanRxMsg RxMessage);
void GY6050_getChassisInfo(CanRxMsg RxMessage);
	


extern GY_IMU_t GY_6050[2];

#endif /* __M3508_MOTOR_H */



