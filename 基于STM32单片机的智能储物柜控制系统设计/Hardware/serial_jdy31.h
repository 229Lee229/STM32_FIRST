#ifndef __SERIAL_JDY31_H
#define __SERIAL_JDY31_H

#include "my_include.h"
#include "stdio.h"


void Serial_JDY31_Init(void);
void Serial_SendByte(uint16_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint16_t Serial_GetRxData(void);			// 从8位改为16位 4/7


void jdy31_Rx_Data(void);				// 从第一放在后面解决类型不一致比对出错的问题

#endif
