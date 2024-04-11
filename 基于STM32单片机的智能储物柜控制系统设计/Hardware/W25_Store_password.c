#include "W25_Store_password.h"

//#define ARRAY_PHONE_NUM_MAX		11			// �ֻ������Ĵ洢�� 11λ
//#define CODE_LENGTH_MAX			5			// ��֤����󳤶�Ϊ5λ
// #define PHONE_JUDGE_CODE		(char*)'-'		������ͬ!!!
char PHONE_JUDGE_CODE[1] = "-";				// 1350237XXXX-54541		�����
extern u16 unavailable;
// ��ʱ�洢�ֻ�������
u8 Input_Phone_Num[ARRAY_PHONE_NUM_MAX];
// ��ʱ�洢��֤������
u8 CAPTCHA[CODE_LENGTH_MAX];
u8 CAPTCHA_FLASH_R[CODE_LENGTH_MAX];		// ���ڲ�Flash������  ���û�����Ľ���ƥ��
int8_t Num,temp,Array_index = 0;
// extern u8 Exist_TIM2_limit;
/* �����ֻ��� */

void W25_Input_Phone_Num(bool mode){
	temp = Matrix_Key_Scan2();
	if(mode == STORE){			// �洢�ֻ���
		if(temp != -1 && (temp >= 0) && (temp <= 9)){
			Num = temp;
			Input_Phone_Num[Array_index] = Num;			// ÿ��ȡһ��ֵ ��������
			Array_index++;			// ��������++
		}
	}
	else if(mode == PICKUP){	// �洢�û������֤��Բ���
		if(temp != -1 && (temp >= 0) && (temp <= 9)){
			Num = temp;
			CAPTCHA[Array_index] = Num;			// ÿ��ȡһ��ֵ ��������
			Array_index++;			// ��������++
		}
	}
}

