/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:             
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "server.h"
#include "log.h"
#include "db.h"
#include "rtu.h"
#include "mthread.h"
#include "config.h"
#ifdef OMP_ENABLE
#include <omp.h>
#endif
//attention!
HUB_Server *HUB_Server::this_tmp;
HUB_Server :: HUB_Server(int p, const char *_ip, int clie_max, int buf_max)
{
	port = p;
	memcpy(ip, _ip, strlen(_ip));
	client_max_num = clie_max;

	connect_num = 0;
	buf_max_size = buf_max;
	rxbuf = new unsigned char[buf_max_size];
	memset(rxbuf, 0, buf_max_size);
	txbuf = new unsigned char[buf_max_size];
	memset(txbuf, 0, buf_max_size);
	// evts = new struct epoll_event[client_max_num];
	/* init */
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1)
	{
		LOG_ERROR(0, "socket create error!\n");
		return;
	}
	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
//	inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	int res = bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (res == -1)
	{
		close(listen_fd);
		LOG_ERROR(0, "port bind error!\n");
		return;
	}
	res = listen(listen_fd, client_max_num);
	if (res == -1)
	{
		close(listen_fd);
		LOG_ERROR(0, "socket listen error!\n");
		return;
	}
	/************************ epoll ********************/
	ep_fd = epoll_create(client_max_num);
	if (ep_fd == -1)
	{
		LOG_ERROR(0, "epoll create error!\n");
		return;
	}	
		
	// if (Server::SetNonblock(listen_fd))
	// 	return 5;
	struct epoll_event evt_temp;
	evt_temp.data.fd = listen_fd;
	evt_temp.events  = EPOLLIN ;
	if(epoll_ctl(ep_fd, EPOLL_CTL_ADD, listen_fd, &evt_temp) == -1)
	{
		LOG_ERROR(0, "epoll_ctl error!\n");
		return;
	}
	this_tmp = this;
	LOG_INFO("HUB_Server Init success, port: %d\n", port);
}

HUB_Server :: ~HUB_Server()
{
	if(rxbuf != NULL) {delete[] rxbuf; rxbuf = NULL;}
	if(txbuf != NULL) {delete[] txbuf; txbuf = NULL;}

	if(listen_fd) {close(listen_fd); listen_fd = 0;}
	if(ep_fd) {close(ep_fd); ep_fd = 0;}
}

