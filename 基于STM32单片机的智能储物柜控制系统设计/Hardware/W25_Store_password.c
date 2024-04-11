#include "W25_Store_password.h"

//#define ARRAY_PHONE_NUM_MAX		11			// 手机号最大的存储量 11位
//#define CODE_LENGTH_MAX			5			// 验证码最大长度为5位
// #define PHONE_JUDGE_CODE		(char*)'-'		区别异同!!!
char PHONE_JUDGE_CODE[1] = "-";				// 1350237XXXX-54541		区别符
extern u16 unavailable;
// 临时存储手机号数组
u8 Input_Phone_Num[ARRAY_PHONE_NUM_MAX];
// 临时存储验证码数组
u8 CAPTCHA[CODE_LENGTH_MAX];
u8 CAPTCHA_FLASH_R[CODE_LENGTH_MAX];		// 读内部Flash的数据  对用户输入的进行匹配
int8_t Num,temp,Array_index = 0;
// extern u8 Exist_TIM2_limit;
/* 输入手机号 */

void W25_Input_Phone_Num(bool mode){
	temp = Matrix_Key_Scan2();
	if(mode == STORE){			// 存储手机号
		if(temp != -1 && (temp >= 0) && (temp <= 9)){
			Num = temp;
			Input_Phone_Num[Array_index] = Num;			// 每读取一个值 赋给数组
			Array_index++;			// 数组索引++
		}
	}
	else if(mode == PICKUP){	// 存储用户填的验证码对不对
		if(temp != -1 && (temp >= 0) && (temp <= 9)){
			Num = temp;
			CAPTCHA[Array_index] = Num;			// 每读取一个值 赋给数组
			Array_index++;			// 数组索引++
		}
	}
}

