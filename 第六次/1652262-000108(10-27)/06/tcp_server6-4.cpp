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
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
int main(int argc, char* argv[])
{
	int socket_sers = socket(AF_INET, SOCK_STREAM, 0);
	int enable = 1;
	int j, i_num = 0;
	int connection;
	int port = atoi(argv[1]);
	int in_byte = atoi(argv[2]);
	int out_byte = atoi(argv[3]);
	char buffer_area_in[in_byte];
	char buffer_area_out[out_byte];
	struct sockaddr_in socket_client;
	struct sockaddr_in socket_ser;
	if (setsockopt(socket_sers, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	socket_ser.sin_family = AF_INET;
	socket_ser.sin_port = htons(port);
	socket_ser.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(socket_sers, (struct sockaddr *)&socket_ser, sizeof(socket_ser)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(socket_sers, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	socklen_t length = sizeof(socket_client);
	connection = accept(socket_sers, (struct sockaddr*)&socket_client, &length);
	if (connection < 0)
	{
		perror("connect");
		exit(1);
	}
	else
		cout << "与客户端连接成功!" << endl;
	while (1)
	{
		memset(buffer_area_in, 0, sizeof(buffer_area_in));
		for (j = 0; j < out_byte; j++)
			buffer_area_out[j] = rand() % 25 + 'a';
		i_num += sizeof(buffer_area_out);
		write(connection, buffer_area_out, sizeof(buffer_area_out));
		cout << "发送字节数:" << i_num << endl;
		read(connection, buffer_area_in, sizeof(buffer_area_in));
		if (strcmp(buffer_area_in, "exit\n") == 0)
			break;
	}
	close(connection);
	close(socket_sers);
	return 0;
}