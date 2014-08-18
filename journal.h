#pragma once


#include <stdint.h>


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

void Journal_Init (journalSettings_t *ptrJournalSettings, uint16_t elementSize, uint16_t offset, uint16_t maxNumOfItems);
uint16_t Journal_GetNumberOfItems (journalSettings_t *ptrJournalSettings);
int Journal_ReadItem (journalSettings_t *ptrJournalSettings, void *journalItem, uint16_t itemNumber);
void Journal_WriteItem (journalSettings_t *ptrJournalSettings, void *journalItem);
void Journal_DeleteJournal (journalSettings_t *ptrJournalSettings);
