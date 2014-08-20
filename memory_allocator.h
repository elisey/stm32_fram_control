#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "fram_driver.h"	//fram_driverFRAM_SIZE

#define MemoryAllocatorMEMORY_SIZE		fram_driverFRAM_SIZE

uint16_t MemoryAllocator_MemoryAllocate(uint16_t wantedSize);
uint16_t MemoryAllocator_GetFreeSpace();
void MemoryAllocator_Error();

