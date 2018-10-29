/*******************************
@@Author     : Charles
@@Date       : 2018-10-24
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include "cmd_send.h"
#include "db.h"
#include "rtu.h"
#include "server.h"
#include "mthread.h"
#include "config.h"

HUB_CMD_Send :: HUB_CMD_Send()
{
}

HUB_CMD_Send :: ~HUB_CMD_Send()
{
}
HUB_CMD_Send* HUB_CMD_Send :: _instance = NULL;
HUB_CMD_Send ::SingletonDestructor HUB_CMD_Send ::singleton_destructor;
HUB_CMD_Send* HUB_CMD_Send::Singleton()
{
	if (_instance == NULL)
	{
		_instance = new HUB_CMD_Send;
	}
	return _instance;
}

void *HUB_CMD_Send :: Task(void *args)
{
	// struct _thread_args *args_tmp = (struct _thread_args*)args;
	// Rtus *rtus_tmp 				  = args_tmp->_rtus;
	// HUB_Server *hub_server_tmp    = args_tmp->_hub_server;

	while (1)
	{
		

		usleep(10000);
	}

	return 0;
}