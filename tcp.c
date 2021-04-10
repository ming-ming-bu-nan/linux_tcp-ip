#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define N  128

//TCP客户端代码
#if 1
int main(int argc,char const *argv[])
{

	if(argc < 3)
	{
		fprintf(stderr,"Usage:%s ip port\n",argv[0]);
		exit(1);
	}
	//通过socket函数创建一个TCP套接字
	int sockfd;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	//printf("sockfd = %d\n",sockfd);
	
	//发送客户端连接请求
	struct sockaddr_in serveraddr;
	socklen_t addrlen  = sizeof(serveraddr);
	
	serveraddr.sin_family = AF_INET;//协议族，IPV4
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sockfd,(struct sockaddr*)&serveraddr,addrlen) == -1)
	{
		perror("fail to connect");
		exit(1);
	}
	
	
	//进行通信
	//发送数据
	char buf[N] = "";
	while(1)
	{
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';
		if(send(sockfd,buf,N,0) == -1)
		{
			perror("fail to send");
			exit(1);
		}
		
		//接收数据
		char text[N] = "";
		if(recv(sockfd,text,N,0) == -1)
		{
			perror("fail to recv");
			exit(1);
		}
		
		printf("from recv = %s\n",text);
	}

	
	//关闭套接字
	close(sockfd);
	
	return 0;
}
#endif

//TCP服务器
#if 0
int main(int argc,char const *argv[])
{

	if(argc < 3)
	{
		fprintf(stderr,"Usage:%s ip port\n",argv[0]);
		exit(1);
	}
	//通过socket函数创建一个TCP套接字
	int sockfd;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	
	//绑定套接字与服务器信息结构体
	struct sockaddr_in serveraddr;
	socklen_t addrlen  = sizeof(serveraddr);
	
	serveraddr.sin_family = AF_INET;//协议族，IPV4
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	if(bind(sockfd,(struct sockaddr*)&serveraddr,addrlen) == -1)
	{
		perror("fail to connect");
		exit(1);
	}
	
	//修改套接字为被动
	if(listen(sockfd,5) == -1)
	{
		perror("fail to listern");
		exit(1);
	}
	
	
	int accept1;
	struct sockaddr_in clientaddr;
	socklen_t clilen  = sizeof(clientaddr);
	if((accept1 = accept(sockfd,(struct sockaddr*)&clientaddr,&clilen)) == -1)
	{
		perror("fail to accept");
		exit(1);
	}
	
	//打印客户端的信息
	printf("ip:%s,port:%d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

	//进行通信
	//char text[N] = "";
	char buf[N] = "";
	if(recv(accept1,buf,N,0) == -1)
	{
		perror("fail to recv");
		exit(1);
	}
	printf("from recv = %s\n",buf);
	
	strcat(buf,"*_*");
	if(send(accept1,buf,N,0) == -1)
	{
		perror("fail to send");
		exit(1);
	}
	
	//关闭套接字
	close(sockfd);
	close(accept1);
	
	return 0;
}
#endif
