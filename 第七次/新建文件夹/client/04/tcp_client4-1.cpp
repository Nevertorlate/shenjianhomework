#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
int main(int argc ,char* argv[])
{
	int loop_count;
	int flg_client;
	int counter = 0;
	int linkplace=atoi(argv[2]);
    int ip_sockclient = socket(AF_INET,SOCK_STREAM, 0);
	char buffer_temp[20];
    struct sockaddr_in ip_ser;
    memset(&ip_ser, 0, sizeof(ip_ser));
    ip_ser.sin_family = AF_INET;
    ip_ser.sin_port = htons(linkplace);
    ip_ser.sin_addr.s_addr = inet_addr(argv[1]);
    flg_client = fcntl(ip_sockclient, F_GETFL, 0);
    fcntl(ip_sockclient, F_SETFL, flg_client | O_NONBLOCK);
    fd_set  fd_link; 
    //FD_ZERO(&fd_link); 
    //FD_SET(ip_sockclient, &fd_link); 
	int ret;
	int nonum = 1;
	ret = connect(ip_sockclient, (struct sockaddr *)&ip_ser, sizeof(ip_ser));
	if (errno == EINPROGRESS)
	{
		while (1)
		{
			if (nonum == 0)
				break;
			FD_ZERO(&fd_link);
			FD_SET(ip_sockclient, &fd_link);
			switch (select(ip_sockclient + 1, &fd_link, NULL, NULL, NULL))
			{
			case -1:
				perror("select");
				break;
			case 0:
				sleep(1);
				printf("与服务端连接超时\n");
				break;
			default:
				if (FD_ISSET(ip_sockclient, &fd_link))
				{

					int ret;
					if (connect(ip_sockclient, (struct sockaddr *)&ip_ser, sizeof(ip_ser)) < 0)
					{

						perror("connect:");
						exit(-1);
					}
					nonum = 0;
					break;
				}
			}
		}
	}
    cout<<"与服务端连接成功!"<<endl;
    while (1)
    {
		for (loop_count = 0; loop_count < 20; loop_count++) {
			buffer_temp[loop_count] = rand() % 25 + 'a';
		}
        counter+=sizeof(buffer_temp);
        if(write(ip_sockclient, buffer_temp, sizeof(buffer_temp))==EAGAIN)
        {
           getchar();
        } 
        cout<<"当前client端发送数据量为:"<<counter<<endl;
    }
    close(ip_sockclient);
    return 0;
}