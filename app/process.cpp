/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "process.h"
#include "db.h"
#include "rtu.h"
#include "server.h"
#include "mthread.h"
#include "config.h"
#ifdef OMP_ENABLE
#include <omp.h>
#endif

HUB_Process :: HUB_Process()
{
}

HUB_Process :: ~HUB_Process()
{
}
HUB_Process* HUB_Process :: _instance = NULL;
HUB_Process ::SingletonDestructor HUB_Process ::singleton_destructor;
HUB_Process* HUB_Process::Singleton()
{
	if (_instance == NULL)
	{
		_instance = new HUB_Process;
	}
	return _instance;
}
void *HUB_Process :: Task(void *args)
{
	struct _thread_args *args_tmp = (struct _thread_args*)args;
	Rtus *rtus_tmp 				  = args_tmp->_rtus;
	// HUB_Server *hub_server_tmp    = args_tmp->_hub_server;

	while(1)
	{	
		pthread_mutex_lock(&(rtus_tmp->buf_lock));
		while(!rtus_tmp->IsCQEmpty())
			pthread_cond_wait(&(rtus_tmp->buf_signal), &(rtus_tmp->buf_lock));

		HUB_Mysql hub_mysql(DB_NAME, DB_SERV_NAME, DB_USER_NAME, DB_PASSWORD);
		Rtu *r_tmp = NULL;
		while(rtus_tmp->IsCQEmpty())
		{
			/* get cell and phase */
			rtus_tmp->PrintCell(50);
			unsigned char buf_tmp[128] = {0};
			rtus_tmp->GetCell(r_tmp, buf_tmp, 128);

			int reg_pack_int = r_tmp->reg_pack_int;
			int co_id  = (reg_pack_int >> 4) & 0x0f;
			int sta_id = reg_pack_int & 0x0f;
			printf("%d, %d\n", co_id, sta_id);
			/* parse */

			/* store in mysql */
			

		}
		pthread_mutex_unlock(&(rtus_tmp->buf_lock));				

		usleep(100000);
	}

	return 0;
}