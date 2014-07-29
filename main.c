#include "stm32f10x.h"
#include "fram_driver.h"

int main ()
{
	Fram_Init();				// инициализаци¤ модул¤
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
