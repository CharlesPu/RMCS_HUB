#ifndef __RMCS_HUB_CMD_SEND_H__
#define __RMCS_HUB_CMD_SEND_H__

#include "db.h"

class HUB_CMD_Send
{
public:
	HUB_CMD_Send();
	~HUB_CMD_Send();

	static void *CMD_Send(void *);
private:
	// HUB_Mysql *hub_mysql;
};

#endif