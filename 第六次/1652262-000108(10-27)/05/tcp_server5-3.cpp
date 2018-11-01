#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int socket_sers = socket(AF_INET, SOCK_STREAM, 0);
	int enable = 1;
	int nRecvBuf = 32 * 1024;	
	int port = atoi(argv[1]);
	struct sockaddr_in socket_ser;
	struct sockaddr_in socket_clis;
	char string_temp[50000];
	if (setsockopt(socket_sers, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	socket_ser.sin_family = AF_INET;
	socket_ser.sin_port = htons(port);
	socket_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	setsockopt(socket_sers, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	if (bind(socket_sers, (struct sockaddr *)&socket_ser, sizeof(socket_ser)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(socket_sers, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	socklen_t length = sizeof(socket_clis);
	int connection = accept(socket_sers, (struct sockaddr*)&socket_clis, &length);
	if (connection < 0)
	{
		perror("connect");
		exit(1);
	}
	else {
		cout << "与客户端连接成功!" << endl;
	}
	getchar();//设置阻塞
	while (1)
	{
		memset(string_temp, 0, sizeof(string_temp));
		read(connection, string_temp, sizeof(string_temp));
		if (strcmp(string_temp, "exit\n") == 0)
			break;
	}
	close(connection);
	close(socket_sers);
	return 0;
}