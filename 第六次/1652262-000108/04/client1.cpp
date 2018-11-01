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
//#define port  8887
#define BUFFER_SIZE 30
#define per_size 30

int main(int argc ,char* argv[])
{
    ///����sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    ///����sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  ///�������˿�
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  ///������ip

    ///���ӷ��������ɹ�����0�����󷵻�-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    else
	cout<<"���ӳɹ�!"<<endl;
    char recvbuf[BUFFER_SIZE],buf[per_size];
    int i,j;
    while (1)
    {
        for(i=0;i<99;i++)
        {
            for(j=0;j<sizeof(buf);j++)
                buf[j]=rand()%10+'0';
            write(sock_cli, buf, sizeof(buf)); ///����
            cout<<"����:";
            for(j=0;j<sizeof(buf);j++)
                cout<<buf[j];
            cout<<endl;
            sleep(1);
            recv(sock_cli, recvbuf, sizeof(recvbuf),MSG_DONTWAIT); ///����
    	    if(strcmp(recvbuf,"exit\n")==0)
                break;        
    	    fputs(recvbuf, stdout);
            memset(recvbuf, 0, sizeof(recvbuf));
        }
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(buf, 0, sizeof(buf));
    }

    close(sock_cli);
    return 0;
}
