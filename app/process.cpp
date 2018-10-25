/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include "process.h"
#include "db.h"
#include "rtu.h"
#include "server.h"
#include "mthread.h"
#include "config.h"

HUB_Process :: HUB_Process()
{
	// hub_mysql = new HUB_Mysql(DB_NAME, DB_SERV_NAME, DB_USER_NAME, DB_PASSWORD);
}

HUB_Process :: ~HUB_Process()
{
	// if (hub_mysql != NULL) {delete hub_mysql; hub_mysql = NULL;}
}
void *HUB_Process :: Process(void *args)
{
	struct _thread_args *args_tmp = (struct _thread_args*)args;
	Rtus *rtus_tmp 				  = args_tmp->_rtus;
	// HUB_Server *hub_server_tmp    = args_tmp->_hub_server;

	while(1)
	{	
		int rtus_sum = rtus_tmp->GetNeedProcessRtus();
#pragma omp parallel for 
		for (int i = 0; i < rtus_sum; ++i)
		{
			pthread_mutex_lock(&(rtus_tmp->need_proc_rtus[i]->buf_lock));
			while(!rtus_tmp->need_proc_rtus[i]->IsCQEmpty())
				pthread_cond_wait(&(rtus_tmp->need_proc_rtus[i]->buf_signal), 
								  &(rtus_tmp->need_proc_rtus[i]->buf_lock));

			HUB_Mysql hub_mysql(DB_NAME, DB_SERV_NAME, DB_USER_NAME, DB_PASSWORD);
			// int reg_pack_int = rtus_tmp->need_proc_rtus[i]->reg_pack_int;
			// int co_id  = reg_pack_int >> 4 & 0x0f;
			// int sta_id = reg_pack_int & 0x0f;
			while(rtus_tmp->need_proc_rtus[i]->IsCQEmpty())
			{
				/* get cell and phase */
				// rtus_tmp->need_proc_rtus[i]->GetCell();

			}
			pthread_mutex_unlock(&(rtus_tmp->need_proc_rtus[i]->buf_lock));				
		}
		usleep(10);
	}

	return 0;
}