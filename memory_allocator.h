#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MemoryAllocatorMEMORY_SIZE		0x2000

int MemoryAllocator_MemoryAllocate(uint16_t wantedSize);
