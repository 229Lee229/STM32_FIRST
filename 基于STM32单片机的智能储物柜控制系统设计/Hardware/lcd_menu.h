#ifndef __LCD_MENU_H
#define __LCD_MENU_H
#include "My_include.h"

#define LCD_DISPLAY_Init_loading 	1
#define LCD_DISPLAY_WEL_menu		2
#define  CABINET_TOTAL		5			// ¹ñ×Ó×ÜÊý

void lcd_menu_Init(u8 mode);

void lcd_menu_1st(void);
void lcd_menu_2nd(void);
void lcd_menu_3rd(void);
void lcd_menu_4th(void);
void lcd_menu_Exist(bool Num2);
#endif 