//
//返回0 表示密码错误 返回1 表示密码正确 执行下一步存件操作
bool store_password(void){
	u8 i;
	while(1){

		W25_Input_Phone_Num(STORE);			// 接收号码
		// 输入十一位手机号
		switch(Array_index){
			case 1:	LCD_ShowNum(1,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 2:	LCD_ShowNum(9,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 3:	LCD_ShowNum(17,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 4:	LCD_ShowNum(25,32,Input_Phone_Num[Array_index - 1],1,16);break;
			default: break;
		}
		
		if(Array_index == 4){		// 满足4位初始密码长度
			// 判断是否正确
			for(i = 1;i <= 4;i++){
				if(Input_Phone_Num[i-1] == i) continue;
				else break;
			}
			Array_index = 0;
			/* 写入后再对数组清零 */
			W25_Arrary_Int_clear(Input_Phone_Num);
			if(i > 4) return true;
			else{
				USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);
				delay_ms(700);
				return false;
			}
		}
		
		if(temp == key_Exit){			// 在输入过程中按下Exit 则退出 不保存数据  必须是temp 因为数组不保存两位数数据
			/* 若按下Exit 清除当前数组的数据 再返回*/
			W25_Arrary_Int_clear(Input_Phone_Num);
			Array_index = 0;
			return false;
		}

	}
}

void menu_3rd_store(u8 mode){

	if(mode == 0)return;
	srand(TIM_GetCounter(TIM2));		// 随机数种子   使用当前TIM2的计数器的值作为Seed
	while(1){
		
		W25_Input_Phone_Num(STORE);			// 接收号码
		// 输入十一位手机号
		switch(Array_index){
			case 1:	LCD_ShowNum(1,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 2:	LCD_ShowNum(9,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 3:	LCD_ShowNum(17,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 4:	LCD_ShowNum(25,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 5:	LCD_ShowNum(33,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 6:	LCD_ShowNum(41,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 7:	LCD_ShowNum(49,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 8:	LCD_ShowNum(57,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 9:	LCD_ShowNum(65,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 10:LCD_ShowNum(73,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 11:LCD_ShowNum(81,32,Input_Phone_Num[Array_index - 1],1,16);break;		
			default: break;
		}
		
		if(Array_index == ARRAY_PHONE_NUM_MAX){		// 手机号满11位 执行存储Flash操作 
			
			/* 写入Flash 判断当前available的值 确定要移入的扇区地址 写入之前对此扇区进行擦除 */
			W25Q64_SecotErase(unavailable << 12);
			W25Q64_PageProgram((unavailable << 12),Input_Phone_Num,Array_index);
			/* 存入区别符 '-' 在第12位*/
			W25Q64_PageProgram_char(((unavailable << 12)+(3<<2)),PHONE_JUDGE_CODE,1);
			
/*------------------------------ 生成随机验证码 ---------------------------------------------*/			
			for(u8 i = 0;i < CODE_LENGTH_MAX;i++){
				CAPTCHA[i] = rand()%10;
			}
			// 验证码写进Flash 在第13位开始
			W25Q64_PageProgram(((unavailable << 12) + 0xD),CAPTCHA,CODE_LENGTH_MAX);

/*********************************数据写入Flash结束*************************************/
			Array_index = 0;
			/* 写入后再对数组清零 */
			W25_Arrary_Int_clear(Input_Phone_Num);
			delay_ms(500);
			/* 成功写入Flash 不可用++ */
			unavailable++;
/*********************************提示用户保存验证码 显示验证码 **************************/
			Show_Str(1,50,BLACK,WHITE,"您的取件码为:",16,0);
			for(u8 i = 0;i < CODE_LENGTH_MAX;i++){
				LCD_ShowNum(1 + (8*i),66,CAPTCHA[i],1,16);
			}
			Show_Str(1,80,BLACK,WHITE,"确认请按Exit",16,0);

			// 死循环 为了让用户记住取件码 用户确定记住按Exit 
			for(;;){
				temp = Matrix_Key_Scan2();
				if(temp == key_Exit)		break;
			}
			// 存件打开舵机永远是最新的那个 就是没使用过的舵机
			SG90_select(SG90_OPEN,unavailable);		// Drive SG90
			// 显示存件中 按Close 关闭柜门 退出
			LCD_Clear(WHITE);
			Show_Str(30,50,BLACK,WHITE,"存件中...",16,0);
			for(;;){
				temp = Matrix_Key_Scan2();
				if(temp == key_Close){
					SG90_select(SG90_CLOSE,unavailable);		// Drive SG90			unavailable也表示对应存件码的柜机
					USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// 柜门已关闭
					break;
				}
			}
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
		if(temp == key_Exit){			// 在输入过程中按下Exit 则退出 不保存数据  必须是temp 因为数组不保存两位数数据
			
			/* 若按下Exit 清除当前数组的数据 再返回*/
			W25_Arrary_Int_clear(Input_Phone_Num);
			Array_index = 0;
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			// Exist_TIM2_limit = 1;			// 待考证  是否为多余变量 主函数已有限制变量
			break;
		}
	}
}




void menu_1st_pick(void){
	u8 i;
	while(1){
		W25_Input_Phone_Num(PICKUP);			// 用户输入验证码
		// 在屏幕上显示填的取件码
		switch(Array_index){
			case 1:	LCD_ShowNum(1,32,CAPTCHA[Array_index - 1],1,16);break;
			case 2:	LCD_ShowNum(9,32,CAPTCHA[Array_index - 1],1,16);break;
			case 3:	LCD_ShowNum(17,32,CAPTCHA[Array_index - 1],1,16);break;
			case 4:	LCD_ShowNum(25,32,CAPTCHA[Array_index - 1],1,16);break;
			case 5:	LCD_ShowNum(33,32,CAPTCHA[Array_index - 1],1,16);break;
		}
		
		
		
/* ---------------- 开始移位扇区 -----------------------------------*/
		// 输入的长度等于要求取件码长度 进行匹配确认
		if(Array_index == CODE_LENGTH_MAX){
			
			// 共16个扇区
			for(u8 j = 0;j < unavailable;j++){

				W25Q64_ReadData((j << 12 )+ 0xD/* 扇区指定地址+取件码地址 */,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);
				// 比较
				for(i = 0;i < CODE_LENGTH_MAX;i++){
					if(CAPTCHA[i] == CAPTCHA_FLASH_R[i]){
						continue;
					}else break;
				}
				if(i == CODE_LENGTH_MAX){		// 当i == CODE_LENGTH_MAX 时 说明5位已经匹配成功
					// j就是当前目标扇区地址  驱动该地址的SG90
					SG90_select(SG90_OPEN,j+1);		// Drive SG90	j也表示对应存件码的柜机
					Show_Str(30,50,BLACK,WHITE,"取件中...",16,0);
					USART_SendString(UART1_JR6001,My_JR6001_select_order(j+1));		// 语音播报当前柜门 j表示当前的柜门
					// 擦除当前取件码对应的扇区地址
					W25Q64_SecotErase(j << 12);
					
					if(j == unavailable - 1){		// 取件时刚好是最后一个地址  那么不用移位 return
						Array_index = 0;
						unavailable--;
						// 死循环等待 用户关闭柜门
						for(;;){
							temp = Matrix_Key_Scan2();
							if(temp == key_Close){
								SG90_select(SG90_CLOSE,j+1);		// Drive SG90			unavailable也表示对应存件码的柜机
								USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// 柜门已关闭
								break;
							}
						}
						lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
						return;
					}
					
					
					// 取件的扇区刚好在中间的地址  开始移位
					for(i = j + 1;i < unavailable;i++){
						// 该扇区地址后面的数据前移至临时数组
						W25Q64_SecotErase((i - 1) << 12);
						W25Q64_ReadData(i << 12,Input_Phone_Num,ARRAY_PHONE_NUM_MAX);				// 读后面的手机号
						W25Q64_PageProgram((i - 1) << 12,Input_Phone_Num,ARRAY_PHONE_NUM_MAX);		// 转存到前面位置的手机号
						
						W25Q64_PageProgram_char(((i - 1) << 12) + 0xC,PHONE_JUDGE_CODE,1);					// 转存字符'-'
						
						W25Q64_ReadData((i << 12) + 0xD,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);			// 读后面的验证码
						W25Q64_PageProgram(((i - 1) << 12) + 0xD,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);	// 转存到前面位置的验证码
						// 再把搬运完的数据扇区删除 !!!!
						W25Q64_SecotErase(i << 12);						
					}
					unavailable--;			// 移位完成后 unavailable--;
					Array_index = 0;
					
					// 死循环等待 用户关闭柜门
					for(;;){
						temp = Matrix_Key_Scan2();
						if(temp == key_Close){
							SG90_select(SG90_CLOSE,j+1);		// Drive SG90			unavailable也表示对应存件码的柜机
							USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// 柜门已关闭
							break;
						}
					}
					lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
					return;
				}
				else if(j == unavailable - 1){		// 遍历到最后一个还没有匹配到 密码错误
					Array_index = 0;
					USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);
					lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
					return;
				}
				
//				else{		// i != CODE_LENGTH_MAX  的情况
//					//W25_Arrary_Int_clear();
//					Array_index = 0;
//					USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);		// 语音播报当前柜门 j表示当前的柜门
//				}
			
			}
		}
		
/**---------------------------以上为开始移位扇区  -----------------------------*/
				// 如果在按的过程中按下Exit
		if(temp == key_Exit){			// 在输入过程中按下Exit 则退出 不保存数据  必须是temp 因为数组不保存两位数数据
			/* 若按下Exit 清除当前数组的数据 再返回*/
			W25_Arrary_Int_clear(CAPTCHA);
			Array_index = 0;
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			// Exist_TIM2_limit = 1;			// 待考证  是否为多余变量 主函数已有限制变量
			break;
		}
	}
}
