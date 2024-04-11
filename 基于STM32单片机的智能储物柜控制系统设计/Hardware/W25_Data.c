#include "My_include.h"

#define SHIFTING	3		// ��ʾλ��ƫ��
extern u16 unavailable;
/* �������ݱ��� ��ʽΪ "�ֻ���-��֤��" */
void Generate_data_reports(u8 mode){
	if(mode == CLEAR_DATA){
		// �����
		LCD_Clear(BLACK);
		Gui_StrCenter(0,43,WHITE,BLACK,"Erasing...",16,0);
		W25Q64_BlockErase(0x000000);
		delay_ms(1000);
		Gui_StrCenter(0,43,WHITE,BLACK,"DATA CLEARED",17,0);
		unavailable = 0;
		return;
	}
	
	// ���ж�Flash�Ƿ�Ϊ�� Ϊ�ռ����������
	if(unavailable == 0){
		LCD_Clear(WHITE);
		Gui_StrCenter(40,43,BLACK,WHITE,"NO DATA",25,0);
		return;
	}
	
	
	uint8_t ArrayRead[15];				// �洢���յ����ֻ���AND��֤��
	char ArrayRead_char[1];				// �洢���յ����ַ�'-'

	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	Gui_StrCenter(0,2,WHITE,BLUE,"Data Reports",16,0);
	
	for(u8 i = 0;i < unavailable;i++){
		// ��ȡ�ֻ���
		W25Q64_ReadData(i << 12,ArrayRead,11);		
		// ����ֻ���
		for(u8 j = 1;j <= 11;j++){	LCD_ShowNum(j + (6*(j-1)) + SHIFTING,32 + (i * 13),ArrayRead[j - 1],1,12);	}	// ��ʾÿһλ�ֻ���	
		// ��ȡ�ַ�'-'
		W25Q64_ReadData_char(((i<<12) + 0xC),ArrayRead_char,1);			
		// ����ַ�'-'
		LCD_ShowChar(78 + SHIFTING,32 + (i * 13),BLACK,WHITE,ArrayRead_char[0],12,0);
		// ��ȡ��֤��
		W25Q64_ReadData(((i<<12) + 0xD),ArrayRead,5);			
		// �����֤��
		for(u8 j = 13;j < 18;j++){	LCD_ShowNum(j + (6*(j-1)) + SHIFTING,32 + (i * 13),ArrayRead[j - 13],1,12);}	// ��ʾÿһλ��֤��
	}
	
	
	// Sector 01  ����
//	W25Q64_ReadData(0x000000,ArrayRead,11);			
//	W25Q64_ReadData_char(0x00000C,ArrayRead_char,1);
//	for(u8 i = 1;i <= 11;i++){
//		LCD_ShowNum(i + (6*(i-1)),32,ArrayRead[i - 1],1,12);		// ��ʾÿһλ�ֻ���	
//	}
//	// LCD_ShowString(89,32,16,ArrayRead_char,0);			// ��ʾ�ַ�'-'
//	//Show_Str(89,32,BLACK,WHITE,ArrayRead_char,14,0);	
//	LCD_ShowChar(78,32,BLACK,WHITE,ArrayRead_char[0],12,0);
//	W25Q64_ReadData(0x00000D,ArrayRead,5);			
//	for(u8 i = 13;i < 18;i++){
//		LCD_ShowNum(i + (6*(i-1)),32,ArrayRead[i - 13],1,12);		// ��ʾÿһλ��֤��
//	}
}




