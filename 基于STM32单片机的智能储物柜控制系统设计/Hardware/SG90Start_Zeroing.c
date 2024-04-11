/**********************************************************************
* flie：  上电后统一置零度
* brief： 上电后所有舵机默认初始化零度
 * Input： 无
 * Output：无
 * Retval：无
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/22/2024	  Lee				
 ***********************************************************************/
#include "stm32f10x.h"                  // Device header
#include "SG90PWM.h"
#include "SG90Angle.h"



// Servo Motor 角度统一置零
void SG90_Start_Zeroing_Init(void){
	SetSG90PWM_OC_CH3_TIM3(Shift_Angle(0));
	SetSG90PWM_OC_CH1_TIM4(Shift_Angle(0));
	SetSG90PWM_OC_CH2_TIM4(Shift_Angle(0));
	SetSG90PWM_OC_CH3_TIM4(Shift_Angle(0));
	SetSG90PWM_OC_CH4_TIM4(Shift_Angle(0));
}

void SG90_Start_Zeroing_test_180(void){
	SetSG90PWM_OC_CH3_TIM3(Shift_Angle(180));
	SetSG90PWM_OC_CH1_TIM4(Shift_Angle(180));
	SetSG90PWM_OC_CH2_TIM4(Shift_Angle(180));
	SetSG90PWM_OC_CH3_TIM4(Shift_Angle(180));
	SetSG90PWM_OC_CH4_TIM4(Shift_Angle(180));	
}


