/**********************************************************************
 * flie：  矩阵键盘4*4 Init
 * brief： 纵列(A7~A4)使用上拉输入  横列(A3~A0)使用推挽输出低电平
			读输入寄存器来判断具体键定位
 * Input： void
 * Output：void
 * Retval：void
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/26/2024	  Lee		增加TIM1CH1与CH3输出 配置主输出使能
 ***********************************************************************/
#include "stm32f10x.h"                  // Device header
#include "delay.h"


/*********************************************


			*C4		*C3		*C2		*C1
			A7		A6		A5		A4
*R4 A3		S1		S2		S3		S4
*R3 A2		S5		S6		S7		S8
*R2 A1		S9		S10		S11		S12
*R1 A0		S13		S14		S15		S16




****************************************/

void Matrix_ssKey_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}	


int Matrix_Key_Scan2(void)
{
	u8 temp = 0;
	int key_val = -1;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);			//拉低行线
	delay_us(10);
 
	temp=(GPIO_ReadInputData(GPIOA))&0xff;	
	
	//没有按键按下时扫描
	if (temp == 0xf0) 
	{
			delay_ms(30); 
			GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);			//拉低行线
			delay_us(10);
			temp=(GPIO_ReadInputData(GPIOA))&0xff;	
		
			if (temp != 0xf0) //按键按下时，对键值进行赋值
			{
				//第一行
				GPIO_Write(GPIOA,0);
				delay_ms(5);
				GPIO_Write(GPIOA,(uint16_t)(0xFE)); 
				
				if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
				{
						delay_ms(20);//消抖
 
						if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOA)) & 0XFE);		//对列进行扫描
								switch(temp)
								{
										case 0xEE:  key_val = 15/* Exit */;   break;			// 15
										case 0xDE:  key_val = 14/* Close */;   break;		// 14
										case 0xBE:  key_val = 0;   break;				
										case 0x7E:  key_val = 13/* ReInput */;   break;
										default:    key_val = -1;   break;
								}
						}
				}
				
				//第二行
				GPIO_Write(GPIOA,0);
				delay_ms(5);
				GPIO_Write(GPIOA,(uint16_t)(0xFD));
				
				if(((GPIO_ReadInputData(GPIOA)) & 0XF0)!= 0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOA)) & 0XFD);
								switch(temp)
								{
										case 0xED:  key_val = 12/* Enter */;   break;			// 12
										case 0xDD:  key_val = 3/* */;   break;				// 3
										case 0xBD:  key_val = 2;   break;				// 2
										case 0x7D:  key_val = 1;   break;					// 1
										default:    key_val = -1;   break;
								}
						}
				}
				
				//第三行
				GPIO_Write(GPIOA,0);
				delay_ms(5);
				GPIO_Write(GPIOA,(uint16_t)(0xFB));
				
				if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOA)) & 0XFB);
								switch(temp)
								{
										case 0xEB:  key_val = 11/* MoveDown */;   break;		// 11
										case 0xDB:  key_val = 6;   break;				// 6
										case 0xBB:  key_val = 5;   break;					// 5
										case 0x7B:  key_val = 4;   break;					// 4
										default:    key_val = -1;   break;
								}
						}
				}
				
				//第四行
				GPIO_Write(GPIOA,0);
				delay_ms(5);
				GPIO_Write(GPIOA,(uint16_t)(0xF7));
				
				if(((GPIO_ReadInputData(GPIOA)) & 0XF0) !=0XF0)
				{
						delay_ms(20);
 
						if(((GPIO_ReadInputData(GPIOA)) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOA)) & 0XF7);
								switch(temp)
								{
										case 0xE7:  key_val = 10/* MoveUp*/;   break;		// 10
										case 0xD7:  key_val = 9;   break;					// 9
										case 0xB7:  key_val = 8;   break;					// 8
										case 0x77:  key_val = 7;   break;					// 7
										default:    key_val = -1;   break;
								}
						}
					}
				}
			}
	
	return key_val;
 
}
