#include "data_manager.h"
#include "memory_rewrite_handler.h"
#include <string.h>




#ifdef LOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr);
#endif

/*----------------------------------------------------------------------------
 * arg:     void
 * return:  void
 * brief:   Инициализация менеджера данных. Инициализация периферии, инициализация значений по умолчанию.
 ----------------------------------------------------------------------------*/
void DataManager_Init()
{
#ifdef LOAD_DEFAULT_DATA
	DataManager_InitDefaultData();
#endif
	MemoryRewriteHandler_Init();
}

void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	MemoryRewriteHandler_TryWriteBlock(buf, blockSize, blockAdr + DataManager_MainTable);

	int i;
	uint8_t invertBuf[MAX_BLOCK_SIZE];
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~((uint8_t*)(buf))[i]; //FIXME
	}
	MemoryRewriteHandler_TryWriteBlock(invertBuf, blockSize, blockAdr + DataManager_InvertTable);
}

void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t straightBuf[MAX_BLOCK_SIZE];
	uint8_t invertBuf[MAX_BLOCK_SIZE];

	MemoryRewriteHandler_TryReadBlock(straightBuf, blockSize, blockAdr + DataManager_MainTable);
	MemoryRewriteHandler_TryReadBlock(invertBuf, blockSize, blockAdr + DataManager_InvertTable);

	int i;
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~invertBuf[i];

		if (straightBuf[i] != invertBuf[i])	{
#ifdef LOAD_DEFAULT_DATA
			prv_LoadDefaultData(buf, blockSize, blockAdr);
#endif
			//DataManager_ErrorHandler(DATA_MANAGER_ERR_CHK);
			return;
		}
	}
	memcpy ((void*)buf, (const void*) straightBuf, blockSize);
	return;
}

#ifdef LOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t *ptrDefaultData;
	ptrDefaultData = ((uint8_t*)&DataManager_DefaultData) + blockAdr;

	memcpy( (void*)buf, (const void*)ptrDefaultData, blockSize );
}
#endif
