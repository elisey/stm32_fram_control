#include "data_manager_config.h"
#include "data_manager.h"

#ifdef data_managerLOAD_DEFAULT_DATA
void DataManager_InitDefaultData()
{
	DataManager_DefaultData.data8 = 77;
	DataManager_DefaultData.data32 = 45789;
	DataManager_DefaultData.dataFloat = 43.2877f;
}
#endif


