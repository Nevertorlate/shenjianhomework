#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int serverfd;
	int flag = 1;
	int connection;
	int link_duankou = atoi(argv[1]);
	char string_recv[1024];
	struct sockaddr_in tcpserveraddr;
	struct sockaddr_in client_addr;
	serverfd= socket(AF_INET, SOCK_STREAM, 0);
	int error = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	if (error< 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	tcpserveraddr.sin_family = AF_INET;
	tcpserveraddr.sin_port = htons(link_duankou);
	tcpserveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int bind_return = bind(serverfd, (struct sockaddr *)&tcpserveraddr, sizeof(tcpserveraddr));
	if (bind_return== -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(serverfd, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	socklen_t length = sizeof(client_addr);
	connection = accept(serverfd, (struct sockaddr*)&client_addr, &length);
	if (connection >= 0)
	{
		cout << "与客户端连接成功!" << endl;
	}
	else {
		perror("connect");
		exit(1);
	}
	while (1)
	{
		memset(string_recv, 0, sizeof(string_recv));
		if (recv(connection, string_recv, sizeof(string_recv), 0) == 0)
		{
			break;
		}
	}
	close(connection);
	close(serverfd);
	return 0;
}