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
#define BUFFER_SIZE 10
//#define TIMEOUT 10
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

    int flags1 = fcntl(sock_cli, F_GETFL, 0);                        //��ȡ�ļ���flags1ֵ��
    fcntl(sock_cli, F_SETFL, flags1 | O_NONBLOCK);   //���óɷ�����ģʽ��

    fd_set  fdR; 
    FD_ZERO(&fdR); 
    FD_SET(sock_cli, &fdR); 
    switch (select(sock_cli + 1, &fdR, NULL,NULL, NULL)) //������connect
    {
        case -1:
            perror("select");
            break;/* ��˵��select�������� */
        case 0:
            sleep(1);
            printf("��ʱ\n");
            break; /* ˵�����趨��ʱ���ڣ�socket��״̬û�з����仯 */
        default:
            connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    cout<<"���ӳɹ�!"<<endl;

    char sendbuf[BUFFER_SIZE];
    int num;
    for(num=0;num<10;num++)
    {
        sendbuf[num]='a'+rand()%25;
    }
    sendbuf[10]='d';
    // struct  timeval timeout;
    // timeout.tv_sec=TIMEOUT;
    // timeout.tv_usec=0;
    while(1)
    {
        // FD_ZERO(&fdR); 
        // FD_SET(sock_cli, &fdR); 
        // switch (select(sock_cli + 1, &fdR, NULL,NULL,0)) //������recv
        // {
        //     case -1:
        //         perror("select");
        //         break;
        //     case 0:
        //         sleep(1);
        //         printf("��ʱ\n");
        //         break; /* ˵�����趨��ʱ���ڣ�socket��״̬û�з����仯 */
        //     default:
                send(sock_cli, sendbuf, sizeof(sendbuf),MSG_DONTWAIT);
                sleep(1);
        // }
    }
    close(sock_cli);
    return 0;
}

