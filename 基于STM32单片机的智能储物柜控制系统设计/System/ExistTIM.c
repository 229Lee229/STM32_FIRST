
#include "My_include.h"


struct TIM_judge_val TIM_judge_val_1;
extern u8 Exist_TIM2_limit;					// 限制位 限制只在输入手机号时显示
extern u32 Input_Phone_Num[];
extern int8_t Array_index;
void Exist_TIM_Init(void){
	/* 开启时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* 选择内部时钟 */
	TIM_InternalClockConfig(TIM2);
	
	/* 配置时基单元 */
	TIM_TimeBaseInitTypeDef TIM_TIMEBaseInitStructure;
	TIM_TIMEBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TIMEBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TIMEBaseInitStructure.TIM_Period = 10000 - 1;		/* ARR */
	TIM_TIMEBaseInitStructure.TIM_Prescaler = 7200 - 1;		/* PSC */
	TIM_TIMEBaseInitStructure.TIM_RepetitionCounter = 0;    
	TIM_TimeBaseInit(TIM2,&TIM_TIMEBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	/* 使能中断 */
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	/* 初始化NVIC */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		/* 选择NVIC优先级分组 */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* 使能控制 */
	TIM_Cmd(TIM2,ENABLE);
	TIM_judge_val_1.lcd_Exist_flag_Num = 0;
}


void TIM2_IRQHandler(void){
	/* 确定中断位 */
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		TIM_judge_val_1.lcd_Exist_flag_Num++;	
	}
	/* 清除标志位 */
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}	
