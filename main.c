#include "stm32f10x.h"
#include "fram_driver.h"
#include "data_manager.h"
#include "memory_rewrite_handler.h"
#include "journal.h"
void DataManager_ErrorHandler(int error);


extern void Journal_Test();
extern void DataManager_Test();

int main ()
{
	DataManager_Init();
	Fram_Memset(0xff);

	Journal_Test();
	DataManager_Test();

	while ( 1 ) {

	}
}

void MemoryRewriteHandler_ErrorHandler(int error)
{
	switch (error)
	{
	case memory_rewrite_handlerERR_WR:

		break;
	case memory_rewrite_handlerERR_RD:

		break;
	default:

		break;
	}
}

void assert_failed(uint8_t* file, uint32_t line)
{
	while(1);
}
