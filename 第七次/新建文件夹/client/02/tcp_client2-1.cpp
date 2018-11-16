#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int count;
	int flg_async;
	int linkplace = atoi(argv[2]);
	int ip_sock = socket(AF_INET, SOCK_STREAM, 0);
	char buffer[100];
	struct sockaddr_in ip_ser;
	memset(&ip_ser, 0, sizeof(ip_ser));
	ip_ser.sin_family = AF_INET;
	ip_ser.sin_port = htons(linkplace);
	ip_ser.sin_addr.s_addr = inet_addr(argv[1]);
	flg_async = fcntl(ip_sock, F_GETFL, 0);
	fcntl(ip_sock, F_SETFL, flg_async | O_NONBLOCK);
	fd_set  fd_link;
	FD_ZERO(&fd_link);
	FD_SET(ip_sock, &fd_link);
	switch (select(ip_sock + 1, &fd_link, NULL, NULL, NULL))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("与服务端连接超时!\n");
		break;
	default:
		connect(ip_sock, (struct sockaddr *)&ip_ser, sizeof(ip_ser));
	}
	cout << "与服务端连接成功!" << endl;
	for (count = 0; count < 10; count++)
	{
		buffer[count] = 'a' + rand() % 25;
	}
	while (1)
	{
		send(ip_sock, buffer, sizeof(buffer), MSG_DONTWAIT);
		sleep(1);
	}
	close(ip_sock);
	return 0;
}