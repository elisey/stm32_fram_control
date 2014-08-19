#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct	{
		uint16_t elementSize;
		uint16_t offset;
		uint16_t maxNumOfItems;
} journalSettings_t;

typedef struct	{
		uint8_t isValid;
		uint16_t lastItemIndex;
		uint16_t numberOfItems;
} journalHead_t;

void Journal_Init (journalSettings_t *this, uint16_t elementSize, uint16_t offset, uint16_t maxNumOfItems);
uint16_t Journal_GetNumberOfItems (journalSettings_t *this);
bool Journal_ReadItem (journalSettings_t *this, void *journalItem, uint16_t itemNumber);
void Journal_WriteItem (journalSettings_t *this, const void *journalItem);
void Journal_ClearJournal (journalSettings_t *this);
