#ifndef __W25Q64_H
#define __W25Q64_H

// #define 	 W25_SECTOR_ADDRESS   0x000000

void W25Q64_Init(void);


void W25Q64_ReadID(uint8_t *MID,uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray,uint16_t Count);
void W25Q64_PageProgram_char(uint32_t Address, char *DataArray,uint16_t Count);
void W25Q64_SecotErase(uint32_t Address);
void W25Q64_BlockErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count);
void W25Q64_ReadData_char(uint32_t Address,char *DataArray,uint32_t Count);



#endif
