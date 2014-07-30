#include "data_manager.h"
#include "fram_driver.h"
#include "i2cSoft.h"
#include <string.h>

uint32_t readErrorsCounter = 0;
uint32_t writeErrorsCounter = 0;

static bool prv_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);
static bool prv_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);
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
    i2cSoft_Init();
}

void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	if (!(prv_TryWriteBlock(buf, blockSize, blockAdr + DataManager_MainTable))) {
		DataManager_ErrorHandler(DATA_MANAGER_ERR_WR);
		return;
	}

	int i;
	uint8_t invertBuf[MAX_BLOCK_SIZE];
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~((uint8_t*)(buf))[i]; //FIXME
	}
	if (!(prv_TryWriteBlock(invertBuf, blockSize, blockAdr + DataManager_InvertTable))) {
		DataManager_ErrorHandler(DATA_MANAGER_ERR_WR);
		return;
	}
}

void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t straightBuf[MAX_BLOCK_SIZE];
	uint8_t invertBuf[MAX_BLOCK_SIZE];

	if (!( prv_TryReadBlock(straightBuf, blockSize, blockAdr + DataManager_MainTable) ))	{
		DataManager_ErrorHandler(DATA_MANAGER_ERR_RD);
		return;
	}
	if (!( prv_TryReadBlock(invertBuf, blockSize, blockAdr + DataManager_InvertTable) ))	{
		DataManager_ErrorHandler(DATA_MANAGER_ERR_RD);
		return;
	}

	int i;
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~invertBuf[i];

		if (straightBuf[i] != invertBuf[i])	{
#ifdef LOAD_DEFAULT_DATA
			prv_LoadDefaultData(buf, blockSize, blockAdr);
#endif
			DataManager_ErrorHandler(DATA_MANAGER_ERR_CHK);
			return;
		}
	}
	memcpy ((void*)buf, (const void*) straightBuf, blockSize);
	return;

}

uint32_t DataManager_GetReadErrors()
{
	return readErrorsCounter;
}

uint32_t DataManager_GetWriteErrors()
{
	return writeErrorsCounter;
}

void DataManager_ReserErrors()
{
	readErrorsCounter = 0;
	writeErrorsCounter = 0;
}

static bool prv_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr)
{
    uint8_t i;
    bool result;

    for (i = 0; i < NUM_OF_TRYS_TO_LOAD; ++i) {
        result = Memory_WriteBlock(buf,blockSize, blockAdr);
        if (result)
            return true;
        writeErrorsCounter++;
    }
    return false;
}

static bool prv_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t i;
    bool result;

    for (i = 0; i < NUM_OF_TRYS_TO_LOAD; ++i) {
        result = Memory_ReadBlock(buf, blockSize, blockAdr);
        if (result)
            return true;
        readErrorsCounter++;
    }
    return false;
}

#ifdef LOAD_DEFAULT_DATA
static void prv_LoadDefaultData(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t *ptrDefaultData;
	ptrDefaultData = ((uint8_t*)&DataManager_DefaultData) + blockAdr;

	memcpy( (void*)buf, (const void*)ptrDefaultData, blockSize );
}
#endif
