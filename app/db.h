#ifndef __RMCS_HUB_DB_H__
#define __RMCS_HUB_DB_H__ 

#include <mysql/mysql.h>

class HUB_Mysql
{
public:
	HUB_Mysql(const char *d, const char *db_srv_name, const char *usr_name, const char *passwd);
	~HUB_Mysql();

	/*RTU*/
	int ExistRtu(const unsigned char reg_pack_x[8]);
	// bool UpdateRtuSocket(const int sock, const unsigned char reg_pack_x[8]);
	// void GetByRegpack(const char* tb, const char* field_name, unsigned char reg_pack_x[8], const char* field_search, int* search_value);
	// int FindRtuSocket(const int loc_id, const int sta_id);
	/*recv*/

	/*send*/	
private:
	MYSQL* mysql_fd;	
	char db_name[128];
	int SelectDatabase(const char* db);
	int TruncateTable(const char* tb);	
};

#endif