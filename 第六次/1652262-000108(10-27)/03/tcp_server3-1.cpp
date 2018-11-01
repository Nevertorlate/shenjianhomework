#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int get_local_ip() 
{
	//配合宏SIOCGIFHWADDR、SIOCGIFADDR、SIOCGIFBRDADDR与SIOCGIFNETMASK作为参数调用函数ioctl分别获得MAC地址、IP地址、广播地址与子网掩码来实现
    char ip[20][20];
	int fd;
	int intrface;
	int retn = 0;
	int i = 0;
    struct ifreq buf[INET_ADDRSTRLEN];
    struct ifconf ifc;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) 
    {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) 
        {
            intrface = ifc.ifc_len / sizeof(struct ifreq);
            for (i=0;intrface-- > 0;i++) 
            {
				//get the subnet mask of this interface 
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
                    strcpy(ip[i],(inet_ntoa(((struct sockaddr_in *) (&buf[intrface].ifr_addr))->sin_addr)));
                    printf("IP:%s\n", ip[i]);
                }
            }
        }
        close(fd);
        return i;
    }
}
int main(int argc,char* argv[]) 
{
    int sock_ser_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int flag_is = 1;
	int connection;
	int flag_opt;
	int bind_return;
	int port = atoi(argv[2]);    
	char string_temp[1024];
	socklen_t readlen;
	socklen_t length;
	struct sockaddr_in socket_len;
    struct sockaddr_in socket_cli;
	struct sockaddr_in socket_ser;
	flag_opt = setsockopt(sock_ser_fd, SOL_SOCKET, SO_REUSEADDR, &flag_is, sizeof(int));
	if (flag_opt< 0)
	{
		perror("setsockopt(SO_REUSEADDR) failed");
	}
    socket_ser.sin_family = AF_INET;
    socket_ser.sin_port = htons(port);
    socket_ser.sin_addr.s_addr = inet_addr(argv[1]);
    get_local_ip();
	bind_return = bind(sock_ser_fd, (struct sockaddr *) &socket_ser, sizeof(socket_ser));
    if (bind_return== -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(sock_ser_fd, 20) == -1) 
    {
        perror("listen");
        exit(1);
    }
    length = sizeof(socket_cli);
    connection = accept(sock_ser_fd, (struct sockaddr *) &socket_cli, &length);
    if (connection < 0) 
    {
        perror("connect");
        exit(1);
	}
	else {
		cout << "与客户端连接成功!" << endl;
	}
    readlen = sizeof(socket_len);
    getpeername(connection, (struct sockaddr *) &socket_len, &readlen);
	cout << "client IP:" << inet_ntoa(socket_len.sin_addr);
	cout<< "  port:" << ntohs(socket_len.sin_port) << endl;
	while (1)
	{
		memset(string_temp, 0, sizeof(string_temp));
		if (recv(connection, string_temp, sizeof(string_temp), 0) == 0)
		{
			break;
		}
	}
    close(connection);
    close(sock_ser_fd);
    return 0;
}