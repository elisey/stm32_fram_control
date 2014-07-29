/*
 * author:	Елисей Равнюшкин
 * date:	14.04.2014	19:00:00
 * file:	fram_driver.c
 * Драйвер энергонезависимой памяти FRAM
 */

/* Инклуды -------------------------------------------------------------------*/
#include "fram_driver.h"
#include "i2cSoft.h"
#include "stm32f10x_conf.h"
#include <string.h>

/* Прототипы приватных функций -----------------------------------------------*/
static bool prv_WriteBlock ( void* buf, size_t blockSize, uint16_t blockAdr );
static bool prv_ReadBlock ( void* buf, size_t blockSize, uint16_t blockAdr );
static bool prv_SetAddress ( uint16_t adr );


/* Глобайльные функции -------------------------------------------------------*/

/**
 *	@brief	Инициализация внешней энергонезависимой памяти
 *	@param	void
 *	@return	void
 */
void Fram_Init()
{
	i2cSoft_Init();
}

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу одного байта.
 *	@param	uint8_t data - записываемый байт
 *			uint16_t adr - адрес в энергонезависимой памяти
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
bool Fram_WriteByte(uint8_t data, uint16_t adr)
{
	return prv_WriteBlock(&data, sizeof(uint8_t), adr);
}

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу двух байт.
 */
bool Fram_WriteDoubleByte(uint16_t data, uint16_t adr)
{
	return prv_WriteBlock(&data, sizeof(uint16_t), adr);
}

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу четырех байт.
 */
bool Fram_WriteFourByte(uint32_t data, uint16_t adr)
{
	return prv_WriteBlock(&data, sizeof(uint32_t), adr);
}

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу переменной
 *			типа float.
 */
bool Fram_WriteFloat(float data, uint16_t adr)
{
	return prv_WriteBlock(&data, sizeof(float), adr);
}

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу переменной
 *			типа double.
 */
bool Fram_WriteDouble(double data, uint16_t adr)
{
	return prv_WriteBlock(&data, sizeof(double), adr);
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу одного байта.
 *	@param	uint8_t *data - указатель на переменную, в которую будет помещен
 *							результат чтения
 *			uint16_t adr - адрес в энергонезависимой памяти
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
bool Fram_ReadByte(uint8_t *data, uint16_t adr)
{
	return prv_ReadBlock(data, sizeof(uint8_t), adr);
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу двух байт.
 */
bool Fram_ReadDoubleByte(uint16_t *data, uint16_t adr)
{
	return prv_ReadBlock((uint8_t*)data, sizeof(uint16_t), adr);
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу четырех байт.
 */
bool Fram_ReadFourByte(uint32_t *data, uint16_t adr)
{
	return prv_ReadBlock((uint8_t*)data, sizeof(uint32_t), adr);
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу переменной
 *			типа float
 */
bool Fram_ReadFloat(float *data, uint16_t adr)
{
	return prv_ReadBlock((uint8_t*)data, sizeof(float), adr);
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу переменной
 *			типа double
 */
bool Fram_ReadDouble(double *data, uint16_t adr)
{
	return prv_ReadBlock((uint8_t*)data, sizeof(double), adr);
}


/* Приватные функции ---------------------------------------------------------*/

/**
 *	@brief	Запись в энергонезависимую память по указанному адресу
 *			блока данных. Размер блока данных не более 4 байт.
 *	@param	void* buf - указатель на данные
 *			size_t blockSize - размер блока данных в байтах
 *			uint16_t blockAdr - адрес в энергонезависимой памяти, начиная
 *								с которого будет производится запись
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static bool prv_WriteBlock ( void* buf, size_t blockSize, uint16_t blockAdr )
{
	assert_param(blockSize <= MAX_BLOCK_SIZE);			// максимальный размер блока - 4 байта
														// ибо больше не требуется
	bool result;
	uint8_t data[MAX_BLOCK_SIZE + 2];					// буфер на два байта больше.
														// Два байта для адреса ячейки

	memcpy( (void*) data + 2, (void*) buf, blockSize );	// подготавливаем буфер

	data[0] = (uint8_t) (blockAdr >> 8);
	data[1] = (uint8_t) (blockAdr & 0xFF);
														// и отправляем его
	result = i2cSoft_WriteBuffer( FRAM_ADDRESS, data, blockSize + 2 );
	return result;
}

/**
 *	@brief	Чтение из энергонезависимой памяти по указанному адресу
 *			блока данных. Размер блока данных не более 4 байт.
 *	@param	void* buf - указатель на данные
 *			size_t blockSize - размер блока данных в байтах
 *			uint16_t blockAdr - адрес в энергонезависимой памяти, начиная
 *								с которого будет производится чтение
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static bool prv_ReadBlock ( void* buf, size_t blockSize, uint16_t blockAdr )
{
	bool result;
	result = prv_SetAddress( blockAdr );				// сначала устанавливаем адрес чтения
	if ( result == false )
		return false;
	result = i2cSoft_ReadBuffer( FRAM_ADDRESS, buf, blockSize );	// читаем данные
	return result;
}

/**
 *	@brief	Установить указатель на считываемые данные в энергонезависимой памяти
 *	@param	uint16_t blockAdr - адрес в энергонезависимой памяти
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static bool prv_SetAddress ( uint16_t adr )
{
	int result;
	uint8_t buf[2];
	buf[0] = (uint8_t) (adr >> 8);
	buf[1] = (uint8_t) (adr & 0x00FF);
	result = i2cSoft_WriteBuffer( FRAM_ADDRESS, buf, 2 );
	return result;
}
