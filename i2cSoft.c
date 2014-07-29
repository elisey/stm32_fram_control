/*
 * author:	Елисей Равнюшкин
 * date:	08.04.2014	19:00:00
 * file:	i2cSoft.c
 * Софтовая реализация I2C
 */

#include "i2cSoft.h"
#include <stdbool.h>
#include "stm32f10x.h"
//--------------------Прототипы локальных функций-----------------------------
static void i2cSoft_Delay ();
static bool i2cSoft_Start ();
static void i2cSoft_Stop ();
static void i2cSoft_Ack ();
static void i2cSoft_NoAck ();
static bool i2cSoft_WaitAck ();
static void i2cSoft_PutByte ( uint8_t data );
static uint8_t i2cSoft_GetByte ();

//---------------------Глобальные функции--------------------------------------

/**
 *	@brief	Инициализация модуля i2c, а именно портов ввода/вывода
 *	@param	void
 *	@return	void
 */
void i2cSoft_Init ()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( I2C_RCC_APB2Periph_GPIO, ENABLE );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SCL | GPIO_Pin_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init( I2C_GPIO, &GPIO_InitStructure );
}

/**
 *	@brief	Чтение данных из шины в буфер buffer, размером sizeOfBuffer
 *			у слейва с адресом chipAddress.
 *	@param	uint8_t chipAddress		- адрес подчиненного
 *			uint8_t *buffer			- указатель на буфер, куда класть
 *									  прочитанные данные
 *			uint32_t sizeOfBuffer	- количество байт для чтения
 *	@return	int - результат выполнения фунции:
 *			true в случае успеха
 *			false в случае ошибки
 */
int i2cSoft_ReadBuffer ( uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer )
{
	if ( !i2cSoft_Start() )
		return false;

	i2cSoft_PutByte( chipAddress + 1 );
	if ( !i2cSoft_WaitAck() ) {
		i2cSoft_Stop();
		return false;
	}

	while ( sizeOfBuffer != 0 ) {
		*buffer = i2cSoft_GetByte();

		buffer++;
		sizeOfBuffer--;
		if ( sizeOfBuffer == 0 ) {
			i2cSoft_NoAck();
			break;
		}
		else
			i2cSoft_Ack();
	}
	i2cSoft_Stop();
	return true;
}

/**
 *	@brief	Запись данных в шину из буфера buffer, размером sizeOfBuffer
 *			в слейва с адресом chipAddress.
 *	@param	uint8_t chipAddress		- адрес подчиненного
 *			uint8_t *buffer			- указатель на буфер, откуда читать
 *									  записываемые данные
 *			uint32_t sizeOfBuffer	- количество байт для записи
 *	@return	int - результат выполнения фунции:
 *			true в случае успеха
 *			false в случае ошибки
 */
int i2cSoft_WriteBuffer ( uint8_t chipAddress, uint8_t *buffer, uint32_t sizeOfBuffer )
{
	if ( !i2cSoft_Start() )
		return false;

	i2cSoft_PutByte( chipAddress );
	if ( !i2cSoft_WaitAck() ) {
		i2cSoft_Stop();
		return false;
	}

	while ( sizeOfBuffer != 0 ) {
		i2cSoft_PutByte( *buffer );
		if ( !i2cSoft_WaitAck() ) {
			i2cSoft_Stop();
			return false;
		}

		buffer++;
		sizeOfBuffer--;
	}
	i2cSoft_Stop();
	return true;
}

//---------------------------ЛОКАЛЬНЫЕ ФУНКЦИИ-------------------------------

/**
 *	@brief	Реализация простой задержки
 *	@param	void
 *	@return	void
 */
static void i2cSoft_Delay ()
{
	volatile uint16_t i = I2C_DELAY_VALUE;
	while ( i ) {
		i--;
	}
}

