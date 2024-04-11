
#include "My_include.h"


struct TIM_judge_val TIM_judge_val_1;
extern u8 Exist_TIM2_limit;					// ����λ ����ֻ�������ֻ���ʱ��ʾ
extern u32 Input_Phone_Num[];
extern int8_t Array_index;
void Exist_TIM_Init(void){
	/* ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* ѡ���ڲ�ʱ�� */
	TIM_InternalClockConfig(TIM2);
	
	/* ����ʱ����Ԫ */
	TIM_TimeBaseInitTypeDef TIM_TIMEBaseInitStructure;
	TIM_TIMEBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TIMEBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TIMEBaseInitStructure.TIM_Period = 10000 - 1;		/* ARR */
	TIM_TIMEBaseInitStructure.TIM_Prescaler = 7200 - 1;		/* PSC */
	TIM_TIMEBaseInitStructure.TIM_RepetitionCounter = 0;    
	TIM_TimeBaseInit(TIM2,&TIM_TIMEBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	/* ʹ���ж� */
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	/* ��ʼ��NVIC */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		/* ѡ��NVIC���ȼ����� */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* ʹ�ܿ��� */
	TIM_Cmd(TIM2,ENABLE);
	TIM_judge_val_1.lcd_Exist_flag_Num = 0;
}


void TIM2_IRQHandler(void){
	/* ȷ���ж�λ */
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
		TIM_judge_val_1.lcd_Exist_flag_Num++;	
	}
	/* �����־λ */
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}	
