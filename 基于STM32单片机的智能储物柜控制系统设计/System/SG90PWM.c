/**********************************************************************
 * flie：  PWM
 * brief： 初始化PWM  TIM1 TIM2 TIM3 
 * Input： 无
 * Output：无
 * Retval：无
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/30/2024	  Lee			配置TIM3 TIM4 SG90输出信号
 ***********************************************************************/


/**********************************************************************
舵机PWM控制IO口
PB6			-->		TIM4_CH1			J1
PA7			-->		TIM4_CH2			J2
PB8			-->		TIM4_CH3			J3
PB9			-->		TIM4_CH4			J4
PB0			-->		TIM3_CH3			J5


**********************************************************************/
#include "stm32f10x.h"                  // Device header
#include "My_include.h"
#define 		ARR 	20000
#define 		PSC		72
#define 		PULSE	0



void SG90_select(bool mode,u8 order){
	if(mode == SG90_OPEN){				// open
		switch(order){
			case 1:	SetSG90PWM_OC_CH3_TIM3(Shift_Angle(180));
					USART_SendString(UART1_JR6001,JR6001_1ST_OPENED);
					break;
				
			case 2:SetSG90PWM_OC_CH1_TIM4(Shift_Angle(180));			
					USART_SendString(UART1_JR6001,JR6001_2ND_OPENED);
					break;
				
			case 3:SetSG90PWM_OC_CH2_TIM4(Shift_Angle(180));
					USART_SendString(UART1_JR6001,JR6001_3RD_OPENED);
					break;
				
			case 4:SetSG90PWM_OC_CH3_TIM4(Shift_Angle(180));
					USART_SendString(UART1_JR6001,JR6001_4TH_OPENED);
					break;
				
			case 5:SetSG90PWM_OC_CH4_TIM4(Shift_Angle(180));;	
					USART_SendString(UART1_JR6001,JR6001_5TH_OPENED);
					break;
			default:break;
		}
	}else if(mode == SG90_CLOSE){		// close
		switch(order){
			case 1:	SetSG90PWM_OC_CH3_TIM3(Shift_Angle(0));
					break;
				
			case 2:SetSG90PWM_OC_CH1_TIM4(Shift_Angle(0));			
					break;
				
			case 3:SetSG90PWM_OC_CH2_TIM4(Shift_Angle(0));
					break;
				
			case 4:SetSG90PWM_OC_CH3_TIM4(Shift_Angle(0));
					break;
				
			case 5:SetSG90PWM_OC_CH4_TIM4(Shift_Angle(0));
					break;
			default:break;
		}
		
	}
	
	
}


/**
  * @brief  PWM初始化
  * @param  无
  * @retval 无
  */
void SG90PWM_Init(void){
	/* 开启时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);			// 启用复用
	
	
	
	
	/****  // 映射引脚
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);		// TIM2 CH3 PB10
																// TIM2 CH4 PB11
	****/
	
	// PB0 PB1 PB8 PB9 Init
	GPIO_Pin_Init(PB0,GPIO_Mode_AF_PP);
	GPIO_Pin_Init(PB6,GPIO_Mode_AF_PP);
	GPIO_Pin_Init(PB7,GPIO_Mode_AF_PP);
	GPIO_Pin_Init(PB8,GPIO_Mode_AF_PP);
	GPIO_Pin_Init(PB9,GPIO_Mode_AF_PP);

	// GPIOA Init
	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);*/
	
	// 时钟源选择
	// TIM_InternalClockConfig(TIM1);
	TIM_InternalClockConfig(TIM4);
	TIM_InternalClockConfig(TIM3);
	
	// 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = ( ARR - 1 );		// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = ( PSC - 1 );		// PSC // 保持50Hz频率72M / 72 / 20000 = 50Hz = 20ms
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
//	TIM_ClearFlag(TIM1,TIM_FLAG_Update);				// 清除中断标志位
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);				// 更新中断标志位
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	
	// 配置输出比较
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);					// 提前初始化 一些引脚不必配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// *输出模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// *输出极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// *输出状态
	TIM_OCInitStructure.TIM_Pulse = PULSE;								// *占空比CCR
	
	// TIM1
	// TIM_OC4Init(TIM1,&TIM_OCInitStructure);
	//TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);					// 自动重装载荷
	
	// TIM3
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	// TIM4
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	
	// 使能时钟
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	// TIM_CtrlPWMOutputs(TIM1,ENABLE); 高级定时器必须加上 
}



/**
  * @brief  设置CCR 
  * @param  Pulse		输入占空比 控制PWM输出波形
  * @retval 无
  */
void SetSG90PWM_OC_CH3_TIM3(uint16_t Pulse){		// PB0-->TIM3_CH3	J1
	TIM_SetCompare3(TIM3,Pulse);
}
void SetSG90PWM_OC_CH1_TIM4(uint16_t Pulse){		// PB6-->TIM4_CH1	J2
	TIM_SetCompare1(TIM4,Pulse);
}
void SetSG90PWM_OC_CH2_TIM4(uint16_t Pulse){		// PA7-->TIM4_CH2	J3
	TIM_SetCompare2(TIM4,Pulse);
}
void SetSG90PWM_OC_CH3_TIM4(uint16_t Pulse){		// PB8-->TIM4_CH3	J4
	TIM_SetCompare3(TIM4,Pulse);
}
void SetSG90PWM_OC_CH4_TIM4(uint16_t Pulse){		// PB9-->TIM4_CH4	J5
	TIM_SetCompare4(TIM4,Pulse);
}

