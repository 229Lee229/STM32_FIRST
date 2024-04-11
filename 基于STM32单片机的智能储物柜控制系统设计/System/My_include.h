#ifndef __MY_INCLUDE_H
#define __MY_INCLUDE_H

#include "stm32f10x.h"                  // Device header


#include "OLED.h"
#include "Matrix_Key2.h"
#include "MyGPIO.h"
#include "usart.h"
#include "SysBitBand.h"

#include "stdbool.h"
// #include "Delay_1st.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "test.h"
#include "GUI.h"
#include "myiic.h"
#include "SPI_TFT.h"
#include "ExistTIM.h"
#include "lcd_menu.h"
#include "JR6001.h"

#include "SG90PWM.h"
#include "SG90Angle.h"
#include "SG90Start_Zeroing.h"

/*  W25  */
#include "MySPI_W25.h"
#include "W25Q64.h"
#include "W25Q64_Ins.h"
#include "W25_Store_password.h"
#include "Init_Refresh_unavailable.h"
#include "W25_Data.h"


#include "Administrator.h"

/* JDY31 */
#include "ReturnCode_jdy31.h"
#include "serial_jdy31.h"
#endif
