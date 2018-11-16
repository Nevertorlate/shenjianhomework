#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
using namespace std;
#define BUFFER_SIZE 1024
int main(int argc ,char* argv[])
{
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    else
	cout<<"与服务端连接成功!"<<endl;
    int flags1 = fcntl(sock_cli, F_GETFL, 0);
    fcntl(sock_cli, F_SETFL, flags1 | O_NONBLOCK);
    char recvbuf[BUFFER_SIZE];
    while (1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        break;
    }
    close(sock_cli);
    return 0;
}
