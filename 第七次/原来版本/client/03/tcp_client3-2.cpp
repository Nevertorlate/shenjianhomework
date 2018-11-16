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
#include <time.h>
#include <signal.h>

using namespace std;
#define BUFFER_SIZE 100
#define SEND_SIZE 15
#define TIMEOUT 3

char send_buf[SEND_SIZE],buffer[BUFFER_SIZE];
int sock_cli;

void CbSigAlrm(int signo)
{
    send(sock_cli, send_buf, sizeof(send_buf),MSG_DONTWAIT);
    cout<<"send����:";
    int i;
    for(i=0;i<sizeof(send_buf);i++)
        cout<<send_buf[i];
    cout<<endl;
    alarm(TIMEOUT);
}


int main(int argc ,char* argv[])
{
    ///����sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    ///����sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  ///�������˿�
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  ///������ip

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
    perror("signal");
    return 0;
    }
    //�رձ�׼������л���ģʽ
    setbuf(stdout,NULL);

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
            printf("���������ӳ�ʱ\n");
            break; /* ˵�����趨��ʱ���ڣ�socket��״̬û�з����仯 */
        default:
            connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    cout<<"���������ӳɹ�!"<<endl;

    int num;
    for(num=0;num<SEND_SIZE;num++)
        send_buf[num]=rand()%25+'a';
    alarm(TIMEOUT);

    while(1)
    {
        FD_ZERO(&fdR); 
        FD_SET(sock_cli, &fdR); 
        switch (select(sock_cli + 1, &fdR, NULL,NULL,NULL)) //������recv
        {
            case 0:
                break; /* ˵�����趨��ʱ���ڣ�socket��״̬û�з����仯 */
            default:
                memset(buffer,0,sizeof(buffer));
                int len=recv(sock_cli, buffer, sizeof(buffer),MSG_DONTWAIT);
                if(len>0)
                {
                    cout<<"recv����:";
                    for(num=0;num<len;num++)
                        cout<<buffer[num];
                    cout<<endl;
                }
       }
    }
    close(sock_cli);
    return 0;
}

