#ifndef __GLOBALAPI_H
#define __GLOBALAPI_H

#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <pthread.h>

/*服务器端使用的端口*/
#define SERVER_PORT_NO 8889

/*客户端与服务器端使用的消息类型定义*/
#define INITIAL_VALUE 65535

/*客户端使用的消息代码含义*/
#define DISCONNECT 0
#define APPLY_SEAT 1
#define INQUIRE_ALL 2

/*服务器端使用的消息代码含义*/
#define APPLY_SUCCEED 255
#define APPLY_FAILED 256
#define INQUIRE_SUCCEED 257
#define UNKNOWN_CODE 258

#define TRUE 1
#define FALSE 0


/*服务器与客户端使用的消息结构定义*/
struct stMessage {
    unsigned int  msg_type;//消息类型。客户端可以取值为DISCONNEC、APPLY_SEAT、INQUIRE_ALL
    unsigned int room_id;
    unsigned int seat_id;//座位 
	unsigned int is_empty;
} message;

void init_message()
{
	message.msg_type=INITIAL_VALUE;
	message.room_id=0;
	message.seat_id=0;
	message.is_empty=1;
}



char * change_empty(int is_empty) {
	char * change_empty;
	switch(is_empty){
	case 0:change_empty="已占用";break;
	case 1:change_empty="可选择";break;
	default:break;
	}
	return change_empty;
}

/*服务器端的线程缓冲区的最大数量*/
#define THREAD_BUFF_NUM 128

//输出消息
void add_info(char *src)
{ 
	printf("%s\n",src);
}

#endif

int re_connect();

