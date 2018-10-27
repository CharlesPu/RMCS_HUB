#ifndef __RMCS_HUB_RTU_H__
#define __RMCS_HUB_RTU_H__

#include "cir_queue.h"

class Rtu
{
public:
	Rtu();
	~Rtu();

	int rtu_sock;   //need mutex!!!
	unsigned int reg_pack_int; 		// int(last 2 bytes and merge them)
	unsigned char reg_pack_hex[8];  //hex
};

class Rtus : public Cir_Queue
{
public:
	Rtus(int);
	~Rtus();

	Rtu *rtus; //real rtus array

	// void SortRtus(void);
	// struct rtu* FindRtuBySock(const int socket);
	// struct rtu* FindRtuByRegpack(const unsigned int reg_package);
	// int GetNeedProcessRtus();
	void test();
private:
	int rtu_max_num;
	int rtu_num;
};	

#endif