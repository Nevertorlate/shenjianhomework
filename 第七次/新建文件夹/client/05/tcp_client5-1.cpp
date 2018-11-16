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
int ip_client;
char buffer_out[15];
char buffer_temp[100];
void SendLarge(int signo)
{
	send(ip_client, buffer_out, sizeof(buffer_out), MSG_DONTWAIT);
	cout << "send内容:";
	int i;
	for (i = 0; i < sizeof(buffer_out); i++) {
		cout << buffer_out[i];
	}
	cout << endl;
	alarm(3);
}
int main(int argc, char* argv[])
{
	int counter;
	int flg_client;
	int linkplace = atoi(argv[2]);
	struct sockaddr_in ip_server;
	ip_client = socket(AF_INET, SOCK_STREAM, 0);
	memset(&ip_server, 0, sizeof(ip_server));
	ip_server.sin_family = AF_INET;
	ip_server.sin_port = htons(linkplace);
	ip_server.sin_addr.s_addr = inet_addr(argv[1]);
	if (signal(SIGALRM, SendLarge) == SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	setbuf(stdout, NULL);
	flg_client = fcntl(ip_client, F_GETFL, 0);
	fcntl(ip_client, F_SETFL, flg_client | O_NONBLOCK);
	fd_set  fd_link;
	FD_ZERO(&fd_link);
	FD_SET(ip_client, &fd_link);
	switch (select(ip_client + 1, &fd_link, NULL, NULL, 0))
	{
	case -1:
		perror("select");
		break;
	case 0:
		sleep(1);
		printf("与服务端连接超时\n");
		break;
	default:
		connect(ip_client, (struct sockaddr *)&ip_server, sizeof(ip_server));
	}
	cout << "与服务端连接成功!" << endl;
	for (counter = 0; counter < 15; counter++) {
		buffer_out[counter] = rand() % 25 + 'a';
	}
	alarm(3);
	while (1)
	{
		FD_ZERO(&fd_link);
		FD_SET(ip_client, &fd_link);
		switch (select(ip_client + 1, &fd_link, NULL, NULL, 0)) //非阻塞recv
		{
		case 0:
			break;
		default:
			memset(buffer_temp, 0, sizeof(buffer_temp));
			int len = recv(ip_client, buffer_temp, sizeof(buffer_temp), MSG_DONTWAIT);
			if (len > 0)
			{
				cout << "recv内容:";
				for (counter = 0; counter < len; counter++) {
					cout << buffer_temp[counter];
				}
				cout << endl;
			}
		}
	}
	close(ip_client);
	return 0;
}