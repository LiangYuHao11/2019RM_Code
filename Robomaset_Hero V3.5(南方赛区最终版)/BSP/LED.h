#ifndef __LED_H
#define __LED_H

#include "user_common.h"

void LED_GPIO_Config(void);

#define LED_GREEN_GPIO_CLK             	RCC_AHB1Periph_GPIOE
#define LED_GREEN_PIN                  	GPIO_Pin_7
#define LED_GREEN_GPIO_PORT           	GPIOE


#define LED_RED_GPIO_CLK             RCC_AHB1Periph_GPIOF
#define LED_RED_PIN                  GPIO_Pin_14
#define LED_RED_GPIO_PORT            GPIOF


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LEDRED_TOGGLE		 digitalToggle(GPIOE,GPIO_Pin_7)
#define LEDGREEN_TOGGLE		 digitalToggle(GPIOF,GPIO_Pin_14)
	

#define LED_RED(isOn) 			GPIO_WriteBit(LED_GREEN_GPIO_PORT,LED_GREEN_PIN, (BitAction)isOn)
#define LED_GREEN(isOn) 				GPIO_WriteBit(LED_RED_GPIO_PORT,LED_RED_PIN, (BitAction)isOn)

#endif /*__GPIO_H*/
