#include "PC_Mode.h"

PCMode_t PCMode;

void PC_Control_Init(void){
  
	PCMode.chassisSpeedcoe = 1;
	PCMode.cloundSpeedcoe_x = 0.025;
	PCMode.cloundSpeedcoe_y = 0.5f;
  PCMode.suppiyControl = 0;
	PCMode.normalControlCounter = 0;
	
}





