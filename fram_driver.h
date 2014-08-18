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

#define FRAM_ADDRESS		0xA0
#define MAX_BLOCK_SIZE		4
#define FRAM_SIZE			0x2000

void Fram_Init();
int  Fram_Test();
int  Fram_CalculateSize();
void Fram_Memset(uint8_t data);

bool Fram_WriteBlock ( void* buf, size_t blockSize, uint16_t blockAdr );
bool Fram_ReadBlock ( void* buf, size_t blockSize, uint16_t blockAdr );

#endif /* FRAM_DRIVER_H_ */
