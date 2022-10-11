
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

struct ScoreInfo//学生成绩表
{
	char snum[10];
	char score[3];
};

int main(int argc, char *argv[])
{
	int sockfd,new_fd;   /*定义存放套接口描述符的变量 */
	struct sockaddr_in server_addr;   /*定义服务器端套接口数据结构server_addr */
	struct sockaddr_in client_addr;   /*定义客户端套接口数据结构client_addr */
	int sin_size,portnumber;
	char buf[9];     /*定义发送数据缓冲区*/

        portnumber=atoi(argv[1]); //从命令行获取端口参数，并把字符串转换成整型数
	sockfd=socket(AF_INET,SOCK_STREAM,0);/*服务器端开始建立socket描述符*/
	
	/*服务器端填充 sockaddr结构*/
	bzero(&server_addr,sizeof(struct sockaddr_in)); /*先将套接口地址数据结构清零*/
	server_addr.sin_family=AF_INET;/*设为TCP/IP地址族*/
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*设置本机地址并从主机字节序转换为网络字节序*/
	server_addr.sin_port=htons(portnumber);/*设置端口号并从主机字节序转换为网络字节序*/
	bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr));/*调用bind函数绑定指定的端口号和ip地址到服务器创建的套接口*/
	
	listen(sockfd,5); /*端口绑定成功，监听sockfd描述符，设置同时处理的最大连接请求数为5 */


	while(1)      /*服务器阻塞，等待接收连接请求，直到客户程序发送连接请求*/
	{
		sin_size=sizeof(struct sockaddr_in);
		if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)	  /*调用accept接受一个连接请求并返回一个新的套接口描述符*/			
		{
			fprintf(stderr,"Accept error:%s\n\a",strerror(errno));
			exit(1);
		}
		fprintf(stderr,"Server get connection from %s\n",(inet_ntoa(client_addr.sin_addr)));   /*TCP连接已建立，打印申请连接的客户机的IP地址,IP地址从网络字节序转换为十进制数*/	
		if(recv(new_fd,buf,9,0) > 0) {
			printf("收到客户端的学号:%s\n",buf);//输出到终端
			FILE *fp = fopen("./score.txt", "r"); 
			struct ScoreInfo sc;
			char *sendbuf="Sorry, Not Found.\n";
			while(fread(&sc, sizeof(sc), 1, fp) == 1)
			{
				
				sc.snum[9] = '\0';
				if(strcmp(buf, sc.snum) == 0) //学号和成绩表中学号相同
				{
					sendbuf = sc.score;
					write(new_fd,sendbuf,strlen(sendbuf));
					break;
				}
				write(new_fd,sendbuf,strlen(sendbuf));//回复查询失败消息消息
			}
			
		}	
		close(new_fd);  /*本次通信已结束，关闭客户端的套接口，并循环下一次等待*/
	}
	close(sockfd);  /*服务器进程结束，关闭服务器端套接口*/
	exit(0);
}
