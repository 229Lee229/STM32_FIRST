#include "My_include.h"

void Administrator_Select(void){
	u8 temp;
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	Gui_StrCenter(0,3,WHITE,BLUE,"����Աģʽ",16,0);	
	Show_Str(5,30,BLUE,WHITE,"1-Data Reports",16,0);
	Show_Str(5,49,BLUE,WHITE,"2-Clear All Data",14,0);

	while(1){
		temp =  Matrix_Key_Scan2();
		if(temp == 1){
			Generate_data_reports(DISPLAY_DATA);
			USART_SendString(UART1_JR6001,JR6001_DATA_GENERATE);			// �洢����ҽ����
			// ��ʾ����Ļ�� ֱ������Exit
			for(;;){
				temp = Matrix_Key_Scan2();
				if(temp == key_Exit)	break;
			}
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}else if(temp == 2){
			Generate_data_reports(CLEAR_DATA);
			USART_SendString(UART1_JR6001,JR6001_DATA_CLEARED);			// ���������
			delay_ms(1000);
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}else if(temp == key_Exit){
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
	}
}

void Administrator(void){
	u8 temp_input_Administrator[4];			// ��ʱ��Ź���Ա���� ����Ϊ1234
	int8_t temp,index = 0;
	while(1){
		temp =  Matrix_Key_Scan2();
		if(temp >= 0 && temp <= 9){
			temp_input_Administrator[index] = temp;
			index++;
		}
		switch(index){
			// ��ʾÿһλ����
			case 1:	LCD_ShowNum_back_black(1,43,temp_input_Administrator[index - 1],1,16);break;
			case 2:	LCD_ShowNum_back_black(9,43,temp_input_Administrator[index - 1],1,16);break;
			case 3:	LCD_ShowNum_back_black(17,43,temp_input_Administrator[index - 1],1,16);break;
			case 4:	LCD_ShowNum_back_black(25,43,temp_input_Administrator[index - 1],1,16);break;
			default: break;
		}
		if(index == 4) break;
	}
	// �ж��Ƿ�Ϊ����Ա���� 		ԭ��Ϊ1234
	for(u8 i = 1;i <= 4;i++){
		if(temp_input_Administrator[i - 1] == i) index--;
		else break;
		
	}
	// ������ȷ
	if(index == 0){
		LCD_Clear(WHITE);
		Administrator_Select();
//		LCD_Fill(0,0,lcddev.width,20,BLUE);
//		Gui_StrCenter(0,3,WHITE,BLUE,"����Աģʽ",16,0);
//		delay_ms(900);
		return;
	}
	// �������
	else{
		LCD_Clear(WHITE);
		// Gui_StrCenter(0,30,WHITE,BLACK,"PWD ERROR!",16,0);
		Show_Str(20,40,BLACK,WHITE,"�������!",16,0);
		USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);			// 
		delay_ms(900);
		return ;
	}
	
	
	
}

