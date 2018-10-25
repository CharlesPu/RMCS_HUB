/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "mthread.h"
#include "log.h"
#include "config.h"

HUB_MThread :: HUB_MThread(struct _thread_args *t_a, int n)
{
	thread_args = t_a;
	threads_max_num = n;
	threads_num = 0;

	threads = new pthread_t[threads_max_num];
	attrs 	= new pthread_attr_t[threads_max_num];
}

HUB_MThread :: ~HUB_MThread()
{
	if (threads != NULL) {delete[] threads; threads = NULL;}
	if (attrs != NULL) {delete[] attrs; attrs = NULL;}
	threads_num = 0;
}

int HUB_MThread :: CreateThread(int policy, int prior, void *(*func)(void *))
{
	if (threads_num > threads_max_num)
	{
		LOG_ERROR(0, "threads num is not enough!\n");
		return 1;
	}
	struct sched_param sched_params;
	pthread_attr_init(&attrs[threads_num]);
	pthread_attr_setschedpolicy(&attrs[threads_num], policy);   //设置执行方式
	sched_params.sched_priority = prior;
	pthread_attr_setschedparam(&attrs[threads_num], &sched_params);//设置执行优先级
	//要使优先级其作用必须要有这句话!!!!
	int ret = pthread_attr_setinheritsched(&(attrs[threads_num]), PTHREAD_EXPLICIT_SCHED);
	if (ret)
	{
		LOG_ERROR(0, "thread %d set inherit sched error! pthread_attr_setinheritsched erron: %d\n", threads_num, ret);
		return 2;
	}
	// int tmp1; struct sched_param tmp2;
	// pthread_attr_getschedpolicy(&attrs[threads_num], &tmp1);printf("tmp1:%d\n", tmp1);
	// pthread_attr_getschedparam(&attrs[threads_num], &tmp2);printf("tmp2:%d\n", tmp2.sched_priority);
	// thread_args->_rtus->test();thread_args->_hub_server->test();printf("%d\n", threads_num);
	ret = pthread_create(&threads[threads_num], &attrs[threads_num], func, (void*)thread_args);
	pthread_attr_destroy(&attrs[threads_num]);
	if (ret)
	{
		LOG_ERROR(0, "thread %d create error! pthread_create erron: %d\n", threads_num, ret);
		return 3;
	}
	// printf("%d %d %d\n", EAGAIN,EINVAL,EPERM);
	LOG_INFO("thread %d creating...\n", threads_num);
	// attention!!!
	threads_num++;//printf("%d\n", threads_num);

	return 0;
}

int HUB_MThread :: Run()
{
	for (int i = 0; i < threads_num; ++i)
	{
		pthread_join(threads[i], 0);
	}

	return 0;
}
