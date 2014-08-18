#pragma once
#include "fram_driver.h"

#define memory_rewrite_handlerNUM_OF_TRYS_TO_LOAD     3

//	Определение низкоуровневых функций для чтения/записи блоков
//	Прототип функций должен быть:
//	bool your_func ( void* buf, size_t blockSize, uint16_t blockAdr )
#define Memory_WriteBlock			Fram_WriteBlock
#define Memory_ReadBlock			Fram_ReadBlock

//	Обработка ошибок чтения/записи
#define memory_rewrite_handlerERR_WR			(-1)
#define memory_rewrite_handlerERR_RD			(-2)

/* Определите эту функцию для обработки ошибок чтения/записи. */
extern void MemoryRewriteHandler_ErrorHandler(int error);

void MemoryRewriteHandler_Init();
void MemoryRewriteHandler_TryWriteBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);
void MemoryRewriteHandler_TryReadBlock(uint8_t *buf, size_t blockSize, uint16_t blockAdr);

uint32_t MemoryRewriteHandler_GetReadErrors();
uint32_t MemoryRewriteHandler_GetWriteErrors();
void MemoryRewriteHandler_ReserErrors();
