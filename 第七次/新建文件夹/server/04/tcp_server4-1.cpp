#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
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
	int accept_return;
	int flg_client1;
	int flg_client2;
	int flg_en = 1;
	int linkplace = atoi(argv[1]);
	socklen_t socklen_cli;
	int ip_sersock = socket(AF_INET, SOCK_STREAM, 0);
	char buffer_temp[50000];
	struct sockaddr_in ip_cli;
	struct sockaddr_in ip_server;
	if (setsockopt(ip_sersock, SOL_SOCKET, SO_REUSEADDR, &flg_en, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	cout << "套接字设置成功!" << endl;
	ip_server.sin_family = AF_INET;
	ip_server.sin_port = htons(linkplace);
	ip_server.sin_addr.s_addr = htonl(INADDR_ANY);
	flg_client1 = fcntl(ip_sersock, F_GETFL, 0);
	fcntl(ip_sersock, F_SETFL, flg_client1 | O_NONBLOCK);
	if (bind(ip_sersock, (struct sockaddr *)&ip_server, sizeof(ip_server)) == -1)
	{
		perror("bind");
		exit(1);
	}
	cout << "bind成功!" << endl;
	if (listen(ip_sersock, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	cout << "listen成功!" << endl;
	socklen_cli = sizeof(ip_cli);
	fd_set  fd_link;
	FD_ZERO(&fd_link);
	FD_SET(ip_sersock, &fd_link);
	switch (select(ip_sersock + 1, &fd_link, NULL, NULL, NULL))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("与客户端连接超时\n");
		break;
	default:
		accept_return = accept(ip_sersock, (struct sockaddr*)&ip_cli, &socklen_cli);
	}
	cout << "与客户端连接成功!" << endl;
	getchar();
	flg_client2 = fcntl(accept_return, F_GETFL, 0);
	fcntl(accept_return, F_SETFL, flg_client2 | O_NONBLOCK);
	while (1)
	{
		memset(buffer_temp, 0, sizeof(buffer_temp));
		read(accept_return, buffer_temp, sizeof(buffer_temp));
	}
	close(accept_return);
	close(ip_sersock);
	return 0;
}