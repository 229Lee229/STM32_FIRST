#include "serial_jdy31.h"
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>


// ��ʱ�洢��֤������
extern u8 CAPTCHA[];
extern u8 CAPTCHA_FLASH_R[];		// ���ڲ�Flash������  �Խ��յ��Ľ���ƥ��
extern u8 Input_Phone_Num[];
extern char PHONE_JUDGE_CODE[];				// 1350237XXXX-54541		�����
// ---


extern u16 unavailable;


// ��void Serial_SendByte(uint16_t Byte)  ��8λ��Ϊ16λ��  4/7
extern u16 TxData_16[];
extern u8 RxData_8_match[];
extern u8 RxData_8_jdy[];
extern u8 TxData_8_Code[];
extern u8 RxData_8_Code[];
u8 RxState = 0;			// 0 = �����ֻ�����  1 = ������֤��
u8 pRxPacker = 0;		// ָ�����ݰ�������

uint16_t Serial_RxData;			// ���ڽ������ݴ�8λ��Ϊ16λ 4/7
uint8_t Serial_RxFlag;

void Serial_JDY31_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 	T-PB10 R-PB11
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
}

void Serial_SendByte(uint16_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint16_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		jdy31_Rx_Data();			// �����ֻ�������
		// Serial_RxData = USART_ReceiveData(USART3);
		// Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}


// �ѽ��յ������ݴ�һ���ֽڵ���������ת��Ϊһ���ֽ�һ������   
void jdy31_Rx_Data(void){
	u8 TxData_8_Code_NoFind[5] = {0xff,0xff,0xff,0xff,0xff};		// ����û���ҵ�
	u8 RxData = USART_ReceiveData(USART3);
	
	
	if(RxData == 0xFF){		// �����Ƿ��ǽ�����֤�뻹���ֻ���
		RxState = 1;
	}
	else if(RxState == 0){		
		if(pRxPacker == 5){
			RxData_8_jdy[pRxPacker] = RxData;			// ���յ�8λ����  �ŵ�8λ���������� ʹ����λ����
			RxData_8_match[pRxPacker + pRxPacker] = (RxData_8_jdy[pRxPacker] &= 0xf);
			// Serial_SendArray(TxData_8,6);
			pRxPacker = 0;
			
			if(return_code(RxData_8_match,TxData_8_Code,11) == true)			// ƥ����֤��
						Serial_SendArray(TxData_8_Code,5);
			else 		Serial_SendArray(TxData_8_Code_NoFind,5);
			return;
		}
		RxData_8_jdy[pRxPacker] = RxData;			// ���յ�8λ����  �ŵ�8λ���������� ʹ����λ����תΪһ���ֽ�һ������ 
		RxData_8_match[pRxPacker + pRxPacker] = RxData_8_jdy[pRxPacker] >> 4;			// ��λ 0	2
		RxData_8_match[pRxPacker + pRxPacker + 1] = (RxData_8_jdy[pRxPacker] &= 0xf);	// ��λ 1	3
		pRxPacker++;	
	
	}
	else if(RxState == 1){		// ������֤��
		RxData_8_Code[pRxPacker] = RxData;
		pRxPacker++;
		if(pRxPacker == 5){
			pRxPacker = 0;
			RxState = 0;
			
			// -------------------------------------- ƥ�䲿�� ��������
			if(unavailable == 0){		// û�д洢����
				Serial_Printf("Error! No Data Stored!\r\n");
				USART_SendString(UART1_JR6001,JR6001_PICK_FAIL); 
				return;
			}
			
			u8 i,temp;
			// ��16������
			for(u8 j = 0;j < unavailable;j++){

				W25Q64_ReadData((j << 12 )+ 0xD/* ����ָ����ַ+ȡ�����ַ */,CAPTCHA_FLASH_R,CODE_LENGTH_MAX);
				// �Ƚ�
				for(i = 0;i < CODE_LENGTH_MAX;i++){
					if(RxData_8_Code[i] == CAPTCHA_FLASH_R[i]){
						continue;
					}else break;
				}
				if(i == CODE_LENGTH_MAX){		// ��i == CODE_LENGTH_MAX ʱ ˵��5λ�Ѿ�ƥ��ɹ�
					Serial_Printf("Pickup successful!\r\n");
					// j���ǵ�ǰĿ��������ַ  �����õ�ַ��SG90
					SG90_select(SG90_OPEN,j+1);		// Drive SG90	jҲ��ʾ��Ӧ�����Ĺ��
					LCD_Clear(WHITE);
					Show_Str(30,50,BLACK,WHITE,"ȡ����...",16,0);
					USART_SendString(UART1_JR6001,My_JR6001_select_order(j+1));		// ����������ǰ���� j��ʾ��ǰ�Ĺ���
					// ������ǰȡ�����Ӧ��������ַ
					W25Q64_SecotErase(j << 12);
					
					
					
					// #�жϵ�ַλ��
					if(j == unavailable - 1){		// ȡ��ʱ�պ������һ����ַ  ��ô������λ return
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
						lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */ // �жϽ����� ����һ��ʼ��ҳ��,->���������о�4/9
						return;
					}
					
					
					// #�жϵ�ַλ�� ȡ���������պ����м�ĵ�ַ  ��ʼ��λ
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
					Serial_Printf("Error! No Find!\r\n");
					USART_SendString(UART1_JR6001,JR6001_PWD_ERROR);
					lcd_menu_Init(LCD_DISPLAY_WEL_menu);		/* Exit-15 */ 
					return;
				}
			}
			
// ------------------------------------------------------------------------------------------------------------------

			
			
			

		}


	}
}

