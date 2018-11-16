#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
char buffer_out[10];
int fdr_linkcli[20] = { 0 };
void SendLarge(int signo)
{
	int j, flag = 0;
	for (j = 0; j < 20; j++) {
		if (fdr_linkcli[j]) {
			flag = flag + send(fdr_linkcli[j], buffer_out, sizeof(buffer_out), MSG_DONTWAIT);
		}
	}
	if (flag)
	{
		cout << "send内容为:";
		for (j = 0; j < sizeof(buffer_out); j++) {
			cout << buffer_out[j];
		}
		cout << endl;
	}
	alarm(1);
}
int main(int argc, char* argv[])
{
	int i;
	int counter;
	int flg_client;
	int flg_en = 1;
	int limit_fdr = 0;
	int linkplace = atoi(argv[1]);
	int sock_ser = socket(AF_INET, SOCK_STREAM, 0);
	char buf_temp[100];
	socklen_t socklen_cli;
	struct sockaddr_in ip_cli;
	struct sockaddr_in ip_sersock;
	if (setsockopt(sock_ser, SOL_SOCKET, SO_REUSEADDR, &flg_en, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
	}
	cout << "套接字设置成功" << endl;
	ip_sersock.sin_family = AF_INET;
	ip_sersock.sin_port = htons(linkplace);
	ip_sersock.sin_addr.s_addr = htonl(INADDR_ANY);
	if (signal(SIGALRM, SendLarge) == SIG_ERR)
	{
		perror("signal");
		return 0;
	}
	setbuf(stdout, NULL);
	flg_client = fcntl(sock_ser, F_GETFL, 0);
	fcntl(sock_ser, F_SETFL, flg_client | O_NONBLOCK);
	if (bind(sock_ser, (struct sockaddr *)&ip_sersock, sizeof(ip_sersock)) == -1)
	{
		perror("bind");
		exit(1);
	}
	cout << "bind成功" << endl;
	if (listen(sock_ser, 20) == -1)
	{
		perror("listen");
		exit(1);
	}
	cout << "listen成功" << endl;
	for (counter = 0; counter < 10; counter++) {
		buffer_out[counter] = rand() % 25 + 'a';
	}
	socklen_cli = sizeof(ip_cli);
	fd_set fd_link;
	alarm(1);
	while (1)
	{
		FD_ZERO(&fd_link);
		FD_SET(sock_ser, &fd_link);
		if (limit_fdr < sock_ser) {
			limit_fdr = sock_ser;
		}
		for (i = 0; i < 20; i++) {
			if (fdr_linkcli[i])
			{
				FD_SET(fdr_linkcli[i], &fd_link);
				if (limit_fdr < fdr_linkcli[i]) {
					limit_fdr = fdr_linkcli[i];
				}
			}
		}
		int return_select = select(limit_fdr + 1, &fd_link, NULL, NULL, 0);
		if (return_select > 0)
		{
			if (FD_ISSET(sock_ser, &fd_link))
			{
				struct sockaddr_in addr_cli;
				socklen_t address_len;
				int connection_accept = 0;
				connection_accept = accept(sock_ser, (struct sockaddr*)&ip_cli, &socklen_cli);
				if (connection_accept > 0) {
					for (i = 0; i < 20; i++) {
						if (fdr_linkcli[i] == 0)
						{
							fdr_linkcli[i] = connection_accept;
							cout << "与客户端连接成功!" << endl;
							break;
						}
					}
				}
				int flg_client2 = fcntl(fdr_linkcli[i], F_GETFL, 0);
				fcntl(fdr_linkcli[i], F_SETFL, flg_client2 | O_NONBLOCK);
			}

			for (i = 0; i < 20; i++) {
				if (fdr_linkcli[i])
				{
					if (FD_ISSET(fdr_linkcli[i], &fd_link))
					{
						memset(buf_temp, 0, sizeof(buf_temp));
						int return_recv = recv(fdr_linkcli[i], buf_temp, sizeof(buf_temp), 0);
						if (return_recv > 0)
						{
							int k;
							cout << "recv内容为:";
							for (k = 0; k < return_recv; k++) {
								cout << buf_temp[k];
							}
							cout << endl;
						}
						else if (return_recv == 0)
						{
							FD_CLR(fdr_linkcli[i], &fd_link);
							fdr_linkcli[i] = 0;
						}
					}
				}
			}
		}
	}
	close(sock_ser);
	return 0;
}