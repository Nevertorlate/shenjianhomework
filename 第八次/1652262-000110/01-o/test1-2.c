#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
    pid_t pid_father;
	pid_t pid_child;    
	int pipe_describer[2];	 //fd[0]存放管道读取端的文件描述符，fd[1]用于存放管道写入端的文件描述符。
    char store_buffer;
    pid_father=fork();
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
    if (pipe(pipe_describer) == -1)
    {
        perror("pipe");
		puts("create the pipe failed!\n");
        exit(EXIT_FAILURE);
    }
    pid_child = fork();
    if (pid_child == -1)
    {
        perror("fork");
		puts("fork failed!\n");
        exit(EXIT_FAILURE);
    }
    if (pid_child == 0)
    {
        write(pipe_describer[1], "wo ai shen jian test1-2!", strlen("wo ai shen jian test1-2!"));
		close(pipe_describer[0]); 
        close(pipe_describer[1]);
        while(1)//死循环阻塞
        {
            sleep(1);
        }
    }
    else
    {         
		while (read(pipe_describer[0], &store_buffer, 1) > 0) {
			write(STDOUT_FILENO, &store_buffer, 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		close(pipe_describer[1]);
        close(pipe_describer[0]); 
        while(1)//死循环阻塞
        {
            sleep(1);
        }
    }
}
