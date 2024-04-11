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
			// DrawTestPage("智能储物柜欢迎您");		/* 标题栏 */
			Show_Str(0,0,WHITE,BLUE,"智能储物柜欢迎您",16,0);
			// Show_Str(1,17,BLUE,YELLOW,"Type...                    ",12,0);
			Gui_StrCenter(1,17,BLACK,WHITE,"Type...",12,0);			/* 居中显示 */
			Show_Str(9,30,BLUE,WHITE,"1-取件码取件",16,0);
			Show_Str(9,49,BLUE,WHITE,"2-蓝牙通信取件",16,0);
			Show_Str(9,66,BLUE,WHITE,"3-存件",16,0);
			Show_Str(9,83,BLUE,WHITE,"4-联系后台",16,0);
			// LCD_ShowNum(9,100,12312300,8,16);
			break;
		default:	break;
	}


	
	
}

void lcd_menu_1st(void){
	
	if(unavailable == 0){
		LCD_Clear(WHITE);
		Show_Str(0,50,BLACK,WHITE,"取件失败",16,0);
		Show_Str(0,67,BLACK,WHITE,"无存储数据",16,0);	
		USART_SendString(UART1_JR6001,JR6001_PICK_FAIL); 
		u8 i = 5;
		while(i--)	delay_ms(500);	
		lcd_menu_Init(LCD_DISPLAY_WEL_menu);
		return;
	}
	
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_INPUT_CODE);		/* 请输入您的取件码 */
	Show_Str(0,0,WHITE,BLUE,"智能储物柜欢迎您",16,0);
	Show_Str(1,17,BLACK,WHITE,"请输入取件码",16,0);
	// GUI_DrawFont16(1, 17, BLACK, YELLOW, "Please enter the pickup code",0);
	menu_1st_pick();
}

/* Bluetooth pickup */
void lcd_menu_2nd(void){
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_LINK_BULETOOTH);		// 请连接蓝牙取件 
	Show_Str(9,30,BLUE,WHITE,"请连接蓝牙取件",16,0);
	
	
	while(1){
		if(Matrix_Key_Scan2() == key_Exit){			// 在输入过程中按下Exit 则退出 不保存数据  必须是temp 因为数组不保存两位数数据
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}

	}

}

/* Storing objects*/
void lcd_menu_3rd(void){	
	// 如果可用数量为0 则不可存件
	if(unavailable == CABINET_TOTAL){
			LCD_Clear(WHITE);
			USART_SendString(UART1_JR6001,JR6001_STORE_FAIL);		// 存件失败, 无可用空间
			Show_Str(0,50,BLACK,WHITE,"存件失败,无可用",16,0);
			delay_ms(1000);
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			return;
	}
	// 输入初始密码
	LCD_Clear(WHITE);
	Show_Str(1,17,BLACK,WHITE,"请输入密码",16,0);
	if(store_password() == false) {
		lcd_menu_Init(LCD_DISPLAY_WEL_menu);
		return;
	}
	
	
	
	
	LCD_Clear(WHITE);
	USART_SendString(UART1_JR6001,JR6001_INPUT_PHONE);		// 请输入您的手机号码
	Show_Str(1,17,BLACK,WHITE,"请输入手机号:",16,0);
	
	menu_3rd_store(CABINET_TOTAL - unavailable);		// 如果可用数量为0 那么return

	
	// LCD_ShowNum(1,34,TIM_GetCounter(TIM2),5,16);
}




/* Contact the backend */
void lcd_menu_4th(void){
	u8 temp;
	
	LCD_Clear(BLACK);
	// 提示管理员界面--------------------------------
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	Gui_StrCenter(0,3,WHITE,BLUE,"管理员请按9",16,0);
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
		if(temp == 9){			// 进入管理员模式
			LCD_Clear(BLACK);
			// Gui_StrCenter(0,27,WHITE,BLACK,"Administrator password:",16,0);
			Show_Str(2,27,WHITE,BLACK,"管理员密码:",16,0);
			Administrator();			// 进入管理员模式判断
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
		
	}
}

void lcd_menu_Exist(bool Num2){

	if(Num2){	
		// LCD_Fill(9,100,70,100,BLUE);			// 填充白色
		LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
		Show_Str(3,110,BLACK,BLUE,"unavailable: ",16,0);
		LCD_ShowNum_back_blue(101,110,unavailable,1,16);
	}
	else{ 
		// LCD_Fill(9,100,70,100,BLUE);			// 填充白色
		LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
		Show_Str(3,110,BLACK,BLUE,"available:",16,0);
		LCD_ShowNum_back_blue(94,110, CABINET_TOTAL - unavailable,1,16);
	}
	
}
