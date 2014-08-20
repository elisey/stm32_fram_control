#include "data_manager.h"
#include "memory_rewrite_handler.h"
#include "memory_allocator.h"
#include <string.h>

static uint16_t mainTableOffset;
static uint16_t invertTableOffset;

#ifdef data_managerLOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr);
#endif
static void prv_AllocateMemory();

/*----------------------------------------------------------------------------
 * brief:	Инициализация менеджера данных. Инициализация периферии,
 * 			инициализация значений по умолчанию.
 ----------------------------------------------------------------------------*/
void DataManager_Init()
{
	DataManager_InitDefaultData();
	MemoryRewriteHandler_Init();
	prv_AllocateMemory();
}

void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	MemoryRewriteHandler_TryWriteBlock(buf, blockSize, blockAdr + mainTableOffset);

	int i;
	uint8_t invertBuf[fram_driverMAX_BLOCK_SIZE];		//FIXME Межмодульная зависимость. Убрать
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~((uint8_t*) (buf))[i]; 			//FIXME Абракадабра. Улучшить читаемость
	}
	MemoryRewriteHandler_TryWriteBlock(invertBuf, blockSize, blockAdr + invertTableOffset);
}

void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t straightBuf[fram_driverMAX_BLOCK_SIZE];		//FIXME Межмодульная зависимость. Убрать
	uint8_t invertBuf[fram_driverMAX_BLOCK_SIZE];		//FIXME Межмодульная зависимость. Убрать

	MemoryRewriteHandler_TryReadBlock(straightBuf, blockSize, blockAdr + mainTableOffset);
	MemoryRewriteHandler_TryReadBlock(invertBuf, blockSize, blockAdr + invertTableOffset);

	int i;
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~invertBuf[i];

		if (straightBuf[i] != invertBuf[i]) {
			#ifdef data_managerLOAD_DEFAULT_DATA
			prv_LoadDefaultData(buf, blockSize, blockAdr);
			#endif
			//DataManager_ErrorHandler(DATA_MANAGER_ERR_CHK);
			return;
		}
	}
	memcpy((void*) buf, (const void*) straightBuf, blockSize);
	return;
}

uint16_t DataManager_GetSizeOfData()
{
	return (sizeof(DataManager_Data_t) * 2);
}

#ifdef data_managerLOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t *ptrDefaultData;
	ptrDefaultData = ((uint8_t*) &DataManager_DefaultData) + blockAdr;

	memcpy((void*) buf, (const void*) ptrDefaultData, blockSize);
}
#endif

static void prv_AllocateMemory()
{
	mainTableOffset = MemoryAllocator_MemoryAllocate(sizeof(DataManager_Data_t) * 2);
	invertTableOffset = mainTableOffset + sizeof(DataManager_Data_t);
}
