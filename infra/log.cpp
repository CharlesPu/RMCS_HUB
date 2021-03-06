/*******************************
@@Author     :  Charles
@@Date       :  2018-10-19
@@Mail       :  pu17rui@sina.com
@@Description: 	logs print
*******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include "log.h"
#include "config.h"

static pthread_mutex_t gLOG_out_file_mutex;
static FILE* gLOG_out_fd = NULL;
static char out_file_path[128] = {0};
static int txt_max_size = 0;

int LOG_Init(const char *p, int n)
{
	pthread_mutex_init(&gLOG_out_file_mutex, NULL);
	if (strlen(p) > sizeof(out_file_path))
	{
		printf("LOG init wrong!\n");
		return 1;
	}
	memcpy(out_file_path, p, strlen(p));
	txt_max_size = n;
#ifdef LOG_SAVE
	if (access(out_file_path, F_OK) == -1)
	{
		mkdir(out_file_path, S_IRWXU | S_IRWXG | S_IRWXO);
	}	
#endif
	return 0;
}
int LOG_Save(char* data, int len)
{
	char file_name[256] = {0};

	time_t now;
	struct tm  *timenow;
	time(&now);
	timenow = localtime(&now);
	// printf("Local time is %s", asctime(timenow));
	pthread_mutex_lock(&gLOG_out_file_mutex);
	/* the file name */
	sprintf(file_name, "%slog_%d%02d%02d", out_file_path, 
				timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday);
	// printf("%s\n", file_name);
	if (access(file_name, F_OK) == -1 || gLOG_out_fd == NULL)//file not exists or not null
		gLOG_out_fd = fopen(file_name, "a+");
	fwrite(data, len, 1, gLOG_out_fd);
	fflush(gLOG_out_fd);//write immediately from sys buf
	pthread_mutex_unlock(&gLOG_out_file_mutex);

	return 0;
}
int LOG_Print(const char *log_type, int err_n, int line_n, const char *func_name, const char* format, ...)
{
	char buf[txt_max_size * 2] = {0};
	char data[txt_max_size] = {0};
	/*get all the params in ...*/
	va_list args_tmp;
	va_start(args_tmp, format);
	vsnprintf(data, sizeof(data), format, args_tmp);
	va_end(args_tmp);

	time_t now;
	struct tm *timenow;
	time(&now);
	timenow = localtime(&now);
	/*add the header of the data*/
	int len = sprintf(buf, "[%s %d-%02d-%02d %02d:%02d:%02d]", log_type,
				timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday, 
				timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	if (!strcmp(log_type, "ERROR"))
		len += sprintf(buf + len, "[ %d ]", err_n);
	len += sprintf(buf + len, " <%d>", line_n);
	len += sprintf(buf + len, "<%s>", func_name);
	len += sprintf(buf + len, " %s", data);

	// int len = sprintf(buf, "%s", data);
	printf("%s", buf);
	fflush(stdout);//write immediately from sys buf
#ifdef LOG_SAVE
	if (strcmp(log_type, "DEBUG"))
		LOG_Save(buf, len);
#endif
	return 0;
}
int LOG_Free(void)
{
	pthread_mutex_destroy(&gLOG_out_file_mutex);
	
	if (gLOG_out_fd != NULL)
	{
		fclose(gLOG_out_fd);
		gLOG_out_fd = NULL;
	}
	memset(out_file_path, 0, sizeof(out_file_path));
	txt_max_size = 0;

	return 0;
}
