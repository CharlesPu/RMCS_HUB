/*******************************
@@Author     : Charles
@@Date       : 2017-12-15
@@Mail       : pu17rui@sina.com
@@Description: basic structure and macro             
*******************************/
#ifndef __RMCS_HUB_CIR_QUEUE_H__
#define __RMCS_HUB_CIR_QUEUE_H__

#include <pthread.h>
class Rtu;
struct _cell
{
	Rtu *r;
	unsigned char *q_buf;
};

class Cir_Queue
{
public:
	Cir_Queue();
	Cir_Queue(int, int);
	~Cir_Queue();

	struct _cell *cq;	// real buff
	pthread_cond_t buf_signal;
	pthread_mutex_t buf_lock;

	int IsCQEmpty();
	int IsCQFull();
	void PrintCell();
	void PrintCell(int n);
	void cq_test();
	int GetCell(Rtu *&_r, unsigned char *c, int num);
	int PutCell(Rtu *_r, unsigned char *c, int num);
private:
	unsigned int head;		// write, need nmutex!
	unsigned int tail;		// read, need nmutex!
	int buf_max_size;
	int cell_max_size;	
};

// #define INC(a) ((a) = ((a) + 1) & (BUF_MAX_SIZE - 1)) 
// #define DEC(a) ((a) = ((a) - 1) & (BUF_MAX_SIZE - 1))   
// #define EMPTY(a) ((a).head == (a).tail)                
// #define LEFT(a) (((a).tail - (a).head - 1)&(BUF_MAX_SIZE - 1))    
// #define LAST(a) ((a).buf[(BUF_MAX_SIZE - 1)&((a).head - 1)])      
// #define FULL(a) (!LEFT(a))                            
// #define CHARS(a) (((a).head - (a).tail)&(BUF_MAX_SIZE-1)) 


#endif 

