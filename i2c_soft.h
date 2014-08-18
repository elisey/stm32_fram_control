/*
 * author:	Елисей Равнюшкин
 * date:	08.04.2014	19:00:00
 * file:	i2c_soft.h
 * Софтовая реализация I2C
 */

#ifndef I2C_SOFT_H_
#define I2C_SOFT_H_

#include <stdint.h>

#define i2c_softGPIO						GPIOB
#define i2c_softRCC_APB2Periph_GPIO			RCC_APB2Periph_GPIOB
#define i2c_softGPIO_Pin_SDA				GPIO_Pin_0
#define i2c_softGPIO_Pin_SCL				GPIO_Pin_1

#define I2C_DELAY_VALUE						10

void i2c_soft_Init();
int i2c_soft_ReadBuffer(uint8_t chipAddress, void *buffer, uint32_t sizeOfBuffer);
int i2c_soft_WriteBuffer(uint8_t chipAddress, void *buffer, uint32_t sizeOfBuffer);

#endif
