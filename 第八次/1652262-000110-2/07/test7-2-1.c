#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/file.h>  
#include <sys/stat.h> 
#include <sys/types.h>  
int lock_set(int pipe_describer, int type)
{
	struct flock flock_file;
	flock_file.l_whence = 0;
	flock_file.l_start = 0;
	flock_file.l_len = 0;
	flock_file.l_type = type;
	flock_file.l_pid = -1;
	fcntl(pipe_describer, F_GETLK, &flock_file);
	if (flock_file.l_type != F_UNLCK)
	{
		if (flock_file.l_type == F_RDLCK) {
			printf("Read flock_file already set by %d\n", flock_file.l_pid);
		}
		else if (flock_file.l_type == F_WRLCK) {
			printf("Write flock_file already set by %d\n", flock_file.l_pid);
		}
	}
	flock_file.l_type = type;
	if ((fcntl(pipe_describer, F_SETLKW, &flock_file)) < 0)
	{
		printf("Lock failed:type = %d\n", flock_file.l_type);
		return 1;
	}
	switch (flock_file.l_type)
	{
	case F_RDLCK:
		printf("Read flock_file set by %d\n", getpid());
		break;
	case F_WRLCK:
		printf("Write flock_file set by %d\n", getpid());
		break;
	case F_UNLCK:
		printf("Release flock_file by %d\n", getpid());
		return 1;
	default:
		break;
	}
	return 0;
}
int main()  
{  
    int pipe_describer;	
	int write_return;
	char buffer_store[20] = "process 1st";
	pid_t father_pid;
    father_pid=fork();
	if (father_pid == -1 || father_pid > 0) {
		return 0;
	} 
	pipe_describer = open("test7",O_RDWR | O_CREAT, 0666);  
	if(pipe_describer < 0)  
	{  
		printf("打开文件失败\n");  
		exit(1);  
	}  
	fcntl(pipe_describer,F_SETFL,fcntl(pipe_describer,F_GETFL)|O_NONBLOCK);
	lock_set(pipe_describer, F_WRLCK);  
	sleep(10);
	write_return = write(pipe_describer, buffer_store, strlen(buffer_store));
	if (write_return < 0) {
		printf("写入失败.\n");
	}
	else {
		printf("写入内容为:%d\n", write_return);
	}
	lock_set(pipe_describer, F_UNLCK);  
	close(pipe_describer);  
	while (1) {
		sleep(1);
	}
} 