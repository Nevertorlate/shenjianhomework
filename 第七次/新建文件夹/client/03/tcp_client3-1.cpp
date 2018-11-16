#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int ip_clientsock;
char buffer_temp[15];
char store_buf[100];
void SendLarge(int signo)
{
	send(ip_clientsock, buffer_temp, sizeof(buffer_temp), MSG_DONTWAIT);
	cout << "send内容:";
	for (int i = 0; i < sizeof(buffer_temp); i++) {
		cout << buffer_temp[i];
	}
	cout << endl;
	alarm(3);
}
int main(int argc, char* argv[])
{
	int counter;
	int flg_async;
	int linkplace = atoi(argv[2]);
	struct sockaddr_in ip_ser;
	ip_clientsock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ip_ser, 0, sizeof(ip_ser));
	ip_ser.sin_family = AF_INET;
	ip_ser.sin_port = htons(linkplace);
	ip_ser.sin_addr.s_addr = inet_addr(argv[1]);
	if (signal(SIGALRM, SendLarge) == SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	setbuf(stdout, NULL);
	flg_async = fcntl(ip_clientsock, F_GETFL, 0);
	fcntl(ip_clientsock, F_SETFL, flg_async | O_NONBLOCK);
	fd_set  fd_link;
	FD_ZERO(&fd_link);
	FD_SET(ip_clientsock, &fd_link);
	switch (select(ip_clientsock + 1, &fd_link, NULL, NULL, NULL))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("非阻塞连接超时\n");
		break;
	default:
		connect(ip_clientsock, (struct sockaddr *)&ip_ser, sizeof(ip_ser));
	}
	cout << "与服务端连接成功!" << endl;
	for (counter = 0; counter < 15; counter++) {
		buffer_temp[counter] = rand() % 25 + 'a';
	}
	alarm(3);
	while (1)
	{
		FD_ZERO(&fd_link);
		FD_SET(ip_clientsock, &fd_link);
		switch (select(ip_clientsock + 1, &fd_link, NULL, NULL, NULL))
		{
		case 0:
			break;
		default:
			memset(store_buf, 0, sizeof(store_buf));
			int return_recv = recv(ip_clientsock, store_buf, sizeof(store_buf), MSG_DONTWAIT);
			if (return_recv > 0)
			{
				cout << "recv内容:";
				for (counter = 0; counter < return_recv; counter++) {
					cout << store_buf[counter];
				}
				cout << endl;
			}
		}
	}
	close(ip_clientsock);
	return 0;
}