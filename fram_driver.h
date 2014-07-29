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

void Fram_Init();

bool Fram_WriteByte(uint8_t data, uint16_t adr);
bool Fram_WriteDoubleByte(uint16_t data, uint16_t adr);
bool Fram_WriteFourByte(uint32_t data, uint16_t adr);
bool Fram_WriteFloat(float data, uint16_t adr);
bool Fram_WriteDouble(double data, uint16_t adr);

bool Fram_ReadByte(uint8_t *data, uint16_t adr);
bool Fram_ReadDoubleByte(uint16_t *data, uint16_t adr);
bool Fram_ReadFourByte(uint32_t *data, uint16_t adr);
bool Fram_ReadFloat(float *data, uint16_t adr);
bool Fram_ReadDouble(double *data, uint16_t adr);

#endif /* FRAM_DRIVER_H_ */
