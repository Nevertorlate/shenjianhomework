#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
int main(int argc, char* argv[])
{
	int socket_cliss = socket(AF_INET, SOCK_STREAM, 0);
	int port = atoi(argv[2]);
	int in_byte = atoi(argv[3]);
	int out_byte = atoi(argv[4]);
	int j, i_num = 0;
	char buffer_area_in[in_byte];
	char buffer_area_out[out_byte];
	struct sockaddr_in socket_server;
	memset(&socket_server, 0, sizeof(socket_server));
	socket_server.sin_family = AF_INET;
	socket_server.sin_port = htons(port);
	socket_server.sin_addr.s_addr = inet_addr(argv[1]);
	if (connect(socket_cliss, (struct sockaddr *)&socket_server, sizeof(socket_server)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
		cout << "与服务端连接成功!" << endl;
	while (1)
	{
		memset(buffer_area_in, 0, sizeof(buffer_area_in));
		for (j = 0; j < out_byte; j++)
			buffer_area_out[j] = rand() % 25 + 'a';
		i_num += sizeof(buffer_area_out);
		write(socket_cliss, buffer_area_out, sizeof(buffer_area_out));
		cout << "发送字节数:" << i_num << endl;
		read(socket_cliss, buffer_area_in, sizeof(buffer_area_in));
		if (strcmp(buffer_area_in, "exit\n") == 0)
			break;
	}
	close(socket_cliss);
	return 0;
}