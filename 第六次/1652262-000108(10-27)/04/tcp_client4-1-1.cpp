#include <iostream>
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
using namespace std;
int main(int argc ,char* argv[])
{
    int socket_clis = socket(AF_INET,SOCK_STREAM, 0);
    int adr_in=atoi(argv[2]);	
	int i,j;
	char string_in[30];
	char string_send[30];
    struct sockaddr_in socket_cser;
    memset(&socket_cser, 0, sizeof(socket_cser));
    socket_cser.sin_family = AF_INET;
    socket_cser.sin_port = htons(adr_in);
    socket_cser.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(socket_clis, (struct sockaddr *)&socket_cser, sizeof(socket_cser)) < 0)
    {
        perror("connect");
        exit(1);
    }
	else {
		cout << "连接成功!" << endl;
	}
    while (1)
    {
        for(i=0;i<99;i++)
        {
			for (j = 0; j < sizeof(string_send); j++)
			{
				string_send[j] = rand() % 10 + '0';
			}
			write(socket_clis, string_send, sizeof(string_send));
            cout<<"发送:";
			for (j = 0; j < sizeof(string_send); j++)
			{
				cout << string_send[j];
			}
			cout << endl;
            sleep(1);
            recv(socket_clis, string_in, sizeof(string_in),MSG_DONTWAIT); ///接收
			if (strcmp(string_in, "exit\n") == 0)
			{
				break;
			}
			fputs(string_in, stdout);
            memset(string_in, 0, sizeof(string_in));
        }
        memset(string_in, 0, sizeof(string_in));
        memset(string_send, 0, sizeof(string_send));
    }
    close(socket_clis);
    return 0;
}