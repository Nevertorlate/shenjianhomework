#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
int ip_client1;
int ip_client2;
char buffer_client[15];
char buffer_read[100];
void SendLarge(int signo)
{
	int i;
	send(ip_client1, buffer_client, sizeof(buffer_client), MSG_DONTWAIT);
	send(ip_client2, buffer_client, sizeof(buffer_client), MSG_DONTWAIT);
	cout << "send内容:";
	for (i = 0; i < sizeof(buffer_client); i++) {
		cout << buffer_client[i];
	}
	cout << endl;
	alarm(3);
}
int main(int argc, char* argv[])
{
	int counter;
	int flg_client1;
	int flg_client2;
	int linkplace_client1 = atoi(argv[2]);
	int linkplace_client2 = atoi(argv[3]);
	struct sockaddr_in ip_server1;
	struct sockaddr_in ip_server2;
	ip_client1 = socket(AF_INET, SOCK_STREAM, 0);
	ip_client2 = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ip_server1, 0, sizeof(ip_server1));
	ip_server1.sin_family = AF_INET;
	ip_server1.sin_port = htons(linkplace_client1);
	ip_server1.sin_addr.s_addr = inet_addr(argv[1]);
	memset(&ip_server2, 0, sizeof(ip_server2));
	ip_server2.sin_family = AF_INET;
	ip_server2.sin_port = htons(linkplace_client2);
	ip_server2.sin_addr.s_addr = inet_addr(argv[1]);
	if (signal(SIGALRM, SendLarge) == SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	setbuf(stdout, NULL);
	flg_client1 = fcntl(ip_client1, F_GETFL, 0);
	fcntl(ip_client1, F_SETFL, flg_client1 | O_NONBLOCK);
	flg_client2 = fcntl(ip_client2, F_GETFL, 0);
	fcntl(ip_client2, F_SETFL, flg_client1 | O_NONBLOCK);
	fd_set  fd_linkclient1, fd_linkclient2;
	FD_ZERO(&fd_linkclient1);
	FD_SET(ip_client1, &fd_linkclient1);
	FD_ZERO(&fd_linkclient2);
	FD_SET(ip_client2, &fd_linkclient2);
	switch (select(ip_client1 + 1, &fd_linkclient1, NULL, NULL, 0))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("超时\n");
		break;
	default:
		connect(ip_client1, (struct sockaddr *)&ip_server1, sizeof(ip_server1));
	}
	cout << "服务端1号连接成功!" << endl;
	switch (select(ip_client2 + 1, &fd_linkclient2, NULL, NULL, 0))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("超时\n");
		break;
	default:
		connect(ip_client2, (struct sockaddr *)&ip_server2, sizeof(ip_server2));
	}
	cout << "服务端2号连接成功!" << endl;
	for (counter = 0; counter < 15; counter++) {
		buffer_client[counter] = rand() % 25 + 'a';
	}
	alarm(3);
	while (1)
	{
		FD_ZERO(&fd_linkclient1);
		FD_SET(ip_client1, &fd_linkclient1);
		switch (select(ip_client1 + 1, &fd_linkclient1, NULL, NULL, 0))
		{
		case 0:
			break;
		default:
			memset(buffer_read, 0, sizeof(buffer_read));
			int len = recv(ip_client1, buffer_read, sizeof(buffer_read), MSG_DONTWAIT);
			if (len > 0)
			{
				cout << "1号服务端recv内容";
				for (counter = 0; counter < len; counter++) {
					cout << buffer_read[counter];
				}
				cout << endl;
			}
		}
		FD_ZERO(&fd_linkclient2);
		FD_SET(ip_client2, &fd_linkclient2);
		switch (select(ip_client2 + 1, &fd_linkclient2, NULL, NULL, 0))
		{
		case 0:
			break;
		default:
			memset(buffer_read, 0, sizeof(buffer_read));
			int len = recv(ip_client2, buffer_read, sizeof(buffer_read), MSG_DONTWAIT);
			if (len > 0)
			{
				cout << "2号服务端recv内容:";
				for (counter = 0; counter < len; counter++) {
					cout << buffer_read[counter];
				}
				cout << endl;
			}
		}
	}
	close(ip_client1);
	close(ip_client2);
	return 0;
}