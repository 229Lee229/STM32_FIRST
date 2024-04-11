#include "My_include.h"

/*****************************************************************************
 * @name       :刷新unavailable
 * @date       :2024-04-04 
 * @function   :上电默认对Flash进行扫描刷新, 若存有取件数据, 
				unavailable++, 以实现掉电保存数据, 对后续的存件提前刷新unavailable
 * @Author	   :Lee
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
// char PHONE_JUDGE_CODE[1] = "-";				// 1350237XXXX-54541		区别符
extern char PHONE_JUDGE_CODE[];
char temp_refresh_unavailable[1];
extern u16 unavailable;
void Init_Refresh_unavailable(void){
	W25Q64_Init();				/* FLASH 初始化 */
	
	/* 对Flash进行扫描 扫描是否存在'-'字符 从0扇区开始扫描 */
	// 共16个扇区
	for(u8 j = 0;j < CABINET_TOTAL;j++){
		// 每个扇区扫描13次   11位手机号+1位字符位
//		for(u8 i = 0;i < 13;i++){
//			W25Q64_ReadData_char((j << 12 )+ (i)/* 扇区指定地址 */,temp_refresh_unavailable,1);
//			if(temp_refresh_unavailable[0] == PHONE_JUDGE_CODE[0]){		// 如果匹配成功则unavailable++
//				unavailable++;
//			}
//		}
		W25Q64_ReadData_char((j << 12) + 0xC,temp_refresh_unavailable,1);
		if(temp_refresh_unavailable[0] == PHONE_JUDGE_CODE[0]) unavailable++;
	}
}
