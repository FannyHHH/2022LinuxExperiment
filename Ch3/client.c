#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sockfd;
	char buffer[1024];
	char sendbuf[256];
	int len1;
	int len;
	struct sockaddr_in server_addr; /*定义服务器端套接口数据结构server_addr */
	struct hostent *host;/*定义一个hostent结构的指针 */
	int portnumber,nbytes;

    host=gethostbyname(argv[1]);/*获得命令行的第二个参数-主机名*/
    portnumber = atoi(argv[2]);  /*获得命令行的第三个参数--端口号，atoi()把字符串转换成整型数*/

	/* 客户程序开始建立 sockfd描述符 */
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	/*客户程序填充服务端的资料*/
	bzero(&server_addr,sizeof(server_addr));/*先清空*/
	server_addr.sin_family=AF_INET; /*设为TCP/IP地址族*/
	server_addr.sin_port=htons(portnumber); /*设为server开放的端口*/
	server_addr.sin_addr=*((struct in_addr *)host->h_addr); /*设为Server的ip*/
	/*客户程序发起连接请求*/
	if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		exit(1);
	}
	/*连接成功，调用read读取服务器发送来的数据*/
	printf("please enter your student number to query your score\n");
	scanf("%s", sendbuf);
	len1 = sizeof(sendbuf);
	send(sockfd, sendbuf, len1, 0);
	printf("Query result:");
	if((nbytes=read(sockfd,buffer,1024))==-1)
	{
		fprintf(stderr,"Read Error:%s\n",strerror(errno));
		exit(1);
	} 
    
	buffer[nbytes]='\0';
	printf("%s\n",buffer);   /*输出接收到的数据*/
	close(sockfd);   /*结束通信*/ 
	exit(0);
}

