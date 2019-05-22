#include "PC_Mode.h"

PCMode_t PCMode;


	/**
  * @brief   ����������ʼ��
  * @param 	 None
  * @retval  None
  */ 
void PC_Control_Init(void){
  
	PCMode.chassisSpeedcoe = 1;
	PCMode.cloundSpeedcoe_x = 0.025;
	PCMode.cloundSpeedcoe_y = 0.5f;
  PCMode.suppiyControl = 0;
	PCMode.normalControlCounter = 0;
	PCMode.userVisionLight = 0;
}

/*
	��յ��
	1.�Ƿ�Ϊ����
	2.�Ƿ񲹸�վģʽ
	3.����
	
	
	��������
	1.��ҩ
	2.�ӵ�����
*/

	/**
  * @brief   �����ֽ��淵��ָʾ�ƺ���
  * @param 	 None
  * @retval  None
  */ 
void PC_Control_Light(void)
{
	//�Ƿ�Ϊ����
  if(PCMode.chassisSpeedcoe == 2)
	{
		PCMode.userVisionLight |= 1<<0;
	}
	else
		PCMode.userVisionLight &= ~(1<<0);
	
	//�Ƿ񲹸�վģʽ
  if(PCMode.suppiyControl )
	{
		PCMode.userVisionLight |= 1<<1;
	}
	else
		PCMode.userVisionLight &= ~(1<<1);
	
	//����
  if(PCMode.fricSpeedFlag )
	{
		PCMode.userVisionLight |= 1<<2;
	}
	else
		PCMode.userVisionLight &= ~(1<<2);
	
}



