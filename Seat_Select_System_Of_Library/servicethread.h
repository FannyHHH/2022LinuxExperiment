/*servicethread.h*/
#ifndef __SERVICE_THREAD_H
#define __SERVICE_THREAD_H

#include "seat.h"
#include "threadbuff.h"

/*thread_err: 服务线程的错误处理函数，由于服务器端使用的是多线程技术，服务线程发生错误时，不能像在多进程的情况下，简单地调用exit()终止进程。在多线程下，服务线程必须将使用的资源释放后，调用pthread_exit()退出，并且在需要进行线程间同步的情况下，还需要做一些线程同步的工作，才能退出。这个特点在多线程编程中是非常重要的。*/
static void thread_err(char *s, int index)
{
	int i;
	char msg[512];
	/*获取空闲的界面输出信息缓冲区，如果没有空闲的,延迟一段时间后继续获取*/
	sprintf(msg,"线程 %d 发生致命错误：,%s\n",(unsigned short)pthread_self(),s);
	add_info(msg);
	//info_print(strmsg,serverwindow);
	/*释放线程使用的线程缓冲区*/
	free_buff(index);
	pthread_exit(NULL);
}

/*查询剩余座位*/
//*****************
/*
void inquire(int room_id)
{

	int i;
	char msg[512];
	for(i=0;i<ROOM_NUM;i++) {
		sprintf(msg,"\t %s:  剩余间数：%d",change_id(room_list[i].room_id),room_list[i].room_num);
		printf("%s\n",msg);
	}
}
*/

/*service_thread:服务线程的线程函数。服务线程根据函数的参数中获取自身使用的线程缓冲区的序号，而后根据这个序号从线程缓冲区中获取需要的参数*/
void * service_thread(void *p)
{
	int conn_fd;
	int buff_index;
	char send_buf[1024],recv_buf[512];
	int ret,i,cnt;
	uint16_t nbyte;
	struct sockaddr_in peer_name;
	int peer_name_len;
	unsigned int required_room_num;
	int pos;
	thread_buff_struct *pstruct;
	char msg[512];
	/*获取线程使用的线程缓冲区的序号*/
	pstruct=(thread_buff_struct *)p;
	buff_index=pstruct->buff_index;
	pstruct->tid=pthread_self();

	/*从线程缓冲区中获取通信使用的套接字描述符*/
	conn_fd=pstruct->conn_fd;

	/*打印远端主机地址*/
	peer_name_len=sizeof(peer_name);
	ret=getpeername(conn_fd,(struct sockaddr*)&peer_name, &peer_name_len);
	if(ret==-1) 
	thread_err("获取远端主机地址出错",buff_index);

	sprintf(msg,"新连接-->线程ID：%d, 线程缓冲区索引号：%d, 远端地址：%s, 端口号：%d\n",(unsigned short)pstruct->tid,buff_index, inet_ntoa(peer_name.sin_addr), ntohs(peer_name.sin_port));
	add_info(msg);
	while(1) {
		/*从网络中获取数据记录*/
		ret=recv(conn_fd,recv_buf,sizeof(message),0);		
		/*接收出错*/
		if(ret==-1) {
			sprintf(msg,"线程：%d 在连接：%d 接收出错。连接将关闭。\n",(unsigned short)pstruct->tid, conn_fd);
			add_info(msg);
			thread_err(msg, buff_index);
		}
		/*ret==0说明客户端连接已关闭*/
		if(ret==0) {
			sprintf(msg,"线程  %d  的连接( ID：%d ) 客户端已关闭。服务器端连接也将关闭。\n",(unsigned short)pstruct->tid, conn_fd);
			add_info(msg);
			close(conn_fd);
			free_buff(buff_index);
			pthread_exit(NULL);
		}
		/*ret为其他值说明接收到了客户端的请求消息*/
		init_message();
		memcpy(&message,recv_buf,sizeof(message));
		
		int seat_num;
		if(message.room_id == LIBRADY_FLOOR_5) seat_num = LIBRADY_FLOOR_5_SEAT_NUM;
                if(message.room_id == STUDY_ROOM) seat_num = STUDY_ROOM_SEAT_NUM;
            	init_seat_list(message.room_id); 
		switch(message.msg_type) {
		case  DISCONNECT:
			sprintf(msg,"线程 %d  的连接(ID： %d ) 客户端已关闭。服务器端连接也将关闭。\n",(unsigned short)pstruct->tid, conn_fd);
			add_info(msg);
			close(conn_fd);
			free_buff(buff_index);
			pthread_exit(NULL);
		break;
		case  APPLY_SEAT :
		printf("--------用户正在进行申请座位操作-------\n");
		printf("申请位置为 = %s的%d号座位\n", change_id(message.room_id), message.seat_id);
        printf("-------------------------------------");
		
		for(i=0; i<seat_num; i++) {
			pthread_mutex_lock(&seat_list[i].seat_mutex);
			if(seat_list[i].seat_id==message.seat_id) {
				if(seat_list[i].is_empty==1) { 
					message.msg_type=APPLY_SUCCEED;
					seat_list[i].is_empty = 0;
					pthread_mutex_unlock(&seat_list[i].seat_mutex);
					sprintf(msg,"%s的%d号座位被成功申请了\n",change_id(message.room_id), message.seat_id);
					add_info(msg);
					//inquire();
					memcpy(send_buf,&message,sizeof(message));			
					ret=send(conn_fd, send_buf, sizeof(message), 0);
					if(ret<0)
					thread_err("发送数据出错\n", buff_index);
					break;
				} else { //座位已被占用，申请失败
					message.msg_type=APPLY_FAILED;
					message.seat_id=seat_list[i].seat_id;
					pthread_mutex_unlock(&seat_list[i].seat_mutex);
					sprintf(msg,"申请失败！%s的%d座位已被占用\n",change_id(message.room_id), message.seat_id);
					//add_info(msg);
					memcpy(send_buf,&message,sizeof(message));
					ret=send(conn_fd, send_buf, sizeof(message), 0);
					if(ret<0)
					thread_err("发送数据出错\n", buff_index);
					break;
				}
			}
			pthread_mutex_unlock(&seat_list[i].seat_mutex);
		}
		break;
	
		case  INQUIRE_ALL:
		pos=0;
		printf("--------用户正在进行查询座位情况-------\n");
		printf("--------查询位置：%s-------\n", change_id(message.room_id));
		for(i=0; i<seat_num; i++) {
			pthread_mutex_lock(&seat_list[i].seat_mutex);
			message.msg_type=INQUIRE_SUCCEED;
			message.room_id=seat_list[i].room_id;
			message.seat_id=seat_list[i].seat_id;
                       message.is_empty=seat_list[i].is_empty;
			
			pthread_mutex_unlock(&seat_list[i].seat_mutex);
			if(i==0) {
				sprintf(msg,"查询座位信息成功！\n");
				//add_info(msg);
			}
			sprintf(msg,"%s的%d号座位占用情况为：%s",change_id(message.room_id), message.seat_id, change_empty(message.is_empty));
			
			//add_info(msg);
			memcpy(send_buf+pos,&message,sizeof(message));
			pos+=sizeof(message);
		}
		ret=send(conn_fd, send_buf, pos, 0);

		if(ret<0)
		thread_err("发送数据出错\n", buff_index);
		break;
		default :
		message.msg_type=UNKNOWN_CODE;
		memcpy(send_buf, &message,sizeof(message));
		ret=send(conn_fd, send_buf, sizeof(message), 0);
		if(ret<0)
			thread_err("发送数据出错\n", buff_index);
		}
	}
}

#endif


