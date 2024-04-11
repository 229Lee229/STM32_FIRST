#include "My_include.h"

#define SHIFTING	3		// 显示位置偏移
extern u16 unavailable;
/* 生成数据报告 格式为 "手机号-验证码" */
void Generate_data_reports(u8 mode){
	if(mode == CLEAR_DATA){
		// 块擦除
		LCD_Clear(BLACK);
		Gui_StrCenter(0,43,WHITE,BLACK,"Erasing...",16,0);
		W25Q64_BlockErase(0x000000);
		delay_ms(1000);
		Gui_StrCenter(0,43,WHITE,BLACK,"DATA CLEARED",17,0);
		unavailable = 0;
		return;
	}
	
	// 先判断Flash是否为空 为空即不输出数据
	if(unavailable == 0){
		LCD_Clear(WHITE);
		Gui_StrCenter(40,43,BLACK,WHITE,"NO DATA",25,0);
		return;
	}
	
	
	uint8_t ArrayRead[15];				// 存储接收到的手机号AND验证码
	char ArrayRead_char[1];				// 存储接收到的字符'-'

	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	Gui_StrCenter(0,2,WHITE,BLUE,"Data Reports",16,0);
	
	for(u8 i = 0;i < unavailable;i++){
		// 读取手机号
		W25Q64_ReadData(i << 12,ArrayRead,11);		
		// 输出手机号
		for(u8 j = 1;j <= 11;j++){	LCD_ShowNum(j + (6*(j-1)) + SHIFTING,32 + (i * 13),ArrayRead[j - 1],1,12);	}	// 显示每一位手机号	
		// 读取字符'-'
		W25Q64_ReadData_char(((i<<12) + 0xC),ArrayRead_char,1);			
		// 输出字符'-'
		LCD_ShowChar(78 + SHIFTING,32 + (i * 13),BLACK,WHITE,ArrayRead_char[0],12,0);
		// 读取验证码
		W25Q64_ReadData(((i<<12) + 0xD),ArrayRead,5);			
		// 输出验证码
		for(u8 j = 13;j < 18;j++){	LCD_ShowNum(j + (6*(j-1)) + SHIFTING,32 + (i * 13),ArrayRead[j - 13],1,12);}	// 显示每一位验证码
	}
	
	
	// Sector 01  例程
//	W25Q64_ReadData(0x000000,ArrayRead,11);			
//	W25Q64_ReadData_char(0x00000C,ArrayRead_char,1);
//	for(u8 i = 1;i <= 11;i++){
//		LCD_ShowNum(i + (6*(i-1)),32,ArrayRead[i - 1],1,12);		// 显示每一位手机号	
//	}
//	// LCD_ShowString(89,32,16,ArrayRead_char,0);			// 显示字符'-'
//	//Show_Str(89,32,BLACK,WHITE,ArrayRead_char,14,0);	
//	LCD_ShowChar(78,32,BLACK,WHITE,ArrayRead_char[0],12,0);
//	W25Q64_ReadData(0x00000D,ArrayRead,5);			
//	for(u8 i = 13;i < 18;i++){
//		LCD_ShowNum(i + (6*(i-1)),32,ArrayRead[i - 13],1,12);		// 显示每一位验证码
//	}
}




