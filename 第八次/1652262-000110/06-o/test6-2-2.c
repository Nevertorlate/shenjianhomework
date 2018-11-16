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
	socklen_t len_ser;
	int sock_cli = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	char buffer_in[1024];
	char buffer_out[1024] = "wo ai shen jian test6-1hellohellohello!";
    struct sockaddr_in ip_server;
	len_ser= sizeof(ip_server);
    memset(&ip_server, 0, sizeof(ip_server));
    ip_server.sin_family = AF_INET;
    ip_server.sin_port = htons(4000); 
    ip_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    while (1)
    {
        sendto(sock_cli, buffer_out, 1024, 0, (struct sockaddr *) &ip_server, len_ser);
        printf("客户端发送:%s\n",buffer_out);        
        recvfrom(sock_cli, buffer_in, 1024, 0, (struct sockaddr *) &ip_server, &len_ser);
        printf("客户端接收:%s\n",buffer_in);
        sleep(1);
    }
    close(sock_cli);
    return 0;
}
