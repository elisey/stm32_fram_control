/*
 * author:	Елисей Равнюшкин
 * date:	14.04.2014	19:00:00
 * file:	fram_driver.h
 * Драйвер энергонезависимой памяти FRAM
 */

#ifndef FRAM_DRIVER_H_
#define FRAM_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define fram_driverBUS_ADDRESS			0xA0			// Адрес чипа на шине
#define fram_driverMAX_BLOCK_SIZE		4				// Максимальный размер блока для чтения/записи
#define fram_driverFRAM_SIZE			0x2000			// Размер в байтах энергонезависимой памяти

void Fram_Init();
bool Fram_WriteBlock ( void* buf, size_t blockSize, uint16_t blockAdr );
bool Fram_ReadBlock ( void* buf, size_t blockSize, uint16_t blockAdr );

// Сервисные функции
int  Fram_Test();
int  Fram_CalculateSize();
void Fram_Memset(uint8_t data);

#endif /* FRAM_DRIVER_H_ */
