#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int flg_async;
	int linkplace = atoi(argv[2]);
	int client_link = socket(AF_INET, SOCK_STREAM, 0);
	char buffer_temp[1024];
	struct sockaddr_in server_ip;
	memset(&server_ip, 0, sizeof(server_ip));
	server_ip.sin_family = AF_INET;
	server_ip.sin_linkplace = htons(linkplace);
	server_ip.sin_addr.s_addr = inet_addr(argv[1]);
	if (connect(client_link, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else {
		cout << "与服务端连接成功!" << endl;
	}
	flg_async = fcntl(client_link, F_GETFL, 0);
	fcntl(client_link, F_SETFL, flg_async | O_NONBLOCK);
	while (1)
	{
		memset(buffer_temp, 0, sizeof(buffer_temp));
		recv(client_link, buffer_temp, sizeof(buffer_temp), MSG_DONTWAIT);
		break;
	}
	close(client_link);
	return 0;
}
