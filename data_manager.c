#include "data_manager.h"
#include "fram_driver.h"
#include "i2cSoft.h"
#include <string.h>

static bool prv_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);
static bool prv_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);

static void _loadDefaultByte(uint8_t *data, uint16_t offset);
static void _loadDefaultWord(uint32_t *data, uint16_t offset);

/*----------------------------------------------------------------------------
 * arg:     void
 * return:  void
 * brief:   Инициализация менеджера данных. Инициализация периферии, инициализация значений по умолчанию.
 ----------------------------------------------------------------------------*/
void DataManager_Init()
{
    i2cSoft_Init();

    DefaultData.data1 = 1;
    DefaultData.data2 = 3;
}

void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	if (!(prv_TryWriteBlock(buf, blockSize, blockAdr + Fram_MainTable))) {
		//TODO write error handler
		return;
	}

	int i;
	uint8_t invertBuf[MAX_BLOCK_SIZE];
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~((uint8_t*)(buf))[i]; //FIXME
	}
	if (!(prv_TryWriteBlock(invertBuf, blockSize, blockAdr + Fram_InvertTable))) {
		//TODO write error handler
		return;
	}
}

void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t straightBuf[MAX_BLOCK_SIZE];
	uint8_t invertBuf[MAX_BLOCK_SIZE];

	if (!( prv_TryReadBlock(straightBuf, blockSize, blockAdr + Fram_MainTable) ))	{
		//TODO write error handler
		return;
	}
	if (!( prv_TryReadBlock(invertBuf, blockSize, blockAdr + Fram_InvertTable) ))	{
		//TODO write error handler
		return;
	}

	int i;
	for (i = 0; i < blockSize; ++i) {
		invertBuf[i] = ~invertBuf[i];

		if (straightBuf[i] != invertBuf[i])	{
			//TODO read erro handler
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
        result = Fram_WriteBlock(buf,blockSize, blockAdr);
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
        result = Fram_ReadBlock(buf, blockSize, blockAdr);
        if (result)
            return true;
    }
    return false;
}





/*----------------------------------------------------------------------------
 * arg:     uint8_t*, uint16_t
 * return:  void
 * brief:   Загрузка байта по умолчанию из локальнйо копии в оперативке.
 ----------------------------------------------------------------------------*/
static void _loadDefaultByte(uint8_t *data, uint16_t offset)
{
    //*data = *((uint8_t*)(&DefaultData + offset));         //берем значение по умолчанию из локальной копии в СРАМ
    ////EventHandler_Put(eventFramReadError);
}

/*----------------------------------------------------------------------------
 * arg:     uint32_t*, uint16_t
 * return:  void
 * brief:   Загрузка слова по умолчанию из локальнйо копии в оперативке.
 ----------------------------------------------------------------------------*/
static void _loadDefaultWord(uint32_t *data, uint16_t offset)
{
    //*data = *((uint32_t*)(&DefaultData + offset));            //берем значение по умолчанию из локальной копии в СРАМ
    ////EventHandler_Put(eventFramReadError);
}
