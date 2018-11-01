#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
int main(int argc,char* argv[])
{
    int socket_start = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP);
    int flag = 1;
	int connection;
	int bind_return;
	socklen_t length;
	char store_recv[1024];
	int port=atoi(argv[1]);
	struct sockaddr_in socket_ser;
	struct sockaddr_in socket_cli;
	struct sockaddr_in socket_temp;
	if (setsockopt(socket_start, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
    socket_ser.sin_family = AF_INET;
    socket_ser.sin_port = htons(port);
    socket_ser.sin_addr.s_addr =htons(INADDR_ANY);
	bind_return = bind(socket_start, (struct sockaddr *)&socket_ser, sizeof(socket_ser));
    if(bind_return==-1)
    {
        perror("bind");
        exit(1);
    }
    if(listen(socket_start,20) == -1)
    {
        perror("listen");
        exit(1);
    }
    length = sizeof(socket_cli);
    connection = accept(socket_start, (struct sockaddr*)&socket_cli, &length);
    if(connection<0)
    {
        perror("connect");
        exit(1);
    }
	else {
		cout << "与客户端连接成功!" << endl;
	}
    socklen_t cLen = sizeof(socket_temp);
    getpeername(connection, (struct sockaddr*)&socket_temp, &cLen);//用于获取与某个套接字关联的外地协议地址
	cout << "client IP:" << inet_ntoa(socket_temp.sin_addr);
	cout<< "  port:" << ntohs(socket_temp.sin_port) << endl;
	while (1)
	{
		memset(store_recv, 0, sizeof(store_recv));
		if (recv(connection, store_recv, sizeof(store_recv), 0)==0)
		{
			break;
		}
	}
    close(connection);
    close(socket_start);
    return 0;
}