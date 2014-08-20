#include "memory_allocator.h"

static uint16_t allocatedBytesCount = 0;
static const uint16_t memorySize = MemoryAllocatorMEMORY_SIZE;

uint16_t MemoryAllocator_MemoryAllocate(uint16_t wantedSize)
{
	uint16_t memoryIndex;

	if (allocatedBytesCount + wantedSize > memorySize)	{
		MemoryAllocator_Error();
	}

	memoryIndex = allocatedBytesCount;
	allocatedBytesCount += wantedSize;
	return memoryIndex;
}

uint16_t MemoryAllocator_GetFreeSpace()
{
	return (memorySize - allocatedBytesCount);
}

void MemoryAllocator_Error()
{
	while(1)	{

	}
}

