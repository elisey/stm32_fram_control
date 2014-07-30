#include "data_manager.h"
#include "fram_driver.h"
#include "i2cSoft.h"
#include <string.h>

static bool prv_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);
static bool prv_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);

/*----------------------------------------------------------------------------
 * arg:     void
 * return:  void
 * brief:   Инициализация менеджера данных. Инициализация периферии, инициализация значений по умолчанию.
 ----------------------------------------------------------------------------*/
void DataManager_Init()
{
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
			DataManager_ErrorHandler(DATA_MANAGER_ERR_CHK);
			return;
		}
	}
	memcpy ((void*)buf, (const void*) straightBuf, blockSize);
	return;

}

static bool prv_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr)
{
    uint8_t i;
    bool result;

    for (i = 0; i < NUM_OF_TRYS_TO_LOAD; ++i) {
        result = Memory_WriteBlock(buf,blockSize, blockAdr);
        if (result)
            return true;
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
    }
    return false;
}
