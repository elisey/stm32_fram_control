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
	Journal_ClearJournal(&testJournal);

	numOfItems = Journal_GetNumberOfItems(&testJournal);
	assert_param(numOfItems == 0);

	result = Journal_ReadItem(&testJournal, (void*)&item, 0);

	assert_param(result == false);

	item.data1 = 33;
	item.data2 = 44;
	item.data3 = 30000000;
	Journal_WriteItem(&testJournal, (const void*)&item);

	item.data1 = 0;
	item.data2 = 0;
	item.data3 = 0;

	result = Journal_ReadItem(&testJournal, (void*)&item, 0);

	assert_param(result == true);
	assert_param(item.data1 == 33);
	assert_param(item.data2 == 44);
	assert_param(item.data3 == 30000000);

	Journal_ClearJournal(&testJournal);

	int i;
	for (i = 0; i < 10; ++i) {
		item.data1 = i * 2 + 1;
		item.data2 = i * 20 + 1;
		item.data3 = i * 200 + 1;
		Journal_WriteItem(&testJournal, (const void*)&item);
	}
	int j = 10;
	for (i = 0; i < 10; ++i) {
		j--;

		result = Journal_ReadItem(&testJournal, (void*)&item, i);
		assert_param(result == true);
		assert_param(item.data1 == j * 2 + 1);
		assert_param(item.data2 == j * 20 + 1);
		assert_param(item.data3 == j * 200 + 1);
	}

	item.data1 = 999;
	item.data2 = 9999;
	item.data3 = 99999;
	Journal_WriteItem(&testJournal, (const void*)&item);

	result = Journal_ReadItem(&testJournal, (void*)&item, 9);
	assert_param(result == true);
	assert_param(item.data1 == 1 * 2 + 1);
	assert_param(item.data2 == 1 * 20 + 1);
	assert_param(item.data3 == 1 * 200 + 1);


}
