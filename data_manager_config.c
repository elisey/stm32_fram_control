#include "data_manager_config.h"
#include "data_manager.h"

#ifdef LOAD_DEFAULT_DATA
void DataManager_InitDefaultData()
{
	DataManager_DefaultData.data8 = 77;
	DataManager_DefaultData.data32 = 45789;
	DataManager_DefaultData.dataFloat = 43.2877f;
}
#endif

void DataManager_ErrorHandler(int error)
{
	switch (error)
	{
	case DATA_MANAGER_ERR_WR:

		break;
	case DATA_MANAGER_ERR_RD:

		break;
	case DATA_MANAGER_ERR_CHK:

		break;
	default:

		break;
	}
	//while(1);
}
