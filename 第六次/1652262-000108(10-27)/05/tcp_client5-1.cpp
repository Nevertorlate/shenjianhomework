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
	int socket_cliss = socket(AF_INET, SOCK_STREAM, 0);
	int port = atoi(argv[2]);
	int j, i_num = 0;
	struct sockaddr_in servaddr;	
	char string_temp[20000];
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	if (connect(socket_cliss, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else {
		cout << "与服务端连接成功!" << endl;
	}
	while (1)
	{
		for (j = 0; j < 20000; j++)
			string_temp[j] = rand() % 25 + 'a';
		i_num += sizeof(string_temp);
		write(socket_cliss, string_temp, sizeof(string_temp));
		cout << "发送字节数:" << i_num << endl;
		sleep(1);
	}
	close(socket_cliss);
	return 0;
}