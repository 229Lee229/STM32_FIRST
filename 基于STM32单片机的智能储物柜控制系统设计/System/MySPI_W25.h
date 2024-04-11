#ifndef __MYSPI_H
#define __MYSPI_H
#include "My_include.h"

/*****************************************************************************
 * @name       :SPI Init ���ģ��
 * @date       :2024-03-31 
 * @function   :�˳�ʼ��Ϊ�����ʼ�� ����ʹ�ø���
 * @Author	   :Lee
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	

void W25_Arrary_Int_clear(u8 *DataArray);
u16 W25_Array_Int_length(u8 *DataArray);
u16 W25_Array_char_length(char *DataArray);
void MySPI_W_SS(uint8_t BitValue);
void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);


#endif
