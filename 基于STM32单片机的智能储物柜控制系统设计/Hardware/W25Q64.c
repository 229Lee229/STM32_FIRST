/**********************************************************************
 * flie:  W25Q64	
 * brief: ���ģ�� W25Q64
 * Input: 
 * Output:
 * Retval:
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/31/2024	  Lee				�������ֵĶ�д
	V2.0 		 4/01/2024    Lee				���Ӷ��ַ��͵Ķ�д����
	
 ***********************************************************************/
#include "stm32f10x.h"                  // Device header
#include "My_include.h"


/* ��¼��ǰ������ַ */

/*****************************************************************************
 * @name       :W23Q64_Init
 * @date       :2024-04-01 
 * @function   :��ʼ��SPI	���ų�ʼ��ƽ  Init CS-High CLK-LOW
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void W25Q64_Init(void){
	MySPI_Init();
}



/*****************************************************************************
 * @name       :W23Q64_ReadID
 * @date       :2024-04-01 
 * @function   :��W25Q63�����к� ����֤��ȡ�����Ƿ�����ʹ��
 * @parameters :*MID  *DID   �������������� ��ӡֵ
 * @retvalue   :None
******************************************************************************/
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID){
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
}


/*****************************************************************************
 * @name       :W23Q64_WriteEnable
 * @date       :2024-04-01 
 * @function   :����дʹ��  CS-Low 
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void W25Q64_WriteEnable(void){
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}


/*****************************************************************************
 * @name       :W23Q64_WaitBusy
 * @date       :2024-04-01 
 * @function   :�ȴ�æ�źſ��� �ٷ���
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void W25Q64_WaitBusy(void){
	uint32_t TimeOut;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	TimeOut = 100000;										/* ������ʱ����TimeOut �����涨ʱ����break �������δ֪�Ĵ���ѭ�� */			
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 1){		
		TimeOut--;
		if(TimeOut == 0)break;
	}
	MySPI_Stop();
}

/*****************************************************************************
 * @name       :W23Q64_PageProgram
 * @date       :2024-04-01 
 * @function   :ҳ��� ����Ҫд���������������
 * @parameters :0x000000 : Ϊд��Ŀ��ҳ��ַ ��24λѰַ
				DataArray: д��������������
				Count	 : д���ֽڸ���
 * @retvalue   :None
******************************************************************************/
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray,uint16_t Count){
	uint16_t i;
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0;i < Count;i++) MySPI_SwapByte(DataArray[i]);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

/*****************************************************************************
 * @name       :W23Q64_PageProgram_char
 * @date       :2024-04-01 
 * @function   :ҳ��� ����Ҫд����ַ�����������
 * @parameters :0x000000 : Ϊд��Ŀ��ҳ��ַ ��24λѰַ
				DataArray: д���ַ�����������
				Count	 : д���ֽڸ���
 * @retvalue   :None
******************************************************************************/
void W25Q64_PageProgram_char(uint32_t Address, char *DataArray,uint16_t Count){
	uint16_t i;
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0;i < Count;i++) MySPI_SwapByte(DataArray[i]);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

/*****************************************************************************
 * @name       :W23Q64_SectorErase
 * @date       :2024-04-01 
 * @function   :��������
 * @parameters :0x000000 : ָ��������ַ ��24λѰַ
 * @retvalue   :None
******************************************************************************/
void W25Q64_SecotErase(uint32_t Address){
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

/*****************************************************************************
 * @name       :W23Q64_BlockErase
 * @date       :2024-04-01 
 * @function   :�����
 * @parameters :0x000000 : ָ���������ַ ��24λѰַ
 * @retvalue   :None
******************************************************************************/
void W25Q64_BlockErase(uint32_t Address){
	W25Q64_WriteEnable();
	MySPI_Start();
	MySPI_SwapByte(W25Q64_BLOCK_ERASE_64KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	W25Q64_WaitBusy();
}

/*****************************************************************************
 * @name       :W23Q64_ReadData
 * @date       :2024-04-01 
 * @function   :��ָ��ҳ��ַ����
 * @parameters :0x000000 : ΪĿ��ҳ��ַ ��24λѰַ
				DataArray: ���������ݴ������������
				Count	 : �ֽڸ���
 * @retvalue   :None
******************************************************************************/
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count){
	uint32_t i;
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);

	for(i = 0;i < Count ;i++){
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}

/*****************************************************************************
 * @name       :W23Q64_ReadData_char
 * @date       :2024-04-01 
 * @function   :���ַ�������
 * @parameters :0x000000 : ΪĿ��ҳ��ַ ��24λѰַ
				DataArray: �ַ�����������
				Count	 : �ֽڸ���
 * @retvalue   :None
******************************************************************************/
void W25Q64_ReadData_char(uint32_t Address,char *DataArray,uint32_t Count){
	uint32_t i;
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);

	for(i = 0;i < Count ;i++){
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}
