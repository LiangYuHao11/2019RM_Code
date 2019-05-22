/**
  ******************************************************************************
  * @file    DevicesMonitor.c
  * @author  Hare
  * @version V1.0
  * @date    
  * @brief  Ö¡ÂÊ¼ì²â
	******************************************************************************
  */
  
  
#include "DevicesMonitor.h"

//ÉÁµÆ¼ÆÊýÆ÷
static uint8_t led_on_counter = 0;
//ÀëÏß±äÁ¿£¬ÓÃÒ»¸ö8Î»µÄ±äÁ¿£¬´¢´æËùÓÐÊý¾Ý
uint8_t Devi_Offline = 0;
/**
  * @brief  Ö¡ÂÊ¼ì²âº¯Êý
  * @param  None
  * @retval None
  */
void DevicesMonitor_update(void){
	
  static uint8_t Devi_counter_low = 0;
  static uint8_t Devi_counter_fast = 0;
  static uint8_t Devi_counter_LED;

	
	Devi_counter_low++;
	Devi_counter_fast++;
	Devi_counter_LED++;
	
	//¼ì²â·¢ËÍ±È½Ï¿ìµÄÉè±¸
	if(Devi_counter_fast > 5)
	{
		//M3508
			for(int i = 0; i<5; i++){
				if(M3508s[i].M3508InfoUpdateFrame < 5){
					M3508s[i].M3508OffLineFlag = 1;
					if(M3508s[0].M3508OffLineFlag || M3508s[1].M3508OffLineFlag || M3508s[2].M3508OffLineFlag || M3508s[3].M3508OffLineFlag || M3508s[4].M3508OffLineFlag)
					{
						Devi_Offline |= 0x01; 
					}
				}
				else{
					M3508s[i].M3508OffLineFlag = 0;
					if(M3508s[0].M3508OffLineFlag == M3508s[1].M3508OffLineFlag == M3508s[2].M3508OffLineFlag \
						== M3508s[3].M3508OffLineFlag == M3508s[4].M3508OffLineFlag == 0)
					{
						Devi_Offline &= ~1; 	
					}						
				 }
				M3508s[i].M3508InfoUpdateFrame = 0;
			}

			for(int i = 0; i<2; i++){
				if(M2006s[i].M2006InfoUpdateFrame < 5){
					M2006s[i].M2006OffLineFlag = 1;
					Devi_Offline |= 0x02; 
				}
				else{
					M2006s[i].M2006OffLineFlag = 0;
					Devi_Offline &= ~(1<<1);
				}
				M2006s[i].M2006InfoUpdateFrame = 0;
			}	

			for(int i = 0; i<3; i++){
				if(M6020s[i].M6020InfoUpdateFrame < 5){
					M6020s[i].M6020OffLineFlag = 1;
					if(M6020s[0].M6020InfoUpdateFrame ||M6020s[1].M6020InfoUpdateFrame || M6020s[2].M6020InfoUpdateFrame)
          {
					  Devi_Offline |= 0x04; 
					}
				}
				else{
					M6020s[i].M6020OffLineFlag = 0;
					if(M6020s[0].M6020InfoUpdateFrame == M6020s[1].M6020InfoUpdateFrame == M6020s[2].M6020InfoUpdateFrame == 0)
					{					
					  Devi_Offline &= ~(1<<2);
					}
					
				}
				M6020s[i].M6020InfoUpdateFrame = 0;
			}	
			
			//è®¡ç®—å™¨æ¸…é›¶
			Devi_counter_fast = 0;
		}
	
		//¼ì²â·¢ËÍ±È½ÏÂýµÄÉè±¸
		if(Devi_counter_low>20)
		{
			if(GY_6050[0].ImuInfoUpdateFrame < 4){
					GY_6050[0].ImuOffLineFlag = 1;
					Devi_Offline |= 0x08; 
			}
			else{
				GY_6050[0].ImuOffLineFlag = 0;
				Devi_Offline &= ~(1<<3);
			}
			if(GY_6050[0].ImuInfoUpdateFrame > 2)
			{
				GY_6050[0].ImuInfoUpdateFrame = 0;
		  }
		
			if(dr16_data.DR16InfoUpdateFrame < 5){
				dr16_data.DR16OffLineFlag = 1;
        Devi_Offline |= 0x10; 				
			}
			else{
				dr16_data.DR16OffLineFlag = 0;
				Devi_Offline &= ~(1<<4);
				
			}
			dr16_data.DR16InfoUpdateFrame = 0;
			
		  Devi_counter_low = 0;
		}			
				
}


/**
  * @brief  Ö¡ÂÊ¼ì²âÉÁµÆÖ¸Ê¾º¯Êý
  * @param  None
  * @retval None
  */
void DevicesLED_Mode(void)
{	 
	  led_on_counter++;		
	
		if(Devi_Offline == 0)
		{
			LED_GREEN(0);
			led_on_counter = 0;
		}
		
		else
		{ 
			//ÍÓÂÝÒÇÀëÏß£¬ºìµÆ¿ìÉÁ
			if(Devi_Offline == 0x08)
			{
				if(led_on_counter > 30)
				{
					LED_GREEN(1);
					LEDRED_TOGGLE;
					led_on_counter = 0;				
				}	
			}

			//ÍÓÂÝÒÇÀëÏß£¬ÂÌµÆ¿ìÉÁ			
			else if(Devi_Offline == 0x10)
			{
				if(led_on_counter > 30)
				{
					LED_RED(1);
					LEDGREEN_TOGGLE;
					led_on_counter = 0;				
				}	
			}			
			
			//µ×ÅÌÀëÏß£¬ºìµÆÂýÉÁ
			else if(Devi_Offline == 0x01)
			{
				if(led_on_counter > 100)
				{
					LED_GREEN(1);					
					LEDRED_TOGGLE;
					led_on_counter = 0;				
				}	
			}						
			
			//ÔÆÌ¨ÀëÏß£¬ÂÌµÆÂýÉÁ			
			else 
			{
				if(led_on_counter > 100)
				{
					LED_RED(1);					
					LEDGREEN_TOGGLE;
					led_on_counter = 0;				
				}	
			}	
			
	  }


}











