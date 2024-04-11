#include "JR6001.h"



char* My_JR6001_select_order(u8 order){
	switch(order){
		case 1:	return JR6001_1ST_OPENED;
		case 2:	return JR6001_2ND_OPENED;
		case 3:	return JR6001_3RD_OPENED;
		case 4:	return JR6001_4TH_OPENED;
		case 5:	return JR6001_5TH_OPENED;
		default: break;
	}
	return JR6001_PWD_ERROR;
}

void My_JR6001_SetVolume(u8 level){
	if(level > 20)	USART_SendString(UART1_JR6001, JR6001_VOLUME_MAX);
	else if(level > 10)USART_SendString(UART1_JR6001, JR6001_VOLUME_MID);
	else if(level > 0)USART_SendString(UART1_JR6001, JR6001_VOLUME_MIN);
	
}


void My_JR6001_Init(u8 volume){		// JR6001 ≥ı ºªØ
	My_USART_Init(UART1_JR6001,9600);
	My_JR6001_SetVolume(volume);
	
}




