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
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc, char* argv[])
{
	int socket_serfd = socket(AF_INET, SOCK_STREAM, 0);
	int flag_en = 1;	
	int adr_in = atoi(argv[1]);
	struct sockaddr_in socket_ser;
	char string_temp[20];
	struct sockaddr_in socket_client;
	if (setsockopt(socket_serfd, SOL_SOCKET, SO_REUSEADDR, &flag_en, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	socket_ser.sin_family = AF_INET;
	socket_ser.sin_port = htons(adr_in);
	socket_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(socket_serfd, (struct sockaddr *)&socket_ser, sizeof(socket_ser)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(socket_serfd, 20) == -1)
	{
		perror("listen");
		exit(1);
	}

	socklen_t length = sizeof(socket_client);
	int connection = accept(socket_serfd, (struct sockaddr*)&socket_client, &length);
	if (connection < 0)
	{
		perror("connect");
		exit(1);
	}
	else {
		cout << "与客户端连接成功!" << endl;
	}
	while (1)
	{
		memset(string_temp, 0, sizeof(string_temp));
		read(connection, string_temp, 20);
		if (strcmp(string_temp, "exit\n") == 0)
			break;
		int i;
		for (i = 0; i < sizeof(string_temp); i++)
			cout << string_temp[i];
		cout << endl;
		write(connection, string_temp, sizeof(string_temp));
	}
	close(connection);
	close(socket_serfd);
	return 0;
}
