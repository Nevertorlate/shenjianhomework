#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
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
	int return_connection;
	int fl_fcntl2;
	int flag_en = 1;
	int linkplace = atoi(argv[1]);
	int linksock_ser1 = socket(AF_INET, SOCK_STREAM, 0);
	socklen_t slen_sockcli;
	char sendbuf[1024];
	struct sockaddr_in ip_ser;
	struct sockaddr_in ip_clisock;
	if (setsockopt(linksock_ser1, SOL_SOCKET, SO_REUSEADDR, &flag_en, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	ip_ser.sin_family = AF_INET;
	ip_ser.sin_port = htons(linkplace);
	ip_ser.sin_addr.s_addr = htonl(INADDR_ANY);
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
	slen_sockcli = sizeof(ip_clisock);
	return_connection = accept(linksock_ser1, (struct sockaddr*)&ip_clisock, &slen_sockcli);
	fl_fcntl2 = fcntl(return_connection, F_GETFL, 0);
	fcntl(return_connection, F_SETFL, fl_fcntl2 | O_NONBLOCK);
	if (return_connection < 0)
	{
		perror("connect");
		exit(1);
	}
	else {
		cout << "与客户端与客户端连接成功!" << endl;
	}
	while (1)
	{
		memset(sendbuf, 0, sizeof(sendbuf));
		if (recv(return_connection, sendbuf, sizeof(sendbuf), 0) == 0)
		{
			break;
		}
	}
	close(return_connection);
	close(linksock_ser1);
	return 0;
}