#include "stm32f10x.h"
#include "fram_driver.h"
#include  "data_manager.h"

void DataManager_ErrorHandler(int error);

int main ()
{
	Fram_Init();				// инициализаци¤ модуля

	uint32_t qdata32 = 54;
	float qfloatData = 6.434f;
	uint8_t qdata8 = 150;

	DataManager_Write(&qdata32, data32);
	DataManager_Write(&qfloatData, floatData);
	DataManager_Write(&qdata8, data1);

	qdata32 = 0;
	qfloatData = 0;
	qdata8 = 0;

	DataManager_Read(&qdata32, data32);
	DataManager_Read(&qfloatData, floatData);
	DataManager_Read(&qdata8, data1);

	DataManager_Data.data1 = 45;
	DataManager_Data.data2 = 34;
	DataManager_Data.floatData = 6.43f;
	DataManager_Data.data32 = 45000;

	DataManager_Store(data1);
	DataManager_Store(data2);
	DataManager_Store(floatData);
	DataManager_Store(data32);

	DataManager_Data.data1 = 0;
	DataManager_Data.data2 = 0;
	DataManager_Data.floatData = 0;
	DataManager_Data.data32 = 0;

	DataManager_Load(data1);
	DataManager_Load(data2);
	DataManager_Load(floatData);
	DataManager_Load(data32);

	volatile int result = 0;
	result = Fram_CalculateSize();
	result = Fram_Test();
	Fram_Memset(0xFF);

	float data = 0;				// объ¤вл¤ем переменную и присваиваем ее нулю
	Fram_ReadFloat(&data, 32);	// читаем в эту переменную данные из ¤чейки 32
	data = 56.4343f;
	Fram_WriteFloat(data, 32);	// записываем число 56.4343f в ¤чейку
	data = 0;
	Fram_ReadFloat(&data, 32);	// читаем число и убеждаемс¤ что оно совпадает
	data = 66.4343f;
	Fram_WriteFloat(data, 32);	// пишем другое число, чтоб при следующем запуске
								// программы прочитать его и убедитс¤ что оно
								// сохранилось во врем¤ отключенного питани¤.
	while ( 1 ) {

	}
}

void DataManager_ErrorHandler(int error)
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
	while(1);
}

void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}
