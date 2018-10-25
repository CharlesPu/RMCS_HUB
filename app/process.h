#ifndef __RMCS_HUB_PROCESS_H
#define __RMCS_HUB_PROCESS_H

#include "db.h"

class HUB_Process
{
public:
	HUB_Process();
	~HUB_Process();

	static void *Process(void *args);
private:
	// HUB_Mysql *hub_mysql;
};

#endif