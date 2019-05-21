#ifndef  __SUPER_CAP_H
#define  __SUPER_CAP_H

#include "user_common.h"


#define SCCM_RX_ID 0x600
#define SCCM_TX_ID 0x601
#define USE 1
#define DISUSE 0



void SCCM_getInfo(CanRxMsg RxMessage);
void SCCM_Send(float sccm_power,uint8_t sccm_enable,uint8_t sccm_output);
void SCCM_Control(int8_t cap_remain, uint8_t cap_usaflag, uint8_t mode);
void SCCM_Init(void);

extern SCCM_ReceiveData_t SCCM_ReceiveData;
extern SCCM_SendData_t SCCM_SendData;
extern SCCM_Status_t SCCM_Status;

#endif



