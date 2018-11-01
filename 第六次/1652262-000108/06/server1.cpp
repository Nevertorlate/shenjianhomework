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
#define QUEUE   20
int main(int argc,char* argv[])
{
    ///����sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    ///����sockaddr_in
    int port=atoi(argv[1]);
    int read_num=atoi(argv[2]);
    int write_num=atoi(argv[3]);
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
    char buf_r[read_num];
    char buf_w[write_num];
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
    
    int j,i_num=0;
    
    while(1)
    {
        memset(buf_r,0,sizeof(buf_r));
        read(conn,buf_r,sizeof(buf_r));
        for(j=0;j<write_num;j++)
            buf_w[j]=rand()%10+'0';
        i_num+=sizeof(buf_w);
        write(conn,buf_w,sizeof(buf_w));
        cout<<"�����ֽ���:"<<i_num<<endl;
        if(strcmp(buf_r,"exit\n")==0)
            break;
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
