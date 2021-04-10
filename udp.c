/* UDP协议：面向无联机的用户数据包，传输数据之前不需要建立连接，目的主机收到UDP报文后，不需要给出任何确认。
特点：1.相比TCP速度快
	  2.简单的请求/应答应用程序可以使用UDP
	  3.对于海量数据传输不应该使用UDP
	  4.广播和多播应用必须使用UDP
应用：DNS（域名解析），NFS（网络文件系统），RTP（流媒体）等,一般语音和视频通话都是使用UDP实现


 */
 
 /*
 UDP编程C/S架构：
 UDP网络编程流程：
 服务器：
		创建套接字（socket()）
		将服务器的IP地址、端口号与套接字进行绑定（bind()）
		接受数据（recvfrom()）
		发送数据（sendto（））
 客户端:
		创建套接字（socket（））
		发送数据（sendto（））
		接受数据（recvfrom（））
		关闭套接字（close（））
*/

//创建套接字
//int socket (int family,int type,int protocol)
//功能：创建一个用于网络通信的socket套接字（描述符）
//参数：family:协议族（AF_INET,AF_INET6,PF_PACKET）
//		type:套接字类(SOCK_STREAM,SOCK_DGRAM,SOCK_RAW)
//		protocol:协议类别（0，IPPROTO_TCP,IPPROTO_UDP）,一般为0
//返回值：套接字
//特点：创建套接字时，系统不会分配端口
//		创建的套接字默认属性是主动的，即主动发起服务请求，当作为服务器时，往往需要修改为被动


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#if 0
int main()
{
	//使用socket函数创建套接字
	//创建一个用于UDP网络编程的套接字
	int socket_fd;
	if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	printf("socketfd = %d\n",socket_fd);
	
	return 0;
}
#endif


#if 0
//bind实例，服务器端

#define N  128

int main(int argc,char const *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"Usage:%s ip port\n",argv[0]);
		exit(1);
	}
	
	//使用socket函数创建套接字
	//创建一个用于UDP网络编程的套接字
	int socket_fd;
	struct sockaddr_in serveraddr;
	socklen_t addrlen  = sizeof(serveraddr);
	if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	//填充服务器的网络信息
	serveraddr.sin_family = AF_INET;//协议族，IPV4
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	
	//将网络信息结构体与套接字绑定
	if(bind(socket_fd,(struct sockaddr*)&serveraddr,addrlen) != 0)
	{
		perror("fail to bind");
		exit(1);
	}
	
	//接收数据
	char buf[N] = "";
	char n_buf[N] = "";
	struct sockaddr_in clientaddr;
	while(1)
	{
		if(recvfrom(socket_fd,buf,N,0,(struct sockaddr*)&clientaddr,&addrlen) == -1)
		{
			perror("fail to rec");
			exit(1);
		}
		printf("from c buf = %s\n",buf);
		printf("ip:%s,port:%d\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		
		fgets(n_buf,N,stdin);
		n_buf[strlen(n_buf) - 1] = '\0';//把buf字符串中的\n转化为\0
		if(sendto(socket_fd,n_buf,N,0,(struct sockaddr*)&clientaddr,addrlen) == 1)
		{
			perror("fail to sendto");
			exit(1);
		}
		
		
	}

	
	
	
	return 0;
}
#endif

//向网络调试助手发送数据
//客户端代码编写

#if 0

#define N  128



int main(int argc,char const *argv[])
{
	//./a.out 192.168.199.243 8080
	if(argc < 3)
	{
		fprintf(stderr,"Usage:%s ip port\n",argv[0]);
		exit(1);
	}
	
	//使用socket函数创建套接字
	//创建一个用于UDP网络编程的套接字
	int socket_fd;
	struct sockaddr_in serveraddr;
	socklen_t addrlen  = sizeof(serveraddr);
	if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	printf("socketfd = %d\n",socket_fd);
	
	//填充服务器网络信息结构体
	
	serveraddr.sin_family = AF_INET;//协议族，IPV4
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));
	
	//发送数据（sendto（）
	char buf[N] = "";
	char h_buf[N] = "";
	while(1)
	{
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';//把buf字符串中的\n转化为\0
		if(sendto(socket_fd,buf,N,0,(struct sockaddr*)&serveraddr,addrlen) == 1)
		{
			perror("fail to sendto");
			exit(1);
		}
		
		if(recvfrom(socket_fd,h_buf,N,0,(struct sockaddr*)&serveraddr,&addrlen) == -1)
		{
			perror("fail to rec");
			exit(1);
		}
		printf("from c h_buf = %s\n",h_buf);
		
	}
	
	close(socket_fd);
	return 0;
}

#endif


//广播实例
#if 1

#define N  128

int main(int argc,char const *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"Usage:%s ip port\n",argv[0]);
		exit(1);
	}
	
	//使用socket函数创建套接字
	//创建一个用于UDP网络编程的套接字
	int socket_fd;
	struct sockaddr_in broadaddr;
	socklen_t addrlen  = sizeof(broadaddr);
	if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		perror("fail to socket");
		exit(1);
	}
	
	//设置为允许发送广播权限
	int on = 1;
	if(setsockopt(socket_fd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on)) == -1)
	{
		perror("fail to setsockopt");
		exit(1);
	}
	
	//填充广播信息结构体
	broadaddr.sin_family = AF_INET;//协议族，IPV4
	broadaddr.sin_addr.s_addr = inet_addr(argv[1]);
	broadaddr.sin_port = htons(atoi(argv[2]));
	
	char buf[N] = "";
	while(1)
	{
		fgets(buf,N,stdin);
		buf[strlen(buf) - 1] = '\0';//把buf字符串中的\n转化为\0
		if(sendto(socket_fd,buf,N,0,(struct sockaddr*)&broadaddr,addrlen) == 1)
		{
			perror("fail to sendto");
			exit(1);
		}	
	}
}

#endif


//加入多播组实例：
char group[INET_ADDRSTRLEN] = "224.0.1.1";
//定义一个多播地址
struct ip_mreq mreq;
//添加一个多播组IP
mreq.imr_multiaddr.s_addr = inet_addr(group);
//添加一个将要添加到多播组的IP
mreq.imr_interface.s_addr = htonl(INADDR_ANY);

setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));

