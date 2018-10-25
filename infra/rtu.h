#ifndef __RMCS_HUB_RTU_H__
#define __RMCS_HUB_RTU_H__

#include "cir_queue.h"
#include "config.h"
#include <vector>

using namespace std;

class Rtu : public Cir_Queue
{
public:
	Rtu(int, int);
	~Rtu();

	int rtu_sock;   //need mutex!!!
	unsigned int reg_pack_int; // int(last 2 bytes and merge them)
	unsigned char reg_pack_hex[8];  //hex
};

class Rtus
{
public:
	Rtus(int);
	~Rtus();

	Rtu *rtus; //real rtus array
	vector<Rtu*> need_proc_rtus;//real rtu's index(reg_pack)
	// struct rtu* rtus_by_regpack[RTUS_MAX_NUM]; //按注册包(long类型)排序，存放rtu的地址
	// struct rtu* rtus_by_sock[RTUS_MAX_NUM]; //按socket排序，存放rtu的地址

	// void SortRtus(void);
	// struct rtu* FindRtuBySock(const int socket);
	// struct rtu* FindRtuByRegpack(const unsigned int reg_package);
	int GetNeedProcessRtus();
	void test();
private:
	int rtu_max_num;
	int rtu_num;
};	

#endif