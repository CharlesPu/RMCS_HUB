/*******************************
@@Author     : Charles
@@Date       : 2017-12-15
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

Buffer::Buffer(int buf_max, int cell_max)
{
	buf_max_size = buf_max;
	cell_max_size = cell_max;

	buf_signal = PTHREAD_COND_INITIALIZER;
	pthread_mutex_init(&buf_lock, NULL);	
}

Buffer::~Buffer()
{
	buf_max_size = 0;
	cell_max_size = 0;

	pthread_mutex_destroy(&buf_lock);
}

int Buffer::GetCell(unsigned char &c, int num)// 把queue往c里面读取
{
	return 0;
}
int Buffer::PutCell(unsigned char &c, int num)// 把c往queue里面写入
{
	return 0;
}


Cir_Queue::Cir_Queue(int buf_max, int cell_max):Buffer(buf_max, cell_max)
{
	q_buf = new unsigned char *[buf_max];

	for (int i = 0; i < buf_max; ++i)
	{
		q_buf[i] = new unsigned char[cell_max];
		memset(q_buf[i], 0, cell_max);
	}
	head = 0;
	tail = 0;
}

Cir_Queue::~Cir_Queue()
{
	for (int i = 0; i < buf_max_size; ++i)
	{
		if (q_buf[i] != NULL)
			delete[] q_buf[i];
	}
	delete[] q_buf;
	head = 0;
	tail = 0;
}

int Cir_Queue::GetCell(unsigned char &c, int num)
{
	unsigned char &str = c;

	memset(str, 0, cell_max_size);
	memcpy(str, q_buf[tail], num);
	
	tail = (tail + 1) & (buf_max_size - 1);
	
    return num;	
}

int Cir_Queue::PutCell(unsigned char &c, int num)
{
	unsigned char &str = c;
	
	memset(q_buf[head], 0, cell_max_size);
	memcpy(q_buf[head], str, num);

	head = (head + 1) & (buf_max_size - 1);
    return num;
}
