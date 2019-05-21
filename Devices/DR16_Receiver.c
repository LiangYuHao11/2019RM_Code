/**
  ******************************************************************************
  * @file    DR16_Receiver.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   DR16���ջ�Ӧ�ú����ӿ�
  ******************************************************************************
  */
  
  
#include <Math.h>
#include "DR16_Receiver.h"

uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE+2]; 	//����buff
DR16_DBUS_DATA_t dr16_data;

ControlStatus_t ControlStatus;


keyBoard_t dr16_keyBorad;
key_t dr16_mouseKeyLeft;
key_t dr16_mouseKeyRight;

rocker_t dr16_rocker_L,dr16_rocker_R;
rocker_t pcRocker;
#define DR16_KEY_LONGPRESS_TIMEOUT 50




	
/**
  * @brief  DR16��갴������
  * @param  newKeyStatus �°���״̬������Ϊ1��δ����Ϊ0
  *         *key �����ṹ��ָ�� @ref key_t
  * @retval None
  */
void DR16_mouseProcess(uint8_t newKeyStatus, key_t *key){
	key->flag.keyPressedJump = 0;
	key->flag.keyReleaseJump = 0;
	
	key->flag.KeyStatus = newKeyStatus & 0x01;
	
	if (key->flag.KeyStatus != key->flag.lastKeyStatus) {
		if(key->flag.KeyStatus)
		{
			key->flag.keyPressedJump = key->flag.KeyStatus;
			key->flag.keyPressedJump = key->flag.KeyStatus;
		}
		else{
			key->flag.keyReleaseJump = key->flag.lastKeyStatus;
			key->flag.longPressed = 0;
			key->timeCounter = 0;
		}
	}
	
	if(key->flag.KeyStatus){
		key->timeCounter++;
		if(key->timeCounter > DR16_KEY_LONGPRESS_TIMEOUT){
			key->flag.longPressed = key->flag.KeyStatus;
			key->timeCounter = 0;
		}
	}
	
	key->flag.lastKeyStatus = key->flag.KeyStatus;
  
}


/**
  * @brief  DR16���̰�������
  * @param  newKeyStatus �°���״̬�����¶�ӦλΪ1��δ����Ϊ0
  *         *keyBoard �����ṹ��ָ�� @ref key_t
  * @retval None
  */
void DR16_keyBoardProcess(uint16_t newKeyStatus, keyBoard_t *keyBoard){
	keyBoard->keyPressedJump = 0;
	keyBoard->keyReleaseJump = 0;
	
	keyBoard->KeyStatus = newKeyStatus;
	/*
	��keyBoard->KeyStatus ^ keyBoard->lastKeyStatus������ͬΪ0����ͬΪ1������Եó��������λ��Ϊ1. 
	���������λ�������ֿ��ܣ�1.��ʼ�ɿ������ڰ��¡� 2.��ʼ���£������ɿ���
	
	keyBoard->keyPressedJump�������λ��Ϊ1 �� ���ڼ��̵�״̬���룬����Եó�������Щ���ǰ��µ� 
	keyBoard->keyReleaseJump�������λ��Ϊ1 �� ֮ǰ���̵�״̬���룬����Եó�������Щ�����ɿ��� 		
	*/
	keyBoard->keyPressedJump = (keyBoard->KeyStatus ^ keyBoard->lastKeyStatus) & keyBoard->KeyStatus;
  keyBoard->keyReleaseJump = (keyBoard->KeyStatus ^ keyBoard->lastKeyStatus) & keyBoard->lastKeyStatus;
  
	
	/*
	�����жϣ�
	���ĳ����һֱ�����ţ��ͻ���볤���жϣ�ÿ����һ�ζ�Ӧλ��counter�ͻ��һ�Σ�������50��֮��
	������־λ����1��
	���û�б�����֮�����ִ����Ӧ�����������
	����iΪ5 
	1. ~(1<<i) -> 10000  ->  01111
	2. keyBoard->longPressed �� 01111 ���� ��Ὣ��Ӧλ��0
	*/
  for(int i = 0; i < 16; i++){
    if(keyBoard->KeyStatus & 1<<i){
      keyBoard->timeCounter[i]++;
      if(keyBoard->timeCounter[i] > DR16_KEY_LONGPRESS_TIMEOUT){
        keyBoard->longPressed |= 1<<i;
        keyBoard->timeCounter[i] = 0;
      }
    }
    else{
      keyBoard->longPressed &= ~(1<<i);
			keyBoard->timeCounter[i] = 0;
    }
    
  }
  
	keyBoard->lastKeyStatus = keyBoard->KeyStatus;
  
}

