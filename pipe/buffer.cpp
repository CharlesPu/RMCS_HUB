/*******************************
@@Author     : Charles
@@Date       : 2017-12-15
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>


Buffer::Buffer()
{
    int i=0;
	for(i=0;i<BUF_MAX_SIZE;i++)
	{
     recv_q.q_buf[i]=new unsigned char[FRAME_MAX_SIZE];
	 memset(recv_q.q_buf[i],0,FRAME_MAX_SIZE);
	 
     send_q.q_buf[i]=new unsigned char[FRAME_MAX_SIZE];
	 memset(send_q.q_buf[i],0,FRAME_MAX_SIZE);
	}
	recv_q.head=0;
	recv_q.tail=0;
	send_q.head=0;
	send_q.tail=0;
}

Buffer::~Buffer()
{
	int i=0;
	for(i=0;i<BUF_MAX_SIZE;i++)
	{
		if(recv_q.q_buf[i] != NULL)delete[] recv_q.q_buf[i];
		if(send_q.q_buf[i] != NULL)delete[] send_q.q_buf[i];
	}
	recv_q.head=0;
	recv_q.tail=0;
	send_q.head=0;
	send_q.tail=0;
}

void Buffer::BufferInit()
{	
	recv_q.head=0;
	recv_q.tail=0;
	send_q.head=0;
	send_q.tail=0;
	for(int i=0;i<BUF_MAX_SIZE;i++)
	{
     recv_q.q_buf[i]=new unsigned char[FRAME_MAX_SIZE];
	 memset(recv_q.q_buf[i],0,FRAME_MAX_SIZE);
	 
     send_q.q_buf[i]=new unsigned char[FRAME_MAX_SIZE];
	 memset(send_q.q_buf[i],0,FRAME_MAX_SIZE);
	}

}

int Buffer::GetFrame(unsigned char* c, int num)// 把queue往c里面读取
{
	unsigned char* str=c;

	memset(str,0,FRAME_MAX_SIZE);
	memcpy(str, recv_q.q_buf[recv_q.tail], num);	//注意此处的读取的长度，一定不能超过c的长度！！！
	
	INC(recv_q.tail);
	
	// int posit=(((recv_q.tail) - 1) & (BUF_MAX_SIZE - 1));//保证读写指针在0~max之间循环
    return num;
}
int Buffer::PutFrame(unsigned char* c, int num)// 把c往queue里面写入
{
	unsigned char* str=c;
	
	memset(recv_q.q_buf[recv_q.head],0,FRAME_MAX_SIZE);
	memcpy(recv_q.q_buf[recv_q.head], str, num);

	INC(recv_q.head);
    return num;
}


