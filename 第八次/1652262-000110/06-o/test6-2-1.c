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
    int flg_en = 1;
	int counter, i_num = 0;
	int fd_sockser = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	socklen_t cli_len;
	socklen_t len_ser;
	char buffer_in[1024];
	char buffer_out[1024] = "wo ai shen jian test6-1hellohellohello";
	struct sockaddr_in ip_ser;
	struct sockaddr_in ip_cli;
	if (setsockopt(fd_sockser, SOL_SOCKET, SO_REUSEADDR, &flg_en, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	printf("setsockopt success\n");
    len_ser = sizeof(ip_cli);
    ip_ser.sin_family = AF_INET;
    ip_ser.sin_port = htons(4000);
    ip_ser.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd_sockser,(struct sockaddr *)&ip_ser,sizeof(ip_ser))==-1)
    {
        perror("bind");
        exit(1);
    }
	printf("bind success\n");
	cli_len = sizeof(ip_cli);
    while(1)
    {
        sendto(fd_sockser, buffer_out, 1024, 0, (struct sockaddr *) &ip_cli, len_ser);
        printf("服务端发送:%s\n",buffer_out);        
        recvfrom(fd_sockser, buffer_in, 1024, 0, (struct sockaddr *) &ip_cli, &len_ser);
        printf("服务端接收:%s\n",buffer_in);
        sleep(1);
    }
    close(fd_sockser);
    return 0;
}
