#ifndef __RMCS_HUB_SERVER_H__
#define __RMCS_HUB_SERVER_H__ 

#include <netinet/in.h>
#include <sys/epoll.h>

class HUB_Server
{
public:
	HUB_Server(int, const char *, int, int);
	~HUB_Server();

    int Accept();
    int Send(int, unsigned char* , int);
    int Recv(int);
    static void *Receive(void *);
    void test();
    
    int connect_num;
    struct sockaddr_in serv_addr;

    unsigned char* rxbuf;
    unsigned char* txbuf;
private:
	int port;
	char ip[16];
    int client_max_num;
    int buf_max_size;

    int listen_fd;
    int ep_fd;
    // struct epoll_event *evts;

	int SetNonblock(int fd);
    static HUB_Server *this_tmp;
};

#endif