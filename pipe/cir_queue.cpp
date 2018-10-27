/*******************************
@@Author     : Charles
@@Date       : 2017-12-15
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "cir_queue.h"
#include "rtu.h"
#include "log.h"
#include "config.h"

Cir_Queue::Cir_Queue()
{
}

Cir_Queue::Cir_Queue(int buf_max, int cell_max)
{
	buf_max_size = buf_max;
	cell_max_size = cell_max;

	buf_signal = PTHREAD_COND_INITIALIZER;
	pthread_mutex_init(&buf_lock, NULL);

	cq = new struct _cell[buf_max];
	for (int i = 0; i < buf_max; ++i)
	{
		cq[i].r = NULL;
		cq[i].q_buf = new unsigned char[cell_max];
		memset(cq[i].q_buf, 0, cell_max * sizeof(unsigned char));
	}
	head = 0;
	tail = 0;
}

Cir_Queue::~Cir_Queue()
{
	for (int i = 0; i < buf_max_size; ++i)
	{
		cq[i].r = NULL;
		if (cq[i].q_buf != NULL)
			{delete[] cq[i].q_buf; cq[i].q_buf = NULL;}
	}
	if (cq != NULL) {delete[] cq; cq = NULL;}
	head = 0;
	tail = 0;

	buf_max_size = 0;
	cell_max_size = 0;

	pthread_mutex_destroy(&buf_lock);
}

int Cir_Queue :: IsCQEmpty()
{
	// printf("+++++++++++++++++++++++++++++++++++%d, %d\n", head, tail);
	if (head == tail)
		return 0;
	else
		return 1;
}
/*
	the real size of circular queue is (buf_max_size - 1), cause 'head' is pointed to next cell
	where can be write, so this cell must be empty. When full, 'head' is next to 'tail' and 
	'head' cell is empty, so the real size is (buf_max_size - 1).
*/
int Cir_Queue :: IsCQFull()
{
	unsigned int left_num = (int)(tail - head - 1) & (buf_max_size - 1);
	// printf("------%d---%d-----%d------%d\n", tail, head, buf_max_size,left_num);
	if (!left_num)
		return 0;
	else
		return 1;
}
void Cir_Queue :: PrintCell()
{
	int cell_sum = (head - tail) & (buf_max_size - 1);
	for (int i = 0; i < cell_sum; ++i)
	{
		int idx_tmp = (tail + i) & (buf_max_size - 1);
		LOG_DEBUG("cell %d, rtu ", idx_tmp);
		for (int k = 0; k < 8; k++)
			printf("%02x ", cq[idx_tmp].r->reg_pack_hex[k]);
		printf("\n");
		for (int j = 0; j < cell_max_size; ++j)
			printf("(%d)%02x ", j, cq[idx_tmp].q_buf[j]);
		printf("\n");
	}
}
void Cir_Queue :: PrintCell(int n)
{
	int cell_sum = (head - tail) & (buf_max_size - 1);
	for (int i = 0; i < cell_sum; ++i)
	{
		int idx_tmp = (tail + i) & (buf_max_size - 1);
		LOG_DEBUG("cell %d, rtu ", idx_tmp);
		for (int k = 0; k < 8; k++)
			printf("%02x ", cq[idx_tmp].r->reg_pack_hex[k]);
		printf("\n");
		for (int j = 0; j < n; ++j)
			printf("(%d)%02x ", j, cq[idx_tmp].q_buf[j]);
		printf("\n");
	}
}
void Cir_Queue::cq_test()
{
	printf("cir_queue test\n");
}
int Cir_Queue::GetCell(Rtu *&_r, unsigned char *c, int num)
{
	unsigned char *str = c;

	_r = cq[tail].r;
	memset(str, 0, cell_max_size);
	memcpy(str, cq[tail].q_buf, num);
#ifdef PRINT_CQ_CELL
	LOG_DEBUG("GetCell: rtu ");
	for (int k = 0; k < 8; k++)
		printf("%02x ", _r->reg_pack_hex[k]);
	printf("\n");
	for (int j = 0; j < num; j++)
		printf("[%d]%02x ", j, str[j]);
	printf("\n");
#endif	
	tail = (tail + 1) & (buf_max_size - 1);

    return num;	
}

int Cir_Queue::PutCell(Rtu *_r, unsigned char *c, int num)
{
	unsigned char *str = c;
#ifdef PRINT_CQ_CELL
	LOG_DEBUG("PutCell: rtu ");
	for (int k = 0; k < 8; k++)
		printf("%02x ", _r->reg_pack_hex[k]);
	printf("\n");
	for (int j = 0; j < num; j++)
		printf("[%d]%02x ", j, str[j]);
	printf("\n");
#endif	
	cq[head].r = _r;
	memset(cq[head].q_buf, 0, cell_max_size);
	memcpy(cq[head].q_buf, str, num);

	head = (head + 1) & (buf_max_size - 1);

    return num;
}
