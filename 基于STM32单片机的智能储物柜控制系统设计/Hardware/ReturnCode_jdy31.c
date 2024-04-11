#include "stm32f10x.h"                  // Device header
#include "My_include.h"
#include "stdbool.h"

extern u16 unavailable;
bool return_code(uint8_t *Array_jdy,uint8_t *Array_return_code,u16 length){
	
	u8 ArrayRead_Phone[11];
	u8 ArrayRead_Code[5];
	u8 i,j,k;
	
	// 扫描扇区			
	for(i = 0;i < unavailable;i++){
		W25Q64_ReadData(i << 12,ArrayRead_Phone,11);			// 读十一位手机号
		W25Q64_ReadData(((i<<12) + 0xD),ArrayRead_Code,5);		// 读五位验证码		

		// 读完匹配该手机号是否正确
		for(j = 0;j < 11;j++){
			if(Array_jdy[j] == ArrayRead_Phone[j])continue;
			else	break;		// 不正确 下一次遍历
		}
		
// ------------------------------------------------查找成功		
		if(j == 11){		
			// 把验证码赋给数组返回
			for(k = 0;k < 5;k++){
				Array_return_code[k] = ArrayRead_Code[k];
			}return true;
// ----------------------------------------------------------	
		
		}
	
	
	}
// ------------------------------------------------ 查找失败
	return false;

}
