#include <stdint.h>
#include "journal.h"
#include "memory_rewrite_handler.h"

void prv_LoadJournalHead (journalSettings_t *this, journalHead_t *ptrJournalHead);
void prv_SaveJournalHead(journalSettings_t *this, const journalHead_t *ptrJournalHead);
uint16_t prv_GetItemIndexByLastItemOffset (journalSettings_t *this, uint16_t itemNumber);
void prv_LoadJournalItem (journalSettings_t *this, void *journalItem, uint16_t itemIndex);
void prv_SaveJournalItem(journalSettings_t *this, const void *journalItem, uint16_t itemIndex);
int prv_CheckJournal(journalSettings_t *this);

void Journal_Init (journalSettings_t *this, uint16_t elementSize, uint16_t offset, uint16_t maxNumOfItems)
{
	this->elementSize = elementSize;
	this->offset = offset;
	this->maxNumOfItems = maxNumOfItems;

	if (prv_CheckJournal(this) == -1)	{
		Journal_DeleteJournal(this);
	}
}

uint16_t Journal_GetNumberOfItems (journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);
	return journalHead.numberOfItems;
}

int Journal_ReadItem (journalSettings_t *this, void *journalItem, uint16_t itemNumber)
{
	uint16_t itemIndex = prv_GetItemIndexByLastItemOffset(this, itemNumber);
	if (itemIndex == -1)	{
		return -1;
	}
	prv_LoadJournalItem(this, journalItem, itemIndex);

	return 0;
}

void Journal_WriteItem (journalSettings_t *this, void *journalItem)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	journalHead.lastItemIndex++;
	if (journalHead.lastItemIndex >= this->maxNumOfItems)	{
		journalHead.lastItemIndex = 0;
	}
	journalHead.numberOfItems++;
	if (journalHead.numberOfItems < this->maxNumOfItems)	{
		journalHead.numberOfItems++;
	}

	prv_SaveJournalItem(this, journalItem, journalHead.lastItemIndex);
	prv_SaveJournalHead(this, &journalHead);
}

void Journal_DeleteJournal (journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	journalHead.lastItemIndex = 0;
	journalHead.numberOfItems = 0;
	journalHead.isValid = journalVALID_BYTE;

	prv_SaveJournalHead(this, &journalHead);
}

void prv_LoadJournalHead(journalSettings_t *this, journalHead_t *ptrJournalHead)
{
	uint16_t journalOffset;

	journalOffset = this->offset;
	MemoryRewriteHandler_TryReadBlock( (uint8_t*)ptrJournalHead, sizeof (journalHead_t), journalOffset);
}

void prv_SaveJournalHead(journalSettings_t *this, const journalHead_t *ptrJournalHead)
{
	uint16_t journalOffset;

	journalOffset = this->offset;
	MemoryRewriteHandler_TryWriteBlock( (uint8_t*)ptrJournalHead, sizeof (journalHead_t), journalOffset);
}

/*----------------------------------------------------------------------------
 * brief:	Преобразование индекса записи относительно последней записи в
 * 			абсолютный индекс в кольцевом хранилище журнала. Если
 * 			абсолютный индекс (АИ) больше или равен относительному индексу
 * 			последней записи, то при промотке назад перехода через нуль
 * 			не будет. Иначе будет переход через нуль, а следовательно нужно
 * 			учесть максимальный размер в элементах кольцевого буфера.
 ----------------------------------------------------------------------------*/
uint16_t prv_GetItemIndexByLastItemOffset(journalSettings_t *this, uint16_t itemNumber)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	uint16_t lastItemIndex = journalHead.lastItemIndex;
	uint16_t numberOfItems = journalHead.numberOfItems;

	if (itemNumber >= numberOfItems)	{
		return -1; //item not exist
	}
	uint16_t itemIndexRead;
	if (lastItemIndex >= itemNumber)	{	// при обратной перемотке не проходим через нуль
		itemIndexRead = lastItemIndex - itemNumber;
	}
	else {									// при обратной перемотке проходим через нуль
		itemIndexRead = this->maxNumOfItems - (itemNumber - lastItemIndex);
	}
	return itemIndexRead;
}

/*----------------------------------------------------------------------------
 * brief:	Загрузка одной записи журнала с индексом itemIndex
 * 			в буфер journalItem.
 ----------------------------------------------------------------------------*/
void prv_LoadJournalItem(journalSettings_t *this, void *journalItem, uint16_t itemIndex)
{
	uint16_t size = this->elementSize;
	uint16_t adr = this->elementSize * itemIndex;

	MemoryRewriteHandler_TryReadBlock( (uint8_t*)journalItem, size, adr );
}

void prv_SaveJournalItem(journalSettings_t *this, const void *journalItem, uint16_t itemIndex)
{
	uint16_t size = this->elementSize;
	uint16_t adr = this->elementSize * itemIndex;

	MemoryRewriteHandler_TryReadBlock( (uint8_t*)journalItem, size, adr );
}

int prv_CheckJournal(journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	if (journalHead.isValid != journalVALID_BYTE)	{
		return -1;
	}
	return 0;
}
