#include "data_manager.h"
#include "memory_rewrite_handler.h"
#include <string.h>

#ifdef data_managerLOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr);
#endif

/*----------------------------------------------------------------------------
 * arg:     void
 * return:  void
 * brief:   Инициализация менеджера данных. Инициализация периферии, инициализация значений по умолчанию.
 ----------------------------------------------------------------------------*/
void DataManager_Init()
{
#ifdef data_managerLOAD_DEFAULT_DATA
	DataManager_InitDefaultData();
#endif
	MemoryRewriteHandler_Init();
}

void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	MemoryRewriteHandler_TryWriteBlock(buf, blockSize, blockAdr + data_managerMAIN_TABLE_OFFSET);

	int i;
	uint8_t invertBuf[fram_driverMAX_BLOCK_SIZE]; 	//FIXME Межмодульная зависимость. Убрать
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~((uint8_t*)(buf))[i]; 		//FIXME Абракадабра. Улучшить читаемость
	}
	MemoryRewriteHandler_TryWriteBlock(invertBuf, blockSize, blockAdr + data_managerINVERT_TABLE_OFFSET);
}

void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t straightBuf[fram_driverMAX_BLOCK_SIZE];		//FIXME Межмодульная зависимость. Убрать
	uint8_t invertBuf[fram_driverMAX_BLOCK_SIZE];		//FIXME Межмодульная зависимость. Убрать

	MemoryRewriteHandler_TryReadBlock(straightBuf, blockSize, blockAdr + data_managerMAIN_TABLE_OFFSET);
	MemoryRewriteHandler_TryReadBlock(invertBuf, blockSize, blockAdr + data_managerINVERT_TABLE_OFFSET);

	int i;
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~invertBuf[i];

		if (straightBuf[i] != invertBuf[i])	{
#ifdef data_managerLOAD_DEFAULT_DATA
			prv_LoadDefaultData(buf, blockSize, blockAdr);
#endif
			//DataManager_ErrorHandler(DATA_MANAGER_ERR_CHK);
			return;
		}
	}
	memcpy ((void*)buf, (const void*) straightBuf, blockSize);
	return;
}

#ifdef data_managerLOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t *ptrDefaultData;
	ptrDefaultData = ((uint8_t*)&DataManager_DefaultData) + blockAdr;

	memcpy( (void*)buf, (const void*)ptrDefaultData, blockSize );
}
#endif
