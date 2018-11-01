#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc ,char* argv[])
{
    struct sockaddr_in mysock;
    int link_duankou=atoi(argv[2]);
	int sock_fd = socket(AF_INET,SOCK_STREAM, 0);
	char client_out[1024];
	char client_in[1024];
    memset(&mysock, 0, sizeof(mysock));
    mysock.sin_family = AF_INET;
    mysock.sin_port = htons(link_duankou);
    mysock.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(sock_fd, (struct sockaddr *)&mysock, sizeof(mysock)) < 0)
    {
        perror("connect");
        exit(1);
    }
	else {
		cout << "与服务端连接成功!" << endl;
	}
    while (fgets(client_out, sizeof(client_out), stdin) != NULL)
    {
        send(sock_fd, client_out, strlen(client_out),0);
        recv(sock_fd, client_in, sizeof(client_in),0);
		if (strcmp(client_in, "exit\n") == 0)
		{
			break;
		}
		fputs(client_in, stdout);
        memset(client_out, 0, sizeof(client_out));
        memset(client_in, 0, sizeof(client_in));
    }
    close(sock_fd);
    return 0;
}
