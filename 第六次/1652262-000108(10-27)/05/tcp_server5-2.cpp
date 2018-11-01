#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
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
	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int enable = 1;	
	int port = atoi(argv[1]);	
	int j, i_num = 0;
	char string_temp[50];
	struct sockaddr_in client_addr;
	struct sockaddr_in server_sockaddr;
	if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(port);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(server_sockfd, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	socklen_t length = sizeof(client_addr);
	int connection = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
	if (connection < 0)
	{
		perror("connect");
		exit(1);
	}
	else
		cout << "与客户端连接成功!" << endl;
	while (1)
	{
		for (j = 0; j < sizeof(string_temp); j++)
			string_temp[j] = rand() % 25 + 'a';
		write(connection, string_temp, sizeof(string_temp));
		i_num += sizeof(string_temp);
		cout << "发送字节数:" << i_num << endl;
	}
	close(connection);
	close(server_sockfd);
	return 0;
}