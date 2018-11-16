#include <stdio.h> 
#include <signal.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>
int pid_atoi = 0;
char store_buf[30];
void ouch(int sig)
{
	FILE *file_open;
	file_open = fopen("./file_open", "rb+");
	if (file_open == NULL)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	memset(store_buf, 0, sizeof(store_buf));
	fgets(store_buf, sizeof(store_buf), file_open);
	printf("读取内容为:%s\n", store_buf);
	fclose(file_open);
	file_open = fopen("file_open", "w+");
	if (file_open == NULL)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	fputs("wo ai shen jian test3-2", file_open);
	printf("写入文件内容:wo ai shen jian test3-2\n");
	fclose(file_open);
	sleep(1);
	kill(pid_atoi, SIGUSR1);
}
int main()
{
	pid_t pid_fork;
	pid_fork = fork();
	if (pid_fork == -1 || pid_fork > 0) {
		return 0;
	}
	while (pid_atoi <= 0)
	{
		FILE *fp = popen("ps -e | grep \'test3-2-1\' | awk \'{print $1}\'", "r");
		char buf[10] = { 0 };
		fgets(buf, 10, fp);
		pid_atoi = atoi(buf);
	}
	sleep(1);
	(void)signal(SIGUSR1, ouch);
	kill(pid_atoi, SIGUSR1);
	while (1) {
		sleep(1);
	}
}
