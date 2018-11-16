#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc ,char* argv[])
{
    pid_t pid_cli;
    pid_cli=fork();
	if (pid_cli == -1 || pid_cli > 0) {
		return 0;
	}
	int counter;
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
	char buffer_in[1024];
	char buffer_out[1024] = "wo ai shen jian test6-1!";
    struct sockaddr_in ip_server;
    memset(&ip_server, 0, sizeof(ip_server));
    ip_server.sin_family = AF_INET;
    ip_server.sin_port = htons(4000); 
    ip_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sock_cli, (struct sockaddr *)&ip_server, sizeof(ip_server)) < 0)
    {
        perror("connect");
        exit(1);
    }
	else {
		printf("与客户端连接成功\n");
	}
    while (1)
    {
        write(sock_cli,buffer_out,sizeof(buffer_out));
        printf("客户端发送内容:%s\n",buffer_out);        
        read(sock_cli,buffer_in,sizeof(buffer_in));
        printf("客户端读取内容:%s\n",buffer_in);
        sleep(1);
    }
    close(sock_cli);
    return 0;
}