/**
 *	@brief	Отправка последовательности СТАРТ в шину
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static bool i2cSoft_Start ()
{
	SDAH;						// отпустить обе линии, на случай
	SCLH;						// на случай, если они были прижаты
	i2cSoft_Delay();
	if ( !(SDAread) )			// если линия SDA прижата слейвом,
		return false;			// то сформировать старт невозможно, выход с ошибкой
	SDAL;						// прижимаем SDA к земле
	i2cSoft_Delay();
	if ( SDAread )				// если не прижалась, то шина неисправна
		return false;			// выход с ошибкой
	i2cSoft_Delay();
	return true;				// старт успешно сформирован
}

/**
 *	@brief	Отправка последовательности СТОП в шину
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true в случае успеха
 *			false в случае ошибки
 */
static void i2cSoft_Stop ()
{
	SCLL;						// последовательность для формирования Стопа
	i2cSoft_Delay();
	SDAL;
	i2cSoft_Delay();
	SCLH;
	i2cSoft_Delay();
	SDAH;
	i2cSoft_Delay();
}

/**
 *	@brief	Отправка последовательности ACK в шину
 *	@param	void
 *	@return	void
 */
static void i2cSoft_Ack ()
{
	SCLL;
	i2cSoft_Delay();
	SDAL;						// прижимаем линию SDA к земле
	i2cSoft_Delay();
	SCLH;						// и делаем один клик линием SCL
	i2cSoft_Delay();
	SCLL;
	i2cSoft_Delay();
}

/**
 *	@brief	Отправка последовательности NO ACK в шину
 *	@param	void
 *	@return	void
 */
static void i2cSoft_NoAck ()	//
{
	SCLL;
	i2cSoft_Delay();
	SDAH;						// отпускаем линию SDA
	i2cSoft_Delay();
	SCLH;						// и делаем один клик линием SCL
	i2cSoft_Delay();
	SCLL;
	i2cSoft_Delay();
}

/**
 *	@brief	Проверка шины на наличие ACK от слейва
 *	@param	void
 *	@return	bool - результат выполнения функции:
 *			true  - если ACK получен
 *			false - если ACK НЕ получен
 */
static bool i2cSoft_WaitAck ()
{
	SCLL;
	i2cSoft_Delay();
	SDAH;
	i2cSoft_Delay();
	SCLH;						// делаем половину клика линией SCL
	i2cSoft_Delay();
	if ( SDAread ) {			// и проверяем, прижал ли слейв линию SDA
		SCLL;
		return false;
	}
	SCLL;						// завершаем клик линией SCL
	return true;
}

/**
 *	@brief	Отправка одного байта data в шину
 *	@param	uint8_t data - байт данных для отправки
 *	@return	void
 */
static void i2cSoft_PutByte ( uint8_t data )
{
	uint8_t i = 8;				// нужно отправить 8 бит данных
	while ( i-- ) {				// пока не отправили все биты
		SCLL;					// прижимаем линию SCL к земле
		i2cSoft_Delay();
		if ( data & 0x80 )		// и выставляем на линии SDA нужный уровень
			SDAH;
		else
			SDAL;
		data <<= 1;
		i2cSoft_Delay();
		SCLH;					// отпускаем линию SCL
		i2cSoft_Delay();		// после этого слейв сразу же прочитает значение на линии SDA
	}
	SCLL;
}

/**
 *	@brief	Чтение одного байта data из шины
 *	@param	void
 *	@return	uint8_t - прочитанный байт
 */
static uint8_t i2cSoft_GetByte ()
{
	volatile uint8_t i = 8;		// нужно отправить 8 бит данных
	uint8_t data = 0;

	SDAH;						// отпускаем линию SDA. управлять ей будет слейв
	while ( i-- ) {				// пока не получили все биты
		data <<= 1;
		SCLL;					// делаем клик линией SCL
		i2cSoft_Delay();
		SCLH;
		i2cSoft_Delay();
		if ( SDAread ) {		// читаем значение на линии SDA
			data |= 0x01;
		}
	}
	SCLL;
	return data;				// возвращаем прочитанное значение
}
