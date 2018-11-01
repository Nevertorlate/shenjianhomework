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
int main(int argc ,char* argv[])
{
    int socket_client = socket(AF_INET,SOCK_STREAM, 0);
    int linking=atoi(argv[2]);
	int connection;
	char temp_string[1024];
    struct sockaddr_in server_socket;
    memset(&server_socket, 0, sizeof(server_socket));
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(linking);
    server_socket.sin_addr.s_addr = inet_addr(argv[1]);
	connection = connect(socket_client, (struct sockaddr *)&server_socket, sizeof(server_socket));
    if (connection< 0)
    {
        perror("connect");
        exit(1);
    }
	else {
		cout << "与服务端连接成功!" << endl;
	}
    while (1)
    {
		if (recv(socket_client, temp_string, sizeof(temp_string), 0) == 0)
		{
			break;
		}
		memset(temp_string, 0, sizeof(temp_string));
    }
    close(socket_client);
    return 0;
}
