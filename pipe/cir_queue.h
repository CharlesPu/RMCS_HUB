/*******************************
@@Author     : Charles
@@Date       : 2017-12-15
@@Mail       : pu17rui@sina.com
@@Description: basic structure and macro             
*******************************/
#ifndef __RMCS_HUB_CIR_QUEUE_H__
#define __RMCS_HUB_CIR_QUEUE_H__

#include <pthread.h>

class Buffer
{
public:
	Buffer(int, int);
	~Buffer();

 	pthread_cond_t buf_signal;
	pthread_mutex_t buf_lock;

	virtual int GetCell(unsigned char *c, int num);// 把queue往c里面读取,返回读取的字节数
	virtual int PutCell(unsigned char *c, int num);// 把c往queue里面写入	
protected:
	int buf_max_size;
	int cell_max_size;	
};

class Cir_Queue : public Buffer
{
public:
	Cir_Queue(int, int);
	~Cir_Queue();

	unsigned char **q_buf;	// real buff
	int IsCQEmpty();
	virtual int GetCell(unsigned char *c, int num);
	virtual int PutCell(unsigned char *c, int num);
private:
	unsigned char head;		// write
	unsigned char tail;		// read
};

// #define INC(a) ((a) = ((a) + 1) & (BUF_MAX_SIZE - 1)) //a自增但是不能超过最大范围，若超过255，则变为0，因为是8位
// #define DEC(a) ((a) = ((a) - 1) & (BUF_MAX_SIZE - 1))   
// #define EMPTY(a) ((a).head == (a).tail)                //首末指针相等即空
// #define LEFT(a) (((a).tail - (a).head - 1)&(BUF_MAX_SIZE - 1))    
// #define LAST(a) ((a).buf[(BUF_MAX_SIZE - 1)&((a).head - 1)])      
// #define FULL(a) (!LEFT(a))                             // 首末指针相差1则认为满
// #define CHARS(a) (((a).head - (a).tail)&(BUF_MAX_SIZE-1)) 


#endif 

