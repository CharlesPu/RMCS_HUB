/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rtu.h"
#include "log.h"
#include "config.h"

Rtu :: Rtu()
{
	rtu_sock = 0;
	reg_pack_int = 0;
	memset(reg_pack_hex, 0, sizeof(reg_pack_hex));
}

Rtu :: ~Rtu()
{
}

Rtus :: Rtus(int n) : Cir_Queue(CQ_BUF_MAX, CQ_CELL_MAX)
{
	rtu_max_num = n;
	rtu_num 	= 0;

	rtus = new Rtu[rtu_max_num];
	for (int i = 0; i < rtu_max_num; ++i)
	{
		rtus[i].reg_pack_int = i;
		rtus[i].reg_pack_hex[0] = 0x77;
		rtus[i].reg_pack_hex[1] = 0x77;
		rtus[i].reg_pack_hex[2] = 0x77;
		rtus[i].reg_pack_hex[3] = 0x77;
		rtus[i].reg_pack_hex[4] = 0x00;
		rtus[i].reg_pack_hex[5] = 0x00;
		rtus[i].reg_pack_hex[6] = (i >> 4) & 0x0f;
		rtus[i].reg_pack_hex[7] = i & 0x0f;
		// for (int k = 0; k < 8; k++)
		// 	printf("%02x ", rtus[i].reg_pack_hex[k]);
	}
}

Rtus :: ~Rtus()
{
	if (rtus != NULL) {delete[] rtus; rtus = NULL;}
}

// int Rtus :: GetNeedProcessRtus()
// {

// 	vector<Rtu*>().swap(need_proc_rtus); //free 
// 	for (int i = 0; i < rtu_max_num; ++i)
// 	{
// 		pthread_mutex_lock(&(rtus[i].buf_lock));
// 		if (rtus[i].IsCQEmpty())
// 		{
// 			need_proc_rtus.push_back(&(rtus[i]));
// 		}
// 		pthread_mutex_unlock(&(rtus[i].buf_lock));	
// 	}

// 	return need_proc_rtus.size();
// }

void Rtus :: test()
{

	printf("rtus test %s \n",rtus[17].reg_pack_hex);
}
