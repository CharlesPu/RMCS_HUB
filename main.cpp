/*******************************
@@Author     :  Charles
@@Date       :  2018-10-22
@@Mail       :  pu17rui@sina.com
@@Description: 	
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "cir_queue.h"
#include "log.h"
#include "rtu.h"
#include "db.h"
#include "cmd_send.h"
#include "process.h"
#include "mthread.h"
#include "config.h"

int main(int argc, char const *argv[])
{
	LOG_Init(LOG_OUT_PATH, LOG_CONTENT_MAX);
	mysql_library_init(0, NULL, NULL);

	Rtus rtus(RTUS_MAX);
	// rtus.test();
	HUB_Server hub_serv(SRV_PORT, SRV_IP, SRV_CLIENT_MAX, SRV_BUF_MAX);
	struct _thread_args t_args;
	t_args._rtus		= &rtus;
	t_args._hub_server	= &hub_serv;

	HUB_Process hub_process;
	HUB_CMD_Send hub_cmd_send;
	HUB_MThread threads(&t_args, 3);
	threads.CreateThread(SCHED_RR, 99, hub_serv.Receive);
	threads.CreateThread(SCHED_RR, 98, hub_process.Process);
	threads.CreateThread(SCHED_RR, 97, hub_cmd_send.CMD_Send);

	threads.Run();

	mysql_library_end();

	LOG_Free();
	
	return 0;
}


