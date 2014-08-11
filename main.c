#include "stm32f10x.h"
#include "fram_driver.h"
#include "data_manager.h"

void DataManager_ErrorHandler(int error);

int main ()
{
	DataManager_Init();
	Fram_Memset(0xff);

	volatile uint32_t rdErr = 0;
	volatile uint32_t wrErr = 0;

	while ( 1 ) {
		// Попытка загрузить данные из очищенной памяти.
			DataManager_Load(data8);
			DataManager_Load(dataFloat);
			DataManager_Load(data32);

			// Запись данных из локальных переменных
		    uint8_t _data8 = 2;
		    float _dataFloat = 4;
		    uint32_t _data32 = 5;

			DataManager_Write(&_data8, data8);
			DataManager_Write(&_dataFloat, dataFloat);
			DataManager_Write(&_data32, data32);

			_data8 = 0;
			_dataFloat = 0;
			_data32 = 0;

			// Чтение данных в локальные переменные
			DataManager_Read(&_data8, data8);
			DataManager_Read(&_dataFloat, dataFloat);
			DataManager_Read(&_data32, data32);

			// копирование данных в структуру данных
			DataManager_Data.data8 = _data8 + 8;
			DataManager_Data.data32 = _data32 + 32;
			DataManager_Data.dataFloat = _dataFloat + 0.56f;

			// запись данных из структуры данных
			DataManager_Store(data8);
			DataManager_Store(dataFloat);
			DataManager_Store(data32);

			// обнуление структуры данных
			DataManager_Data.data8 = 0;
			DataManager_Data.data32 = 0;
			DataManager_Data.dataFloat = 0;

			// загрузка данных в структуру данных
			DataManager_Load(data8);
			DataManager_Load(dataFloat);
			DataManager_Load(data32);


			rdErr = DataManager_GetReadErrors();
			wrErr = DataManager_GetWriteErrors();
			if ( (rdErr != 0) || (wrErr != 0) )	{
				DataManager_ReserErrors();
			}
	}
}



void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}