/**
  * @brief  DR16���ջ���ʼ��
  * @param  None
  * @retval None
  */
void DR16_receiverInit(void){
	USART1_QuickInit(100000);
	USART1_RXDMA_Config((uint32_t)DR16_rxBuff, DR16_DBUS_PACKSIZE+2);	//+2��֤��ȫ
  
}


/**
  * @brief  ����X��Y����
  * @param  posX posY ���̻�ң�ص�X Y��ֵ
  * @retval None
  */
void Rocker_getData(float posX, float posY, rocker_t *roc){
	roc->x = posX;
	roc->y = posY;
	roc->radian = atan2(roc->y, roc->x);

}


/**
  * @brief  DR16�������ݴ���
  * @param  pData			����buff����ָ��
  * @retval None
  */
void DR16_dataProcess(uint8_t *pData){
	if (pData == NULL)
	{
		return;
	}
	dr16_data.rc.ch0 = (pData[0] | (pData[1] << 8)) & 0x07FF;
	dr16_data.rc.ch1 = ((pData[1] >> 3) | (pData[2] << 5)) & 0x07FF;
	dr16_data.rc.ch2 = ((pData[2] >> 6) | (pData[3] << 2) | (pData[4] << 10)) & 0x07FF;
	dr16_data.rc.ch3 = ((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF;
	dr16_data.rc.s_left = ((pData[5] >> 4) & 0x000C) >> 2;
	dr16_data.rc.s_right = ((pData[5] >> 4) & 0x0003);
	dr16_data.mouse.x = (pData[6]) | (pData[7] << 8);
	dr16_data.mouse.y = (pData[8]) | (pData[9] << 8);
	dr16_data.mouse.z = (pData[10]) | (pData[11] << 8);
	dr16_data.mouse.keyLeft = pData[12];
	dr16_data.mouse.keyRight = pData[13];
	dr16_data.keyBoard.key_code = pData[14]| (pData[15] << 8);
  dr16_data.rc.ch4 = 	pData[16]| (pData[17] << 8);
	//your control code ��.
	
	dr16_data.DR16InfoUpdateFrame++;
	
	dr16_data.rc.ch0 -=1024;
	dr16_data.rc.ch1 -=1024;
	dr16_data.rc.ch2 -=1024;
	dr16_data.rc.ch3 -=1024;
	dr16_data.rc.ch4 -=1024;	
	/* prevent remote control zero deviation */
	if(dr16_data.rc.ch0 <= 20 && dr16_data.rc.ch0 >= -20)
		dr16_data.rc.ch0 = 0;
	if(dr16_data.rc.ch1 <= 20 && dr16_data.rc.ch1 >= -20)
		dr16_data.rc.ch1 = 0;
	if(dr16_data.rc.ch2 <= 20 && dr16_data.rc.ch2 >= -20)
		dr16_data.rc.ch2 = 0;
	if(dr16_data.rc.ch3 <= 20 && dr16_data.rc.ch3 >= -20)
		dr16_data.rc.ch3 = 0;
	
  ControlStatus.ControlLeft=dr16_data.rc.s_left;
	ControlStatus.ControlRight=dr16_data.rc.s_right;
		
	Rocker_getData(dr16_data.rc.ch2, dr16_data.rc.ch3, &dr16_rocker_L);
	Rocker_getData(dr16_data.rc.ch0, dr16_data.rc.ch1, &dr16_rocker_R);
	
	DR16_mouseProcess(dr16_data.mouse.keyLeft, &dr16_mouseKeyLeft);
	DR16_mouseProcess(dr16_data.mouse.keyRight, &dr16_mouseKeyRight);
  
  DR16_keyBoardProcess(dr16_data.keyBoard.key_code, &dr16_keyBorad);	
		
	}
	
	
	
/**
  * @brief  ��������������,���ú�������Ӧ������λ
  * @param  KeyCode		��Ҫ���ļ�ֵ
  * 		mode		0 - ��ƥ����
  *						1 - ��ȫƥ��
  * @retval �Ƿ��������ֵ�İ�����������
  */
uint16_t dr16_keyboard_isPressedJumpKey(uint16_t KeyCode, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - ��ȫƥ��
    temp = dr16_keyBorad.keyPressedJump == KeyCode;
	}
	else{		//0 - ��ƥ����
    temp = dr16_keyBorad.keyPressedJump & KeyCode;
	}
  
  dr16_keyBorad.keyPressedJump &= ~KeyCode;
	return temp;
	
}

/**
  * @brief  �����ſ�������,���ú�������Ӧ������λ
  * @param  KeyCode		��Ҫ���ļ�ֵ
  * 		mode		0 - ��ƥ����
  *						1 - ��ȫƥ��
  * @retval �Ƿ��������ֵ�İ�����������
  */
uint16_t dr16_keyboard_isReleaseJumpKey(uint16_t KeyCode, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - ��ȫƥ��
    temp = dr16_keyBorad.keyReleaseJump == KeyCode;
	}
	else{		//0 - ��ƥ����
    temp = dr16_keyBorad.keyReleaseJump & KeyCode;
	}
  
  dr16_keyBorad.keyReleaseJump &= ~KeyCode;
	return temp;
	
}

