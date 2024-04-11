#include "MySPI_W25.h"		
#include "My_include.h"

/*****************************************************************************
 * @name       :SPI1 Init Ӳ������
 * @date       :2024-04-03 
 * @function   :���÷���������ģʽ			CS������ͨIO��� ����Ϊ�ض�Ӳ������
 * @Author	   :Lee
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	

/* ת������ */

/* �������� */
void W25_Arrary_Int_clear(u8 *DataArray){
	u16 length = W25_Array_Int_length(DataArray);		// ���㳤��
	u16 i = 0;
	// ����
	while(length){
		DataArray[i] = 0;
		i++;
		length--;
	}
}

/* ��¼CharArray���� */
u16 W25_Array_char_length(char *DataArray){
	u16 i = 0;
	while(DataArray[i] != '\0'){
		i++;
	}
	return i;
}
/* ��¼IntArray���� */
u16 W25_Array_Int_length(u8 *DataArray){
	u16 i = 0;
	while(DataArray[i] != '\0')	i++;
	return i;
}


void MySPI_W_SS(uint8_t BitValue){	 /* CS */
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,(BitAction)BitValue);
}

void MySPI_Init(void){
	/* �������� ����SPIʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1,ENABLE); 
	/* GPIO������ӳ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	/* PA15 PB3 PB4 */	
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);			/* PB5 */


	
	/* CS */
	GPIO_Pin_Init(PA15,GPIO_Mode_Out_PP);	
	
	GPIO_Pin_Init(PB3,GPIO_Mode_AF_PP);	/* CK */ 
	GPIO_Pin_Init(PB5,GPIO_Mode_AF_PP);	/* MOSI */	
	/* MISO */
	GPIO_Pin_Init(PB4,GPIO_Mode_IPU);		

	
	
	
	
	/* ����SPI1 */
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	
	SPI_Cmd(SPI1,ENABLE);
	/* ��ʼ�����Ĭ��״̬ */
	MySPI_W_SS(1);			// Ƭѡδʹ��
	
}


void MySPI_Start(void){
	MySPI_W_SS(0);
}
void MySPI_Stop(void){
	MySPI_W_SS(1);
}


uint8_t MySPI_SwapByte(uint8_t ByteSend){
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	SPI_I2S_SendData(SPI1, ByteSend);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	return SPI_I2S_ReceiveData(SPI1);
}

