#include "My_include.h"


extern u16 unavailable;

void lcd_menu_Init(u8 mode){
	
	
	switch(mode){
		case 1:			
			LCD_Clear(BLACK);
			Gui_StrCenter(0,49,WHITE,BLACK,"Init...",16,0);
			break;
		case 2:
			LCD_Clear(WHITE);
			// DrawTestPage("���ܴ����ӭ��");		/* ������ */
			Show_Str(0,0,WHITE,BLUE,"���ܴ����ӭ��",16,0);
			// Show_Str(1,17,BLUE,YELLOW,"Type...                    ",12,0);
			Gui_StrCenter(1,17,BLACK,WHITE,"Type...",12,0);			/* ������ʾ */
			Show_Str(9,30,BLUE,WHITE,"1-ȡ����ȡ��",16,0);
			Show_Str(9,49,BLUE,WHITE,"2-����ͨ��ȡ��",16,0);
			Show_Str(9,66,BLUE,WHITE,"3-���",16,0);
			Show_Str(9,83,BLUE,WHITE,"4-��ϵ��̨",16,0);
			// LCD_ShowNum(9,100,12312300,8,16);
			break;
		default:	break;
	}


	
	
}

void lcd_menu_1st(void){
	
	if(unavailable == 0){
		LCD_Clear(WHITE);
		Show_Str(0,50,BLACK,WHITE,"ȡ��ʧ��",16,0);
		Show_Str(0,67,BLACK,WHITE,"�޴洢����",16,0);	
		USART_SendString(UART1_JR6001,JR6001_PICK_FAIL); 
		u8 i = 5;
		while(i--)	delay_ms(500);	
		lcd_menu_Init(LCD_DISPLAY_WEL_menu);
		return;
	}
	
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_INPUT_CODE);		/* ����������ȡ���� */
	Show_Str(0,0,WHITE,BLUE,"���ܴ����ӭ��",16,0);
	Show_Str(1,17,BLACK,WHITE,"������ȡ����",16,0);
	// GUI_DrawFont16(1, 17, BLACK, YELLOW, "Please enter the pickup code",0);
	menu_1st_pick();
}

/* Bluetooth pickup */
void lcd_menu_2nd(void){
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_LINK_BULETOOTH);		// ����������ȡ�� 
	Show_Str(9,30,BLUE,WHITE,"����������ȡ��",16,0);
	
	
	while(1){
		if(Matrix_Key_Scan2() == key_Exit){			// ����������а���Exit ���˳� ����������  ������temp ��Ϊ���鲻������λ������
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}

	}

}

/* Storing objects*/
void lcd_menu_3rd(void){	
	// �����������Ϊ0 �򲻿ɴ��
	if(unavailable == CABINET_TOTAL){
			LCD_Clear(WHITE);
			USART_SendString(UART1_JR6001,JR6001_STORE_FAIL);		// ���ʧ��, �޿��ÿռ�
			Show_Str(0,50,BLACK,WHITE,"���ʧ��,�޿���",16,0);
			delay_ms(1000);
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			return;
	}
	// �����ʼ����
	LCD_Clear(WHITE);
	Show_Str(1,17,BLACK,WHITE,"����������",16,0);
	if(store_password() == false) {
		lcd_menu_Init(LCD_DISPLAY_WEL_menu);
		return;
	}
	
	
	
	
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_INPUT_PHONE);		// �����������ֻ�����
	Show_Str(1,17,BLACK,WHITE,"�������ֻ���:",16,0);
	
	menu_3rd_store(CABINET_TOTAL - unavailable);		// �����������Ϊ0 ��ôreturn

	
	// LCD_ShowNum(1,34,TIM_GetCounter(TIM2),5,16);
}




/* Contact the backend */
void lcd_menu_4th(void){
	u8 temp;
	
	LCD_Clear(BLACK);
	// ��ʾ����Ա����--------------------------------
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	Gui_StrCenter(0,3,WHITE,BLUE,"����Ա�밴9",16,0);
	// --------------------------------
	Show_Str(2,27,WHITE,BLACK,"Emergency Call:",16,0);
	Gui_StrCenter(0,49,WHITE,BLACK,"020-28320001",16,0);

	Gui_StrCenter(0,77,WHITE,BLACK,"e-mail address",16,0);
	Gui_StrCenter(1,94,WHITE,BLACK,"Yoooo@wyuedu.cn",16,0);

	while(1){
		temp = Matrix_Key_Scan2();
		if(temp == key_Exit){
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
		if(temp == 9){			// �������Աģʽ
			LCD_Clear(BLACK);
			// Gui_StrCenter(0,27,WHITE,BLACK,"Administrator password:",16,0);
			Show_Str(2,27,WHITE,BLACK,"����Ա����:",16,0);
			Administrator();			// �������Աģʽ�ж�
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
		
	}
}

void lcd_menu_Exist(bool Num2){

	if(Num2){	
		// LCD_Fill(9,100,70,100,BLUE);			// ����ɫ
		LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
		Show_Str(3,110,BLACK,BLUE,"unavailable: ",16,0);
		LCD_ShowNum_back_blue(101,110,unavailable,1,16);
	}
	else{ 
		// LCD_Fill(9,100,70,100,BLUE);			// ����ɫ
		LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
		Show_Str(3,110,BLACK,BLUE,"available:",16,0);
		LCD_ShowNum_back_blue(94,110, CABINET_TOTAL - unavailable,1,16);
	}
	
}
