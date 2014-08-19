#include "memory_allocator.h"

static uint16_t allocatedBytesCount = (size_t) 0;
static const int memorySize = MemoryAllocatorMEMORY_SIZE;

int MemoryAllocator_MemoryAllocate(uint16_t wantedSize)
{
	if (allocatedBytesCount + wantedSize >= memorySize)	{	//FIXME Проверить
		while(1);											//FIXME Добавить обработчик ошибок
		return (-1);
	}

	int memoryIndex = allocatedBytesCount;
	allocatedBytesCount += wantedSize;
	return memoryIndex;
}
