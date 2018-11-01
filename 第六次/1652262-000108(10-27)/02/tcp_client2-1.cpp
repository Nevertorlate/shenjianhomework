#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc ,char* argv[])
{
    int socket_link = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[3]);
	int bind_return;
	int connection;
	char buffer_1[1024];	
	char buffer_2[1024];
	struct sockaddr_in socket_server;
	struct sockaddr_in socket_client;
    memset(&socket_server, 0, sizeof(socket_server));
    memset(&socket_client, 0, sizeof(socket_client));
    socket_server.sin_family = AF_INET;
    socket_server.sin_port = htons(port);
    socket_server.sin_addr.s_addr = inet_addr(argv[2]);
    socket_client.sin_family = AF_INET;
    socket_client.sin_port = htons(atoi(argv[1]));
    socket_client.sin_addr.s_addr = inet_addr("192.168.80.230");
	bind_return = bind(socket_link, (struct sockaddr *)&socket_client, sizeof(socket_client));
    if(bind_return==-1)
    {
        perror("bind");
        exit(1);
    }
	connection = connect(socket_link, (struct sockaddr *)&socket_server, sizeof(socket_server));
    if (connection< 0)
    {
        perror("connect");
        exit(1);
    }
	else {
		cout << "与服务端连接成功!" << endl;
	}
    while (fgets(buffer_2, sizeof(buffer_2), stdin) != NULL)
    {
		if (recv(socket_link, buffer_1, sizeof(buffer_1), 0) == 0) {
			break;
		}
        memset(buffer_1, 0, sizeof(buffer_1));
    }
    close(socket_link);
    return 0;
}
