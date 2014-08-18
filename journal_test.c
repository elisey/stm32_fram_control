#include "journal.h"
#include <string.h>
#include "stm32f10x.h"
journalSettings_t testJournal;

typedef struct	{
		uint16_t data1;
		uint16_t data2;
		uint32_t data3;
} myJournal_t;

void Journal_Test()
{
	uint16_t numOfItems;
	bool result;

	myJournal_t item;
	Journal_Init(&testJournal, sizeof (myJournal_t), 0, 10);
	Journal_DeleteJournal(&testJournal);

	numOfItems = Journal_GetNumberOfItems(&testJournal);
	assert_param(numOfItems == 0);

	result = Journal_ReadItem(&testJournal, (void*)&item, 0);

	assert_param(result == false);

	item.data1 = 33;
	item.data2 = 44;
	item.data3 = 30000000;
	result = Journal_WriteItem(&testJournal, (void*)&item);

	assert_param(result == true);

	item.data1 = 0;
	item.data2 = 0;
	item.data3 = 0;

	Journal_ReadItem(&testJournal, (void*)&item, 0);
}
