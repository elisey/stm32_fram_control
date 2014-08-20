#include "journal.h"
#include "memory_rewrite_handler.h"
#include "memory_allocator.h"

#define journalVALID_BYTE		0xA0

/*----------------------------------------------------------------------------
 * Прототипы локальных функций
 ----------------------------------------------------------------------------*/
void prv_LoadJournalHead (journalSettings_t *this, journalHead_t *ptrJournalHead);
void prv_SaveJournalHead(journalSettings_t *this, const journalHead_t *ptrJournalHead);
void prv_LoadJournalItem (journalSettings_t *this, void *journalItem, uint16_t itemIndex);
void prv_SaveJournalItem(journalSettings_t *this, const void *journalItem, uint16_t itemIndex);
int16_t prv_GetItemIndexByLastItemOffset (journalSettings_t *this, uint16_t itemNumber);
bool prv_CheckJournal(journalSettings_t *this);

/*----------------------------------------------------------------------------
 * Глобальные функции
 ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * brief:	Инициализация журнала this
 ----------------------------------------------------------------------------*/
void Journal_Init (journalSettings_t *this, uint16_t elementSize, uint16_t maxNumOfItems)
{
	this->elementSize = elementSize;
	this->maxNumOfItems = maxNumOfItems;
	this->offset = MemoryAllocator_MemoryAllocate(elementSize * maxNumOfItems + sizeof (journalHead_t));

	if (prv_CheckJournal(this) == false)	{
		Journal_ClearJournal(this);
	}
}

/*----------------------------------------------------------------------------
 * brief:	Взять количество записанных элементов в журнале
 ----------------------------------------------------------------------------*/
uint16_t Journal_GetNumberOfItems (journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);
	return journalHead.numberOfItems;
}

/*----------------------------------------------------------------------------
 * brief:	Прочитать одну запись из журнала. itemNumber - относительное
 * 			смещение относиельно последней записи.
 ----------------------------------------------------------------------------*/
bool Journal_ReadItem (journalSettings_t *this, void *journalItem, uint16_t itemNumber)
{
	int16_t itemIndex = prv_GetItemIndexByLastItemOffset(this, itemNumber);
	if (itemIndex == -1)	{
		return false;
	}
	prv_LoadJournalItem(this, journalItem, itemIndex);

	return true;
}

/*----------------------------------------------------------------------------
 * brief:	Записать запись в журнал в конец
 ----------------------------------------------------------------------------*/
void Journal_WriteItem (journalSettings_t *this, const void *journalItem)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	journalHead.lastItemIndex++;
	if (journalHead.lastItemIndex >= this->maxNumOfItems)	{
		journalHead.lastItemIndex = 0;
	}
	if (journalHead.numberOfItems < this->maxNumOfItems)	{
		journalHead.numberOfItems++;
	}
	prv_SaveJournalItem(this, journalItem, journalHead.lastItemIndex);
	prv_SaveJournalHead(this, &journalHead);
}

/*----------------------------------------------------------------------------
 * brief:	Отчистить журнал
 ----------------------------------------------------------------------------*/
void Journal_ClearJournal (journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	journalHead.lastItemIndex = 0;
	journalHead.numberOfItems = 0;
	journalHead.isValid = journalVALID_BYTE;

	prv_SaveJournalHead(this, &journalHead);
}

/*----------------------------------------------------------------------------
 * Приватные функции
 ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * brief:	Загрузить заголовок журнала
 ----------------------------------------------------------------------------*/
void prv_LoadJournalHead(journalSettings_t *this, journalHead_t *ptrJournalHead)
{
	uint16_t journalOffset;

	journalOffset = this->offset;
	MemoryRewriteHandler_TryReadBlock( (void*)ptrJournalHead, sizeof (journalHead_t), journalOffset);
}

/*----------------------------------------------------------------------------
 * brief:	Сохранить заголовок журнала
 ----------------------------------------------------------------------------*/
void prv_SaveJournalHead(journalSettings_t *this, const journalHead_t *ptrJournalHead)
{
	uint16_t journalOffset;

	journalOffset = this->offset;
	MemoryRewriteHandler_TryWriteBlock( (void*)ptrJournalHead, sizeof (journalHead_t), journalOffset);
}

/*----------------------------------------------------------------------------
 * brief:	Загрузка одной записи журнала с индексом itemIndex
 * 			в буфер journalItem.
 ----------------------------------------------------------------------------*/
void prv_LoadJournalItem(journalSettings_t *this, void *journalItem, uint16_t itemIndex)
{
	uint16_t size = this->elementSize;
	uint16_t adr = this->elementSize * itemIndex + sizeof(journalHead_t);

	MemoryRewriteHandler_TryReadBlock( (void*)journalItem, size, adr );
}

/*----------------------------------------------------------------------------
 * brief:	Сохранить одну запись журнала
 ----------------------------------------------------------------------------*/
void prv_SaveJournalItem(journalSettings_t *this, const void *journalItem, uint16_t itemIndex)
{
	uint16_t size = this->elementSize;
	uint16_t adr = this->elementSize * itemIndex + sizeof(journalHead_t);

	MemoryRewriteHandler_TryWriteBlock( (void*)journalItem, size, adr );
}

/*----------------------------------------------------------------------------
 * brief:	Преобразование индекса записи относительно последней записи в
 * 			абсолютный индекс в кольцевом хранилище журнала. Если
 * 			абсолютный индекс (АИ) больше или равен относительному индексу
 * 			последней записи, то при промотке назад перехода через нуль
 * 			не будет. Иначе будет переход через нуль, а следовательно нужно
 * 			учесть максимальный размер в элементах кольцевого буфера.
 ----------------------------------------------------------------------------*/
int16_t prv_GetItemIndexByLastItemOffset(journalSettings_t *this, uint16_t itemNumber)
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
 * brief:	Проверить журнал на валидность. TRUE - журнал валидный.
 ----------------------------------------------------------------------------*/
bool prv_CheckJournal(journalSettings_t *this)
{
	journalHead_t journalHead;
	prv_LoadJournalHead(this, &journalHead);

	if (journalHead.isValid != journalVALID_BYTE)	{
		return false;
	}
	return true;
}
