/*******************************
@@Author     : Charles
@@Date       : 2018-10-23
@@Mail       : pu17rui@sina.com
@@Description:          
*******************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "db.h"
#include "log.h"
#include "config.h"

HUB_Mysql :: HUB_Mysql(const char *d, const char *db_srv_name, const char *usr_name, const char *passwd)
{
	mysql_thread_init();//neccessary in multi thread program
    mysql_fd = mysql_init(NULL);
	if (mysql_fd == NULL)
	{
		LOG_ERROR(0, "mysql init fail!\n");
		return;
	}
	if (strlen(d) > sizeof(db_name))
	{
		LOG_ERROR(0, "mysql init db name wrong!\n");
		return;
	}
	memcpy(db_name, d, strlen(d));
	if (!mysql_real_connect(mysql_fd, db_srv_name, usr_name, passwd, db_name, 0, NULL, 0))
	{
		LOG_ERROR(0, "mysql init connect db error!, mysql err_n: %d\n", mysql_error(mysql_fd));
		return;
	}
}

HUB_Mysql :: ~HUB_Mysql()
{
	mysql_close(mysql_fd);
	mysql_thread_end();//neccessary in multi thread program
	memset(db_name, 0, sizeof(db_name));
}

int HUB_Mysql :: SelectDatabase(const char* db)
{
	if (mysql_select_db(mysql_fd, db))
	{
		LOG_ERROR(0, "mysql init select db error!, mysql err_n: %d\n", mysql_error(mysql_fd));
		return 1;
	}
	return 0;
}

int HUB_Mysql :: TruncateTable(const char* tb)
{
	char cmd[128] = {0};
	sprintf(cmd, "truncate table %s", tb);
	if (mysql_query(mysql_fd, cmd))
	{
		LOG_ERROR(0, "truncate table fail!\n");
		return 1;
	}
	return 0;
}

int HUB_Mysql :: ExistRtu(const unsigned char reg_pack_x[8])
{
	char cmd[256] = {0};
	int posi = sprintf(cmd, "select * from RTU where RTU_reg_pack='");
	for (int i = 0; i < 8; ++i)
		posi += sprintf(cmd + posi, "%02x", reg_pack_x[i]);
	sprintf(cmd + posi, "'");
	int ret = 0;
	// printf("%s\n", cmd);
	if(mysql_query(mysql_fd, cmd))
	{
		LOG_ERROR(0, " mysql_query exist rtu fail\n");
		ret = -1;
	}else
	{
		MYSQL_RES *res = mysql_use_result(mysql_fd);
		MYSQL_ROW row  = mysql_fetch_row(res);
		if (row == NULL) 
			ret = 1; //not exist
		else 
			ret = 0;  //exist
		mysql_free_result(res);
	}

	return ret;
}
