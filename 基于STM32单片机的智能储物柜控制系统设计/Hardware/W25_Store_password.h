#ifndef __W25_STORE_PASSWORD_H
#define __W25_STORE_PASSWORD_H


#define ARRAY_PHONE_NUM_MAX		11			// �ֻ������Ĵ洢�� 11λ
#define CODE_LENGTH_MAX			5			// ��֤����󳤶�Ϊ5λ

#define	 	STORE 	1
#define 	PICKUP 	0
#include "My_include.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
bool store_password(void);
void W25_Input_Phone_Num(bool mode);
void menu_3rd_store(u8 mode);
void menu_1st_pick(void);
#endif
