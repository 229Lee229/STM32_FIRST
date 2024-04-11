#include "stm32f10x.h"                  // Device header
#include "My_include.h"



// 舵机转换
uint16_t Shift_Angle(float Count){
	
	return Count / 180 * 2000 + 500;
	
}

