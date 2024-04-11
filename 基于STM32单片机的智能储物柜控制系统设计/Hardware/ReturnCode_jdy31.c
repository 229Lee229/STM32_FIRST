#include "stm32f10x.h"                  // Device header
#include "My_include.h"
#include "stdbool.h"

extern u16 unavailable;
bool return_code(uint8_t *Array_jdy,uint8_t *Array_return_code,u16 length){
	
	u8 ArrayRead_Phone[11];
	u8 ArrayRead_Code[5];
	u8 i,j,k;
	
	// ɨ������			
	for(i = 0;i < unavailable;i++){
		W25Q64_ReadData(i << 12,ArrayRead_Phone,11);			// ��ʮһλ�ֻ���
		W25Q64_ReadData(((i<<12) + 0xD),ArrayRead_Code,5);		// ����λ��֤��		

		// ����ƥ����ֻ����Ƿ���ȷ
		for(j = 0;j < 11;j++){
			if(Array_jdy[j] == ArrayRead_Phone[j])continue;
			else	break;		// ����ȷ ��һ�α���
		}
		
// ------------------------------------------------���ҳɹ�		
		if(j == 11){		
			// ����֤�븳�����鷵��
			for(k = 0;k < 5;k++){
				Array_return_code[k] = ArrayRead_Code[k];
			}return true;
// ----------------------------------------------------------	
		
		}
	
	
	}
// ------------------------------------------------ ����ʧ��
	return false;

}