//
//����0 ��ʾ������� ����1 ��ʾ������ȷ ִ����һ���������
bool store_password(void){
	u8 i;
	while(1){

		W25_Input_Phone_Num(STORE);			// ���պ���
		// ����ʮһλ�ֻ���
		switch(Array_index){
			case 1:	LCD_ShowNum(1,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 2:	LCD_ShowNum(9,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 3:	LCD_ShowNum(17,32,Input_Phone_Num[Array_index - 1],1,16);break;
			case 4:	LCD_ShowNum(25,32,Input_Phone_Num[Array_index - 1],1,16);break;
			default: break;
		}
		
		if(Array_index == 4){		// ����4λ��ʼ���볤��
			// �ж��Ƿ���ȷ
			for(i = 1;i <= 4;i++){
				if(Input_Phone_Num[i-1] == i) continue;
				else break;
			}
			Array_index = 0;
			/* д����ٶ��������� */
			W25_Arrary_Int_clear(Input_Phone_Num);
			if(i > 4) return true;
			else{
				USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);
				delay_ms(700);
				return false;
			}
		}
		
		if(temp == key_Exit){			// ����������а���Exit ���˳� ����������  ������temp ��Ϊ���鲻������λ������
			/* ������Exit �����ǰ��������� �ٷ���*/
			W25_Arrary_Int_clear(Input_Phone_Num);
			Array_index = 0;
			return false;
		}

	}
}

void menu_3rd_store(u8 mode){

	if(mode == 0)return;
	srand(TIM_GetCounter(TIM2));		// ���������   ʹ�õ�ǰTIM2�ļ�������ֵ��ΪSeed
	while(1){
		
		W25_Input_Phone_Num(STORE);			// ���պ���
		// ����ʮһλ�ֻ���
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
		
		if(Array_index == ARRAY_PHONE_NUM_MAX){		// �ֻ�����11λ ִ�д洢Flash���� 
			
			/* д��Flash �жϵ�ǰavailable��ֵ ȷ��Ҫ�����������ַ д��֮ǰ�Դ��������в��� */
			W25Q64_SecotErase(unavailable << 12);
			W25Q64_PageProgram((unavailable << 12),Input_Phone_Num,Array_index);
			/* ��������� '-' �ڵ�12λ*/
			W25Q64_PageProgram_char(((unavailable << 12)+(3<<2)),PHONE_JUDGE_CODE,1);
			
/*------------------------------ ���������֤�� ---------------------------------------------*/			
			for(u8 i = 0;i < CODE_LENGTH_MAX;i++){
				CAPTCHA[i] = rand()%10;
			}
			// ��֤��д��Flash �ڵ�13λ��ʼ
			W25Q64_PageProgram(((unavailable << 12) + 0xD),CAPTCHA,CODE_LENGTH_MAX);

/*********************************����д��Flash����*************************************/
			Array_index = 0;
			/* д����ٶ��������� */
			W25_Arrary_Int_clear(Input_Phone_Num);
			delay_ms(500);
			/* �ɹ�д��Flash ������++ */
			unavailable++;
/*********************************��ʾ�û�������֤�� ��ʾ��֤�� **************************/
			Show_Str(1,50,BLACK,WHITE,"����ȡ����Ϊ:",16,0);
			for(u8 i = 0;i < CODE_LENGTH_MAX;i++){
				LCD_ShowNum(1 + (8*i),66,CAPTCHA[i],1,16);
			}
			Show_Str(1,80,BLACK,WHITE,"ȷ���밴Exit",16,0);

			// ��ѭ�� Ϊ�����û���סȡ���� �û�ȷ����ס��Exit 
			for(;;){
				temp = Matrix_Key_Scan2();
				if(temp == key_Exit)		break;
			}
			// ����򿪶����Զ�����µ��Ǹ� ����ûʹ�ù��Ķ��
			SG90_select(SG90_OPEN,unavailable);		// Drive SG90
			// ��ʾ����� ��Close �رչ��� �˳�
			LCD_Clear(WHITE);
			Show_Str(30,50,BLACK,WHITE,"�����...",16,0);
			for(;;){
				temp = Matrix_Key_Scan2();
				if(temp == key_Close){
					SG90_select(SG90_CLOSE,unavailable);		// Drive SG90			unavailableҲ��ʾ��Ӧ�����Ĺ��
					USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// �����ѹر�
					break;
				}
			}
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			break;
		}
		if(temp == key_Exit){			// ����������а���Exit ���˳� ����������  ������temp ��Ϊ���鲻������λ������
			
			/* ������Exit �����ǰ��������� �ٷ���*/
			W25_Arrary_Int_clear(Input_Phone_Num);
			Array_index = 0;
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			// Exist_TIM2_limit = 1;			// ����֤  �Ƿ�Ϊ������� �������������Ʊ���
			break;
		}
	}
}




void menu_1st_pick(void){
	u8 i;
	while(1){
		W25_Input_Phone_Num(PICKUP);			// �û�������֤��
		// ����Ļ����ʾ���ȡ����
		switch(Array_index){
			case 1:	LCD_ShowNum(1,32,CAPTCHA[Array_index - 1],1,16);break;
			case 2:	LCD_ShowNum(9,32,CAPTCHA[Array_index - 1],1,16);break;
			case 3:	LCD_ShowNum(17,32,CAPTCHA[Array_index - 1],1,16);break;
			case 4:	LCD_ShowNum(25,32,CAPTCHA[Array_index - 1],1,16);break;
			case 5:	LCD_ShowNum(33,32,CAPTCHA[Array_index - 1],1,16);break;
		}
		
		
		
/* ---------------- ��ʼ��λ���� -----------------------------------*/
		// ����ĳ��ȵ���Ҫ��ȡ���볤�� ����ƥ��ȷ��
		if(Array_index == CODE_LENGTH_MAX){
			
			// ��16������
			for(u8 j = 0;j < unavailable;j++){

				W25Q64_ReadData((j << 12 )+ 0xD/* ����ָ����ַ+ȡ�����ַ */,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);
				// �Ƚ�
				for(i = 0;i < CODE_LENGTH_MAX;i++){
					if(CAPTCHA[i] == CAPTCHA_FLASH_R[i]){
						continue;
					}else break;
				}
				if(i == CODE_LENGTH_MAX){		// ��i == CODE_LENGTH_MAX ʱ ˵��5λ�Ѿ�ƥ��ɹ�
					// j���ǵ�ǰĿ��������ַ  �����õ�ַ��SG90
					SG90_select(SG90_OPEN,j+1);		// Drive SG90	jҲ��ʾ��Ӧ�����Ĺ��
					Show_Str(30,50,BLACK,WHITE,"ȡ����...",16,0);
					USART_SendString(UART1_JR6001,My_JR6001_select_order(j+1));		// ����������ǰ���� j��ʾ��ǰ�Ĺ���
					// ������ǰȡ�����Ӧ��������ַ
					W25Q64_SecotErase(j << 12);
					
					if(j == unavailable - 1){		// ȡ��ʱ�պ������һ����ַ  ��ô������λ return
						Array_index = 0;
						unavailable--;
						// ��ѭ���ȴ� �û��رչ���
						for(;;){
							temp = Matrix_Key_Scan2();
							if(temp == key_Close){
								SG90_select(SG90_CLOSE,j+1);		// Drive SG90			unavailableҲ��ʾ��Ӧ�����Ĺ��
								USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// �����ѹر�
								break;
							}
						}
						lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
						return;
					}
					
					
					// ȡ���������պ����м�ĵ�ַ  ��ʼ��λ
					for(i = j + 1;i < unavailable;i++){
						// ��������ַ���������ǰ������ʱ����
						W25Q64_SecotErase((i - 1) << 12);
						W25Q64_ReadData(i << 12,Input_Phone_Num,ARRAY_PHONE_NUM_MAX);				// ��������ֻ���
						W25Q64_PageProgram((i - 1) << 12,Input_Phone_Num,ARRAY_PHONE_NUM_MAX);		// ת�浽ǰ��λ�õ��ֻ���
						
						W25Q64_PageProgram_char(((i - 1) << 12) + 0xC,PHONE_JUDGE_CODE,1);					// ת���ַ�'-'
						
						W25Q64_ReadData((i << 12) + 0xD,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);			// ���������֤��
						W25Q64_PageProgram(((i - 1) << 12) + 0xD,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);	// ת�浽ǰ��λ�õ���֤��
						// �ٰѰ��������������ɾ�� !!!!
						W25Q64_SecotErase(i << 12);						
					}
					unavailable--;			// ��λ��ɺ� unavailable--;
					Array_index = 0;
					
					// ��ѭ���ȴ� �û��رչ���
					for(;;){
						temp = Matrix_Key_Scan2();
						if(temp == key_Close){
							SG90_select(SG90_CLOSE,j+1);		// Drive SG90			unavailableҲ��ʾ��Ӧ�����Ĺ��
							USART_SendString(UART1_JR6001,JR6001_SG90_CLOSED);		// �����ѹر�
							break;
						}
					}
					lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
					return;
				}
				else if(j == unavailable - 1){		// ���������һ����û��ƥ�䵽 �������
					Array_index = 0;
					USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);
					lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */
					return;
				}
				
//				else{		// i != CODE_LENGTH_MAX  �����
//					//W25_Arrary_Int_clear();
//					Array_index = 0;
//					USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);		// ����������ǰ���� j��ʾ��ǰ�Ĺ���
//				}
			
			}
		}
		
/**---------------------------����Ϊ��ʼ��λ����  -----------------------------*/
				// ����ڰ��Ĺ����а���Exit
		if(temp == key_Exit){			// ����������а���Exit ���˳� ����������  ������temp ��Ϊ���鲻������λ������
			/* ������Exit �����ǰ��������� �ٷ���*/
			W25_Arrary_Int_clear(CAPTCHA);
			Array_index = 0;
			lcd_menu_Init(LCD_DISPLAY_WEL_menu);
			// Exist_TIM2_limit = 1;			// ����֤  �Ƿ�Ϊ������� �������������Ʊ���
			break;
		}
	}
}