int HUB_Server :: Accept()
{
	struct sockaddr_in clie_addr;
	socklen_t clie_addr_len = sizeof(clie_addr);
	int clie_fd = accept(listen_fd, (struct sockaddr *)&clie_addr, &clie_addr_len);

	if (clie_fd == -1)
	{
		LOG_ERROR(0, "accept error!\n");
		return 1;
	}
	else
	{
		LOG_INFO("HUB_Server connect to %s:%d, clie_fd: %d, connect_num:%d\n", 
				inet_ntoa(clie_addr.sin_addr), ntohs(clie_addr.sin_port), clie_fd, ++connect_num);

		if (SetNonblock(clie_fd))
		{
			LOG_ERROR(0, "set clie_fd nonblock fail!");
			return 2;
		}
		struct epoll_event evt_temp;
		evt_temp.events  = EPOLLIN ;
		evt_temp.data.fd = clie_fd;
		if(epoll_ctl(ep_fd, EPOLL_CTL_ADD, clie_fd, &evt_temp) == -1)
		{
			LOG_ERROR(0, "epoll_ctl clie_fd fail!");
			return 3;
		}
	}	

	return 0;
}
/* 
	if you need to use omp or thread pool to handle every fd returned by epoll_wait(), 
	you need to: 1. make HUB_Server::Recv reentrant: separate 'rxbuf' from HUB_Server.
				 2. delete the write operation of HUB_Server::connect_num.
*/
void *HUB_Server :: Receive(void *args)
{
	struct _thread_args *args_tmp = (struct _thread_args*)args;
	Rtus *rtus_tmp 				  = args_tmp->_rtus;
	HUB_Server *hub_server_tmp    = args_tmp->_hub_server;

	struct epoll_event evts[hub_server_tmp->client_max_num];
	while (1)
	{	
		int fd_num = epoll_wait(hub_server_tmp->ep_fd, evts, hub_server_tmp->client_max_num, -1);
		if (fd_num < 0)
		{
			LOG_ERROR(0, "epoll_wait fail\n");
			continue;
		}
#ifdef OMP_ENABLE
#pragma omp parallel for
#endif 
		for (int i = 0; i < fd_num; i++)
		{
			if( !(evts[i].events & EPOLLIN))          
				continue;
			if (evts[i].data.fd == hub_server_tmp->listen_fd)  
				hub_server_tmp->Accept();
			else if( evts[i].events & EPOLLIN )     
			{
				int fd_temp = evts[i].data.fd;
				int bytes_num = hub_server_tmp->Recv(fd_temp); // rxbuf is only one! omp wrong!!!
				if (bytes_num <= 0)
				{
					int res = epoll_ctl(hub_server_tmp->ep_fd, EPOLL_CTL_DEL, fd_temp, NULL);  
					if (res == -1)
						LOG_ERROR(0, "epoll_ctl_delete fail!\n");
					close(fd_temp);
					hub_server_tmp->connect_num--;  // cannot write at the same time! omp wrong!!!
					LOG_INFO("client fd %d closed connection\n", fd_temp);
				}else 
				if(bytes_num >= 8)
				{	
					unsigned char reg_package_x[8] = {0};
					memcpy(reg_package_x, hub_server_tmp->rxbuf + 0, 8);
					LOG_INFO("registration package: ");
					for (int i = 0; i < 8; i++)
						printf("%02x ", reg_package_x[i]);

					unsigned int reg_package = 0;
					reg_package = ((reg_package_x[6] << 4) & 0xf0) | (reg_package_x[7] & 0x0f);
					printf(", %d\n", reg_package);
					/* check reg pack */
					HUB_Mysql hub_mysql(DB_NAME, DB_SERV_NAME, DB_USER_NAME, DB_PASSWORD);
					if (!hub_mysql.ExistRtu(reg_package_x))//exist
					{
						/*parse and store*/
						pthread_mutex_lock(&(rtus_tmp->buf_lock));
						if (rtus_tmp->IsCQFull()) // if full, drop new data...
						{
							/* parse(drop header) */


							/* store to cir_queue*/
							rtus_tmp->PutCell(&(rtus_tmp->rtus[reg_package]), hub_server_tmp->rxbuf, bytes_num);
						}else
							LOG_INFO("Circle Queue is full!\n");
						// don't forget fd!
						rtus_tmp->rtus[reg_package].rtu_sock = fd_temp;
						pthread_mutex_unlock(&(rtus_tmp->buf_lock));
						pthread_cond_signal(&(rtus_tmp->buf_signal));
					}else
					{
						LOG_INFO("illegal reg package!\n");
					}
				}
			}		
		}
		// printf("1111111111111\n");
		usleep(1);
	}

	return 0;
}

void HUB_Server :: test()
{
	printf("HUB_Server test\n");
}

int HUB_Server :: Recv(int fd)
{
	memset(rxbuf, 0, buf_max_size);
	int num = read(fd, rxbuf, buf_max_size);
	if (num > 0)
	{
#ifdef PRINT_SRV_DATA_RAW
		LOG_INFO("recv len: %d data:\n", num);
		for (int i = 0; i < num; i++)
			printf("[%d]%02x ", i, rxbuf[i]);
		printf("\n");
#endif
	}
	return num;
}

int HUB_Server :: Send(int fd, unsigned char* buf, int length)
{
	int num = write(fd, buf, length);
	if (num != length)
	{
		LOG_ERROR(0, "send error!\n");
		return -1;
	}
	return num;
}

int HUB_Server::SetNonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if (flag == -1)
	{
		LOG_ERROR(0, "set nonblock error!\n");
		return 1;
	}
	flag |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flag) < 0)
	{
		LOG_ERROR(0, "set nonblock error!\n");
		return 2;
	}
	return 0;	
}