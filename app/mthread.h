#ifndef __RMCS_HUB_MTHREAD_H__
#define __RMCS_HUB_MTHREAD_H__

#include <pthread.h>

class Rtus;
class HUB_Server;
struct _thread_args
{
	Rtus* _rtus;
	HUB_Server* _hub_server;
};

class HUB_MThread
{
public:
	HUB_MThread(struct _thread_args *, int);
	~HUB_MThread();

	pthread_t *threads;
	pthread_attr_t *attrs;
	int CreateThread(int policy, int prior, void *(*func)(void *));
	int Run();
private:
	struct _thread_args *thread_args;
	int threads_max_num;
	int threads_num;
};

#endif