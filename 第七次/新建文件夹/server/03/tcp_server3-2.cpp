#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>
using namespace std;
int counter;
int return_connection;
char buffer[88];//缓冲区内容
char buffer_temp[10];//发送字节存储
void SendLarge(int signo)
{
	send(return_connection, buffer_temp, sizeof(buffer_temp), MSG_DONTWAIT);
	cout << "send内容为:";
	int i;
	for (i = 0; i < sizeof(buffer_temp); i++) {
		cout << buffer_temp[i];
	}
	cout << endl;
	alarm(1);
}
int main(int argc, char* argv[])
{
	int ip_sersock = socket(AF_INET, SOCK_STREAM, 0);
	int flg_async1;
	int flg_async2;
	int flg_en = 1;
	int linkplace = atoi(argv[1]);
	socklen_t recv_len;
	struct sockaddr_in ip_cli;
	struct sockaddr_in ip_ser;
	if (setsockopt(ip_sersock, SOL_SOCKET, SO_REUSEADDR, &flg_en, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	ip_ser.sin_family = AF_INET;
	ip_ser.sin_port = htons(linkplace);
	ip_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	if (signal(SIGALRM, SendLarge) == SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	setbuf(stdout, NULL);
	flg_async1 = fcntl(ip_sersock, F_GETFL, 0);
	fcntl(ip_sersock, F_SETFL, flg_async1 | O_NONBLOCK);
	if (bind(ip_sersock, (struct sockaddr *)&ip_ser, sizeof(ip_ser)) == -1)
	{
		perror("bind");
		exit(1);
	}
	printf("bind成功\n");
	if (listen(ip_sersock, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("listen成功\n");
	for (counter = 0; counter < 10; counter++) {
		buffer_temp[counter] = rand() % 25 + 'a';
	}
	recv_len = sizeof(ip_cli);
	fd_set  fdR;
	FD_ZERO(&fdR);
	FD_SET(ip_sersock, &fdR);
	switch (select(ip_sersock + 1, &fdR, NULL, NULL, NULL)) //非阻塞connect
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("与客户端连接超时\n");
		break;
	default:
		return_connection = accept(ip_sersock, (struct sockaddr*)&ip_cli, &recv_len);
	}
	cout << "与客户端连接成功!" << endl;
	flg_async2 = fcntl(return_connection, F_GETFL, 0);
	fcntl(return_connection, F_SETFL, flg_async2 | O_NONBLOCK);
	alarm(1);
	while (1)
	{
		FD_ZERO(&fdR);
		FD_SET(return_connection, &fdR);
		switch (select(return_connection + 1, &fdR, NULL, NULL, NULL))
		{
		case 0:
			break;
		default:
			memset(buffer, 0, sizeof(buffer));
			int len = recv(return_connection, buffer, sizeof(buffer), MSG_WAITALL);
			if (len > 0)
			{
				cout << "recv内容为:";
				for (counter = 0; counter < len; counter++) {
					cout << buffer[counter];
				}
				cout << endl;
			}
		}
	}
	close(return_connection);
	close(ip_sersock);
	return 0;
}