/**
  * @brief  ����������������,���ú�������Ӧ������λ
  * @param  KeyCode		��Ҫ���ļ�ֵ
  * 		mode		0 - ��ƥ����
  *						1 - ��ȫƥ��
  * @retval �Ƿ��������ֵ�İ�����������
  */
uint16_t dr16_mouseL_isPressedJumpKey(uint16_t mouse, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - ��ȫƥ��
    temp = dr16_mouseKeyLeft.flag.keyPressedJump == mouse;
	}
	else{		//0 - ��ƥ����
    temp = dr16_mouseKeyLeft.flag.keyPressedJump & mouse;
	}
  
  dr16_mouseKeyLeft.flag.keyPressedJump &= ~mouse;
	return temp;
	
}

uint16_t dr16_mouseR_isPressedJumpKey(uint16_t mouse, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - ��ȫƥ��
    temp = dr16_mouseKeyRight.flag.keyPressedJump == mouse;
	}
	else{		//0 - ��ƥ����
    temp = dr16_mouseKeyRight.flag.keyPressedJump & mouse;
	}
  
  dr16_mouseKeyRight.flag.keyPressedJump &= ~mouse;
	return temp;
	
}

/**
  * @brief  �������¼��
  * @param  KeyCode		��Ҫ���ļ�ֵ
  * @retval �Ƿ��������ֵ�İ�������
  */
uint16_t dr16_keyboard_isPressedKey(uint16_t KeyCode, uint8_t mode){
	
	if(mode){	//1 - ��ȫƥ��
		return dr16_data.keyBoard.key_code == KeyCode;
	}
	else{		//0 - ��ƥ����
		return dr16_data.keyBoard.key_code & KeyCode;
	}
	
}

/**
  * @brief  ��ϼ��жϴ��� 
  * @param  None
  * @retval None
  */
void Control_combineKeyScan(void)
{
	if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_CTRL, KEY_FULL_MATCH) || dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_SHIFT, KEY_FULL_MATCH)){
			//�����Ctrl��shift���Ȱ��£�����û������������ʱ�ж�Ϊ��ϼ�
			ControlStatus.isCombineKey = 1;
		}
	if(dr16_keyBorad.KeyStatus == KEYBOARD_PRESSED_NONE){
			//�����ϼ�ȫ���ɿ���ر���ϼ�
			ControlStatus.isCombineKey = 0;
	}
	
}


uint8_t Control_isCombineKey(void){
	return ControlStatus.isCombineKey;
}
