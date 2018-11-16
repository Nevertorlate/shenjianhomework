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
	int create_flag = 0;
	int pipe_describer1 = -1;
	int pipe_describer2 = -1;
	int bytes_sent = 0;
	const int file_readonly = O_WRONLY;
	const int file_writeonly = O_RDONLY;
	const char *file_open1 = "./my_fifo1";
	const char *file_open2 = "./my_fifo2";
	char store_buf[100] = "wo ai shen jian test2-5-1!";
	pid_father = fork();
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
	if (access(file_open1, F_OK) == -1)
	{
		create_flag = mkfifo(file_open1, 0644);
		if (create_flag != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", file_open1);
			exit(1);
		}
	}
	create_flag = 0;
	if (access(file_open2, F_OK) == -1)
	{
		create_flag = mkfifo(file_open2, 0645);
		if (create_flag != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", file_open2);
			exit(1);
		}
	}
	pipe_describer1 = open(file_open1, file_readonly);
	pipe_describer2 = open(file_open2, file_writeonly);
	if (pipe_describer1 != -1)
	{
		create_flag = write(pipe_describer1, store_buf, sizeof(store_buf));
		printf("write返回内容:%s\n", store_buf);
		close(pipe_describer1);
	}
	else {
		exit(1);
	}
	if (pipe_describer2 != -1)
	{
		create_flag = read(pipe_describer2, store_buf, sizeof(store_buf));
		printf("read返回内容:%s\n", store_buf);
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
