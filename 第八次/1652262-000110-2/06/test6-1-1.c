#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
int main(int argc,char* argv[])
{
    pid_t pid_cli;
    pid_cli=fork();
	if (pid_cli == -1 || pid_cli > 0) {
		return 0;
	}
    int counter;
    int flg_en = 1;
	int connection_return;
	int fd_sockser = socket(AF_INET,SOCK_STREAM, 0);
	socklen_t cli_len;
	char buffer_in[1024];
	char buffer_out[1024] = "wo ai shen jian test6-1!";
	struct sockaddr_in ip_cli;
	struct sockaddr_in ip_ser;
	if (setsockopt(fd_sockser, SOL_SOCKET, SO_REUSEADDR, &flg_en, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	printf("setsockopt success\n");
    ip_ser.sin_family = AF_INET;
    ip_ser.sin_port = htons(4000);
    ip_ser.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd_sockser,(struct sockaddr *)&ip_ser,sizeof(ip_ser))==-1)
    {
        perror("bind");
        exit(1);
    }
	printf("bind success\n");
    if(listen(fd_sockser,20) == -1)
    {
        perror("listen");
        exit(1);
    }
	printf("listen success\n");
    cli_len = sizeof(ip_cli);
     connection_return = accept(fd_sockser, (struct sockaddr*)&ip_cli, &cli_len);
    if(connection_return<0)
    {
        perror("connect");
        exit(1);
    }
	else {
		printf("与客户端连接成功!\n");
	}
    
    while(1)
    {
        write(connection_return,buffer_out,sizeof(buffer_out));
        printf("服务端发送内容:%s\n",buffer_out);        
        read(connection_return,buffer_in,sizeof(buffer_in));
        printf("服务端读取内容:%s\n",buffer_in);
        sleep(1);
    }
    close(connection_return);
    close(fd_sockser);
    return 0;
}
