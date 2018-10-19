/*******************************
@@Author     :  Charles
@@Date       :  2018-10-19
@@Mail       :  pu17rui@sina.com
@@Description: 	
*******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int port;
	if (argc == 1)
		port = PORT;
	else
		port = atoi(argv[1]);
	Server serv;
	if (serv.ServerInit(port))
		return 1;
	printf("Server init success! port:%d\n",port);
	Rtus rtus;

	mysql_library_init(0, NULL, NULL);
	Params params;
	params._rtus=&rtus;
	params._server=&serv;

	pthread_t threads[TASK_NUM];
	Task tasks;
	tasks.TaskInit();
	//创建线程
	int ret1=pthread_create(&threads[0],&tasks.attr[0],tasks.ServerReceive,(void*)&params);
	int ret2=pthread_create(&threads[1],&tasks.attr[1],tasks.DataProcess,(void*)&rtus);
	int ret3=pthread_create(&threads[2],&tasks.attr[2],tasks.Control,(void*)&params);
	//记住要摧毁attr[ ]!!!
	for(int i=0;i<TASK_NUM;i++)
		pthread_attr_destroy(&tasks.attr[i]);

	if(ret1 || ret2 ||ret3)
	{
		perror("Threads create error!\n");
		return 1;
	}
	printf("Threads create success!\n");
	//让线程跑起来~
	for(int i=0;i<TASK_NUM;i++)
		pthread_join(threads[i],0);

	mysql_library_end();
	
	return 0;
}
