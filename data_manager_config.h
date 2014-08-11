#pragma once

#include "stdint.h"

typedef struct  {
    uint8_t data8;
    float dataFloat;
    uint32_t data32;
}DataManager_Data_t;

void DataManager_InitDefaultData();
void DataManager_ErrorHandler(int error);
