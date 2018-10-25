#ifndef __CONFIG_H__
#define __CONFIG_H__

/* DEBUG */
#define PRINT_SRV_DATA_RAW
/* rtus */
#define RTUS_MAX 256 //real is 15*15=225
/* server */
#define SRV_CLIENT_MAX 256   
#define SRV_IP "47.100.183.68"
#define SRV_PORT 10001
#define SRV_BUF_MAX 2048
/*************mysql**************/
#define DB_SERV_NAME "localhost"
#define DB_USER_NAME "root"
#define DB_PASSWORD  "1"
#define DB_NAME 	 "zhongzhen"
/* buffer */
#define CQ_BUF_MAX 256
#define CQ_CELL_MAX 1024
/* infra - log */
// #define LOG_SAVE
#define LOG_OUT_PATH "./log/"
#define LOG_CONTENT_MAX 256

#endif
