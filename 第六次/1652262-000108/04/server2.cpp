#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <arpa/inet.h>
using namespace std;
//#define port  8887
#define QUEUE   20
#define BUFFER_SIZE 20
#define maxbit 20
int main(int argc,char* argv[])
{
    ///����sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	perror("setsockopt(SO_REUSEADDR) failed");
    ///����sockaddr_in
    int port=atoi(argv[1]);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind���ɹ�����0��������-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen���ɹ�����0��������-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///�ͻ����׽���
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///�ɹ����طǸ������֣�������-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }
    else
	cout<<"���ӳɹ�!"<<endl;
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        recv(conn, buffer, maxbit,0);
        if(strcmp(buffer,"exit\n")==0)
            break;
        int i;
	for(i=0;i<sizeof(buffer);i++)
	    cout<<buffer[i];
	cout<<endl;
        send(conn, buffer, sizeof(buffer), 0);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}

