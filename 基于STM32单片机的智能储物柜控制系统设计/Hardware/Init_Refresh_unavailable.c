#include "My_include.h"

/*****************************************************************************
 * @name       :ˢ��unavailable
 * @date       :2024-04-04 
 * @function   :�ϵ�Ĭ�϶�Flash����ɨ��ˢ��, ������ȡ������, 
				unavailable++, ��ʵ�ֵ��籣������, �Ժ����Ĵ����ǰˢ��unavailable
 * @Author	   :Lee
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
// char PHONE_JUDGE_CODE[1] = "-";				// 1350237XXXX-54541		�����
extern char PHONE_JUDGE_CODE[];
char temp_refresh_unavailable[1];
extern u16 unavailable;
void Init_Refresh_unavailable(void){
	W25Q64_Init();				/* FLASH ��ʼ�� */
	
	/* ��Flash����ɨ�� ɨ���Ƿ����'-'�ַ� ��0������ʼɨ�� */
	// ��16������
	for(u8 j = 0;j < CABINET_TOTAL;j++){
		// ÿ������ɨ��13��   11λ�ֻ���+1λ�ַ�λ
//		for(u8 i = 0;i < 13;i++){
//			W25Q64_ReadData_char((j << 12 )+ (i)/* ����ָ����ַ */,temp_refresh_unavailable,1);
//			if(temp_refresh_unavailable[0] == PHONE_JUDGE_CODE[0]){		// ���ƥ��ɹ���unavailable++
//				unavailable++;
//			}
//		}
		W25Q64_ReadData_char((j << 12) + 0xC,temp_refresh_unavailable,1);
		if(temp_refresh_unavailable[0] == PHONE_JUDGE_CODE[0]) unavailable++;
	}
}
