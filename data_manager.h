#pragma once

#include "stddef.h"
#include "stdint.h"
#include "data_manager_config.h"

#define NUM_OF_TRYS_TO_LOAD     3

DataManager_Data_t DataManager_Data;

//	Распределение адресов таблиц в энергонезависимой памяти
#define DataManager_MainTable     	0
#define DataManager_InvertTable    	(DataManager_MainTable + sizeof(DataManager_Data_t))
#define DataManager_EndOfTables   	(DataManager_InvertTable + sizeof(DataManager_Data_t))

//	Определение низкоуровневых функций для чтения/записи блоков
//	Прототип функций должен быть:
//	bool your_func ( void* buf, size_t blockSize, uint16_t blockAdr )
#define Memory_WriteBlock			Fram_WriteBlock
#define Memory_ReadBlock			Fram_ReadBlock

//	Обработка ошибок чтения/записи
#define DATA_MANAGER_ERR_WR			(-1)
#define DATA_MANAGER_ERR_RD			(-2)
#define DATA_MANAGER_ERR_CHK		(-3)

/* You need to implement this function. Example:
 * void DataManager_ErrorHandler(int error)
{
	switch (error)
	{
	case DATA_MANAGER_ERR_WR:
		break;
	case DATA_MANAGER_ERR_RD:
		break;
	case DATA_MANAGER_ERR_CHK:
		break;
	default:
	break;
	}
}
 */
extern void DataManager_ErrorHandler(int error);

#define SIZE_OF_MEMBER(s,m) 		((size_t)sizeof(s.m))

// Макросы, обеспечивающие функциональность данного модуля
#define DataManager_Store(x)		DataManager_WriteBlock((void*)&DataManager_Data.x, SIZE_OF_MEMBER(DataManager_Data, x), offsetof(DataManager_Data_t,x))
#define DataManager_Load(x)			DataManager_ReadBlock((void*)&DataManager_Data.x, SIZE_OF_MEMBER(DataManager_Data, x), offsetof(DataManager_Data_t,x))
#define DataManager_Write(s,x)		DataManager_WriteBlock((void*)s, SIZE_OF_MEMBER(DataManager_Data, x), offsetof(DataManager_Data_t,x))
#define DataManager_Read(s,x)		DataManager_ReadBlock((void*)s, SIZE_OF_MEMBER(DataManager_Data, x), offsetof(DataManager_Data_t,x))

void DataManager_Init();
void DataManager_WriteBlock(void *buf, size_t blockSize, uint16_t blockAdr);
void DataManager_ReadBlock(void *buf, size_t blockSize, uint16_t blockAdr);
