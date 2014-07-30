#pragma once
#include "stddef.h"
#include "stdint.h"
#define NUM_OF_TRYS_TO_LOAD     20

typedef struct  {
    uint8_t data1;
    uint8_t data2;
    float floatData;
    uint32_t data32;
}framData_t;

framData_t DefaultData;

#define Fram_MainTable      0
#define Fram_InvertTable    (Fram_MainTable + sizeof(framData_t))
#define Fram_JournalSpace   (Fram_InvertTable + sizeof(framData_t))

#define SIZE_OF_MEMBER(s,m) ((size_t)sizeof(s.m))

#define DataManager_Store(x)	DataManager_WriteBlock((void*)&DefaultData.x, SIZE_OF_MEMBER(DefaultData, x), offsetof(framData_t,x))
#define DataManager_Load(x)		DataManager_ReadBlock((void*)&DefaultData.x, SIZE_OF_MEMBER(DefaultData, x), offsetof(framData_t,x))

#define DataManaget_Write(s,x)	DataManager_WriteBlock((void*)s, SIZE_OF_MEMBER(DefaultData, x), offsetof(framData_t,x))
#define DataManaget_Read(s,x)	DataManager_ReadBlock((void*)s, SIZE_OF_MEMBER(DefaultData, x), offsetof(framData_t,x))

void DataManager_Init();
void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr);
void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr);


