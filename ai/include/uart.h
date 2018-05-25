#ifndef _UART_H_
#define _UART_H_

#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sqlite3.h>

#define price 0.5 //单价
typedef struct 
{
	char id[20]; 		  //卡号
	char name[20];    //用户名
	char phone[13];	      //联系方式
	char CarNum[20];  //车牌号
	unsigned int flag;         //停车标志位，=1结算
	long int date;
}MSG;
MSG temp;
MSG *usr;
pthread_mutex_t photo_mutex;

int uart1_init(void);
int message_get(int fd, char *id_read, sqlite3 *db);
int id_get(char *id_read, char *read_buf);
sqlite3 *open_db(MSG *usr);
int usr_register(MSG *usr, sqlite3 *db);
int execut(MSG *usr, sqlite3 *db);
int callback1(void *pv, int num, char **buf, char **tname);
int callback2(void *pv, int num, char **buf, char **tname);
void time_in(MSG *usr, sqlite3 *db);
void time_out(MSG *usr, sqlite3 *db);
float check_bill(long int now_time, long int date);

#endif




