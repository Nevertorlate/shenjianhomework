#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
	int port = atoi(argv[2]);	
	int j, i_num = 0;
	char buf[20];
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	if (1)
	{
		int buffer_area = 32 * 1024; //设置为32K
		setsockopt(sock_cli, SOL_SOCKET, SO_SNDBUF, (const char*)&buffer_area, sizeof(int));
	}
	if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		cout << "与服务端连接成功!" << endl;
	}

	while (1)
	{
		for (j = 0; j < 20; j++)
			buf[j] = rand() % 25 + 'a';
		i_num += 20;
		write(sock_cli, buf, sizeof(buf));
		cout << "发送字节数:" << i_num << endl;
	}
	close(sock_cli);
	return 0;
}
