#include <stdio.h>
#include <fcntl.h>  
#include <limits.h>  
#include <unistd.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/stat.h>
#include <sys/types.h>   
int main()
{
	pid_t pid_father;
	pid_father = fork();
	int create_flag = 0;
	int pipe_describer1 = -1;
	int pipe_describer2 = -1;
	int file_readonly = O_RDONLY;//以只读模式打开  
	int file_writeonly = O_WRONLY;//以写模式打开  
	char store_buf[100];
	const char *file_open1 = "./my_fifo1";
	const char *file_open2 = "./my_fifo2";
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
	memset(store_buf, 0, sizeof(store_buf));
	pipe_describer1 = open(file_open1, file_readonly);
	pipe_describer2 = open(file_open2, file_writeonly);
	if (pipe_describer1 != -1)
	{
		create_flag = read(pipe_describer1, store_buf, sizeof(store_buf));
		printf("read返回内容:%s\n", store_buf);
		close(pipe_describer1);
	}
	else {
		exit(1);
	}
	if (pipe_describer2 != -1)
	{
		create_flag = write(pipe_describer2, store_buf, sizeof(store_buf));
		printf("write返回内容:%s\n", store_buf);
		close(pipe_describer2);
	}
	else {
		exit(1);
	}
	while (1)
	{
		sleep(1);
	}
}
