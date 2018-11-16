#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
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
int main(int argc, char* argv[])
{
	int return_connection;
	int fl_fcntl1;
	int fl_fcntl2;
	int flag_en = 1;
	int linkplace = atoi(argv[1]);
	int linksock_ser1 = socket(AF_INET, SOCK_STREAM, 0);
	char buffer[10];
	socklen_t slen_sockcli;
	struct sockaddr_in ip_cli;
	struct sockaddr_in ip_ser;
	if (setsockopt(linksock_ser1, SOL_SOCKET, SO_REUSEADDR, &flag_en, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	ip_ser.sin_family = AF_INET;
	ip_ser.sin_port = htons(linkplace);
	ip_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	fl_fcntl1 = fcntl(linksock_ser1, F_GETFL, 0);
	fcntl(linksock_ser1, F_SETFL, fl_fcntl1 | O_NONBLOCK);
	if (bind(linksock_ser1, (struct sockaddr *)&ip_ser, sizeof(ip_ser)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(linksock_ser1, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	slen_sockcli = sizeof(ip_cli);
	fd_set  fd_link;
	FD_ZERO(&fd_link);
	FD_SET(linksock_ser1, &fd_link);
	switch (select(linksock_ser1 + 1, &fd_link, NULL, NULL, NULL))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("与客户端连接超时!\n");
		break;
	default:
		return_connection = accept(linksock_ser1, (struct sockaddr*)&ip_cli, &slen_sockcli);
	}
	cout << "与客户端连接成功!!" << endl;
	fl_fcntl2 = fcntl(return_connection, F_GETFL, 0), num;
	fcntl(return_connection, F_SETFL, fl_fcntl2 | O_NONBLOCK);
	while (1)
	{
		FD_ZERO(&fd_link);
		FD_SET(return_connection, &fd_link);
		switch (select(return_connection + 1, &fd_link, NULL, NULL, NULL))
		{
		case -1:
			perror("select");
			break;
		case 0:
			sleep(1);
			printf("与客户端连接超时!\n");
			break;
		default:
			memset(buffer, 0, sizeof(buffer));
			int len = recv(return_connection, buffer, sizeof(buffer), MSG_DONTWAIT);
			if (len > 0)
			{
				for (num = 0; num < len; num++) {
					cout << buffer[num];
				}
				cout << endl;
			}
		}
	}
	close(return_connection);
	close(linksock_ser1);
	return 0;
}