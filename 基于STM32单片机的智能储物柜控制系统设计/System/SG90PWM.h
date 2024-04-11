#ifndef __SG90PWM_H
#define __SG90PWM_H
#include "stdbool.h"
#define 		ARR 	20000
#define 		PSC		72
#define 		PULSE	0
#define 	SG90_OPEN		1
#define 	SG90_CLOSE		0
void SG90PWM_Init(void);
void SG90_select(bool mode,u8 order);
void SetSG90PWM_OC_CH3_TIM3(uint16_t Pulse);		// PB0-->TIM3_CH3	J1

void SetSG90PWM_OC_CH1_TIM4(uint16_t Pulse);		// PB6-->TIM4_CH1	J2

void SetSG90PWM_OC_CH2_TIM4(uint16_t Pulse);		// PA7-->TIM4_CH2	J3

void SetSG90PWM_OC_CH3_TIM4(uint16_t Pulse);		// PB8-->TIM4_CH3	J4

void SetSG90PWM_OC_CH4_TIM4(uint16_t Pulse);		// PB9-->TIM4_CH4	J5

#endif
