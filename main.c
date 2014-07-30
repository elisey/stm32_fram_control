#include "stm32f10x.h"
#include "fram_driver.h"
#include  "data_manager.h"
int main ()
{
	Fram_Init();				// инициализаци¤ модуля

	uint32_t qdata32 = 54;
	float qfloatData = 6.434f;
	uint8_t qdata8 = 150;

	DataManaget_Write(&qdata32, data32);
	DataManaget_Write(&qfloatData, floatData);
	DataManaget_Write(&qdata8, data1);

	qdata32 = 0;
	qfloatData = 0;
	qdata8 = 0;

	DataManaget_Read(&qdata32, data32);
	DataManaget_Read(&qfloatData, floatData);
	DataManaget_Read(&qdata8, data1);

	DefaultData.data1 = 45;
	DefaultData.data2 = 34;
	DefaultData.floatData = 6.43f;
	DefaultData.data32 = 45000;

	DataManager_Store(data1);
	DataManager_Store(data2);
	DataManager_Store(floatData);
	DataManager_Store(data32);

	DefaultData.data1 = 0;
	DefaultData.data2 = 0;
	DefaultData.floatData = 0;
	DefaultData.data32 = 0;

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

void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}
