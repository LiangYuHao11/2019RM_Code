#ifndef __STEERING_H
#define __STEERING_H


#include "user_common.h"



#define TIM4_CH3_GPIO_CLK          RCC_AHB1Periph_GPIOD
#define TIM4_CH3_GPIO_PORT         GPIOD
#define TIM4_CH3_Pin          	 	 GPIO_Pin_14
#define TIM4_CH3_PINSOURCE			   GPIO_PinSource14
#define TIM4_CH3_ENABLE			       0

void Steering_Init(void);


#endif /* __FILTER_H */



