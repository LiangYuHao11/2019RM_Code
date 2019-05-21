#ifndef __M2006_MOTOR_H
#define __M2006_MOTOR_H

#include "user_common.h"

#define M2006_READID_START	0x201
#define M2006_READID_END	0x203
#define M2006_SENDID		0x200


extern M2006s_t M2006s[3];

//M2006与6623共用发送函数
void M2006_setCurrent(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);
void M2006_getInfo(CanRxMsg RxMessage);

#endif /* __M2006_MOTOR_H */
