/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rtu.h"
#include "log.h"
#include "config.h"

Rtu :: Rtu(int m, int n) : Cir_Queue(m, n)
{
	rtu_sock = 0;
	reg_pack_int = 0;
	memset(reg_pack_hex, 0, sizeof(reg_pack_hex));
}

Rtu :: ~Rtu()
{
}


Rtus :: Rtus(int n)
{
	rtu_max_num = n;
	rtu_num 	= 0;

	rtus = (Rtu*)malloc(sizeof(Rtu) * rtu_max_num);
	for (int i = 0; i < rtu_max_num; ++i)
	{
		rtus[i] = Rtu(CQ_BUF_MAX, CQ_CELL_MAX);
		rtus[i].reg_pack_int = i;
		sprintf((char*)rtus[i].reg_pack_hex, 
				"7777777700000%x0%x", (i >> 4) & 0x0f, i & 0x0f);
	}
}

Rtus :: ~Rtus()
{
	if (rtus != NULL) {free(rtus); rtus = NULL;}
}

int Rtus :: GetNeedProcessRtus()
{
	vector<Rtu*>().swap(need_proc_rtus); //free 
	for (int i = 0; i < rtu_max_num; ++i)
	{
		if (rtus[i].IsCQEmpty())
		{
			need_proc_rtus.push_back(&(rtus[i]));
		}
	}

	return need_proc_rtus.size();
}

void Rtus :: test()
{

	printf("rtus test %s \n",rtus[17].reg_pack_hex);
}
