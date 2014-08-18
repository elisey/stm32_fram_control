#include "memory_rewrite_handler.h"

static uint32_t readErrorsCounter = 0;
static uint32_t writeErrorsCounter = 0;



void MemoryRewriteHandler_Init()
{
	Fram_Init();
}

void MemoryRewriteHandler_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr)
{
    uint8_t i;
    bool result;

    for (i = 0; i < NUM_OF_TRYS_TO_LOAD; ++i) {
        result = Memory_WriteBlock(buf,blockSize, blockAdr);
        if (result)
            return;
        writeErrorsCounter++;
    }
    MemoryRewriteHandler_ErrorHandler(memory_rewrite_handlerERR_WR);
}

void MemoryRewriteHandler_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr)
{
	uint8_t i;
    bool result;

    for (i = 0; i < NUM_OF_TRYS_TO_LOAD; ++i) {
        result = Memory_ReadBlock(buf, blockSize, blockAdr);
        if (result)
            return;
        readErrorsCounter++;
    }
    MemoryRewriteHandler_ErrorHandler(memory_rewrite_handlerERR_RD);
}



uint32_t MemoryRewriteHandler_GetReadErrors()
{
	return readErrorsCounter;
}

uint32_t MemoryRewriteHandler_GetWriteErrors()
{
	return writeErrorsCounter;
}

void MemoryRewriteHandler_ReserErrors()
{
	readErrorsCounter = 0;
	writeErrorsCounter = 0;
}


