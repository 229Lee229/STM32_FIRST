/**********************************************************************
 * flie:  W25Q64	
 * brief: 软件模拟 W25Q64
 * Input: 
 * Output:
 * Retval:
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/31/2024	  Lee				增加数字的读写
	V2.0 		 4/01/2024    Lee				增加对字符型的读写操作
	
 ***********************************************************************/
#include "stm32f10x.h"                  // Device header
#include "My_include.h"


/* 记录当前扇区地址 */

/*****************************************************************************
 * @name       :W23Q64_Init
 * @date       :2024-04-01 
 * @function   :初始化SPI	引脚初始电平  Init CS-High CLK-LOW
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void W25Q64_Init(void){
	MySPI_Init();
}



/*****************************************************************************
 * @name       :W23Q64_ReadID
 * @date       :2024-04-01 
 * @function   :读W25Q63的序列号 以验证读取函数是否正常使用
 * @parameters :*MID  *DID   引用这两个参数 打印值
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
 * @function   :开启写使能  CS-Low 
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
 * @function   :等待忙信号空闲 再返回
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void W25Q64_WaitBusy(void){
	uint32_t TimeOut;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	TimeOut = 100000;										/* 给定超时变量TimeOut 超出规定时间内break 以免产生未知的错误循环 */			
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 1){		
		TimeOut--;
		if(TimeOut == 0)break;
	}
	MySPI_Stop();
}

/*****************************************************************************
 * @name       :W23Q64_PageProgram
 * @date       :2024-04-01 
 * @function   :页编程 输入要写入的整型数据数组
 * @parameters :0x000000 : 为写入目标页地址 共24位寻址
				DataArray: 写入整型数组名称
				Count	 : 写入字节个数
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
 * @function   :页编程 输入要写入的字符型数据数组
 * @parameters :0x000000 : 为写入目标页地址 共24位寻址
				DataArray: 写入字符型数组名称
				Count	 : 写入字节个数
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
 * @function   :扇区擦除
 * @parameters :0x000000 : 指定扇区地址 共24位寻址
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
 * @function   :块擦除
 * @parameters :0x000000 : 指定擦除块地址 共24位寻址
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
 * @function   :读指定页地址数据
 * @parameters :0x000000 : 为目标页地址 共24位寻址
				DataArray: 读到的数据存入该整型数组
				Count	 : 字节个数
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
 * @function   :读字符型数据
 * @parameters :0x000000 : 为目标页地址 共24位寻址
				DataArray: 字符型数组名称
				Count	 : 字节个数
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
