/**
  ******************************************************************************
  * @file    DR16_Receiver.c
  * @author  Liang Yuhao
  * @version V1.0
  * @date    
  * @brief   DR16接收机应用函数接口
  ******************************************************************************
  */
  
  
#include <Math.h>
#include "DR16_Receiver.h"

uint8_t DR16_rxBuff[DR16_DBUS_PACKSIZE+2]; 	//接收buff
DR16_DBUS_DATA_t dr16_data;

ControlStatus_t ControlStatus;


keyBoard_t dr16_keyBorad;
key_t dr16_mouseKeyLeft;
key_t dr16_mouseKeyRight;

rocker_t dr16_rocker_L,dr16_rocker_R;
rocker_t pcRocker;
#define DR16_KEY_LONGPRESS_TIMEOUT 50




	
/**
  * @brief  DR16鼠标按键处理
  * @param  newKeyStatus 新按键状态，按下为1，未按下为0
  *         *key 按键结构体指针 @ref key_t
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
  * @brief  DR16键盘按键处理
  * @param  newKeyStatus 新按键状态，按下对应位为1，未按下为0
  *         *keyBoard 按键结构体指针 @ref key_t
  * @retval None
  */
void DR16_keyBoardProcess(uint16_t newKeyStatus, keyBoard_t *keyBoard){
	keyBoard->keyPressedJump = 0;
	keyBoard->keyReleaseJump = 0;
	
	keyBoard->KeyStatus = newKeyStatus;
	/*
	（keyBoard->KeyStatus ^ keyBoard->lastKeyStatus）：相同为0，不同为1。则可以得出，跳变的位置为1. 
	对于跳变的位置有两种可能：1.开始松开，现在按下。 2.开始按下，现在松开。
	
	keyBoard->keyPressedJump：跳变的位置为1 和 现在键盘的状态相与，则可以得出现在哪些键是按下的 
	keyBoard->keyReleaseJump：跳变的位置为1 和 之前键盘的状态相与，则可以得出现在哪些键是松开的 		
	*/
	keyBoard->keyPressedJump = (keyBoard->KeyStatus ^ keyBoard->lastKeyStatus) & keyBoard->KeyStatus;
  keyBoard->keyReleaseJump = (keyBoard->KeyStatus ^ keyBoard->lastKeyStatus) & keyBoard->lastKeyStatus;
  
	
	/*
	长按判断：
	如果某个键一直被按着，就会进入长按判断，每进来一次对应位的counter就会加一次，当加了50次之后，
	长按标志位被置1。
	如果没有被按了之后，则会执行相应的清楚操作。
	假设i为5 
	1. ~(1<<i) -> 10000  ->  01111
	2. keyBoard->longPressed 和 01111 相与 则会将对应位置0
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
  * @brief  DR16接收机初始化
  * @param  None
  * @retval None
  */
void DR16_receiverInit(void){
	USART1_QuickInit(100000);
	USART1_RXDMA_Config((uint32_t)DR16_rxBuff, DR16_DBUS_PACKSIZE+2);	//+2保证安全
  
}


/**
  * @brief  产生X，Y坐标
  * @param  posX posY 键盘或遥控的X Y数值
  * @retval None
  */
void Rocker_getData(float posX, float posY, rocker_t *roc){
	roc->x = posX;
	roc->y = posY;
	roc->radian = atan2(roc->y, roc->x);

}


/**
  * @brief  DR16接收数据处理
  * @param  pData			接收buff数据指针
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
	//your control code ….
	
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
  * @brief  按键按下跳变检测,调用后会清除对应的跳变位
  * @param  KeyCode		需要检测的键值
  * 		mode		0 - 有匹配项
  *						1 - 完全匹配
  * @retval 是否有这个键值的按键按下跳变
  */
uint16_t dr16_keyboard_isPressedJumpKey(uint16_t KeyCode, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - 完全匹配
    temp = dr16_keyBorad.keyPressedJump == KeyCode;
	}
	else{		//0 - 有匹配项
    temp = dr16_keyBorad.keyPressedJump & KeyCode;
	}
  
  dr16_keyBorad.keyPressedJump &= ~KeyCode;
	return temp;
	
}

/**
  * @brief  按键放开跳变检测,调用后会清除对应的跳变位
  * @param  KeyCode		需要检测的键值
  * 		mode		0 - 有匹配项
  *						1 - 完全匹配
  * @retval 是否有这个键值的按键按下跳变
  */
uint16_t dr16_keyboard_isReleaseJumpKey(uint16_t KeyCode, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - 完全匹配
    temp = dr16_keyBorad.keyReleaseJump == KeyCode;
	}
	else{		//0 - 有匹配项
    temp = dr16_keyBorad.keyReleaseJump & KeyCode;
	}
  
  dr16_keyBorad.keyReleaseJump &= ~KeyCode;
	return temp;
	
}

/**
  * @brief  鼠标左键按下跳变检测,调用后会清除对应的跳变位
  * @param  KeyCode		需要检测的键值
  * 		mode		0 - 有匹配项
  *						1 - 完全匹配
  * @retval 是否有这个键值的按键按下跳变
  */
uint16_t dr16_mouseL_isPressedJumpKey(uint16_t mouse, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - 完全匹配
    temp = dr16_mouseKeyLeft.flag.keyPressedJump == mouse;
	}
	else{		//0 - 有匹配项
    temp = dr16_mouseKeyLeft.flag.keyPressedJump & mouse;
	}
  
  dr16_mouseKeyLeft.flag.keyPressedJump &= ~mouse;
	return temp;
	
}

uint16_t dr16_mouseR_isPressedJumpKey(uint16_t mouse, uint8_t mode){
  
	uint16_t temp;
	if(mode){	//1 - 完全匹配
    temp = dr16_mouseKeyRight.flag.keyPressedJump == mouse;
	}
	else{		//0 - 有匹配项
    temp = dr16_mouseKeyRight.flag.keyPressedJump & mouse;
	}
  
  dr16_mouseKeyRight.flag.keyPressedJump &= ~mouse;
	return temp;
	
}

/**
  * @brief  按键按下检测
  * @param  KeyCode		需要检测的键值
  * @retval 是否有这个键值的按键按下
  */
uint16_t dr16_keyboard_isPressedKey(uint16_t KeyCode, uint8_t mode){
	
	if(mode){	//1 - 完全匹配
		return dr16_data.keyBoard.key_code == KeyCode;
	}
	else{		//0 - 有匹配项
		return dr16_data.keyBoard.key_code & KeyCode;
	}
	
}

/**
  * @brief  组合键判断处理 
  * @param  None
  * @retval None
  */
void Control_combineKeyScan(void)
{
	if(dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_CTRL, KEY_FULL_MATCH) || dr16_keyboard_isPressedJumpKey(KEYBOARD_PRESSED_SHIFT, KEY_FULL_MATCH)){
			//如果有Ctrl或shift键先按下，并且没有其他键按下时判断为组合键
			ControlStatus.isCombineKey = 1;
		}
	if(dr16_keyBorad.KeyStatus == KEYBOARD_PRESSED_NONE){
			//如果组合键全部松开则关闭组合键
			ControlStatus.isCombineKey = 0;
	}
	
}


uint8_t Control_isCombineKey(void){
	return ControlStatus.isCombineKey;
}
