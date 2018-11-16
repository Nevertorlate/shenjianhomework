#include <stdio.h> 
#include <fcntl.h>  
#include <limits.h>  
#include <string.h>
#include <unistd.h>  
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>   
int main()  
{  
    pid_t pid_father;
    pid_father=fork();
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
	int pipe_describer = -1;  
    int create_flag = 0;
	int bytes_sent = 0;  
    const int file_readonly = O_WRONLY; 
	const char *file_path = "./my_fifo";   
    char store_buf[100]="wo ai shen jian test2-4-1!";  
    if(access(file_path, F_OK) == -1)  
    {  
        create_flag = mkfifo(file_path, 0644);  
        if(create_flag != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", file_path);  
            exit(1);  
        }  
    }  
    printf("Process %d opening FIFO O_WRONLY\n", getpid());  
    pipe_describer = open(file_path, file_readonly);  
    if(pipe_describer != -1)  
    {  
        create_flag = write(pipe_describer, store_buf, sizeof(store_buf));  
        printf("write·µ»ØÄÚÈÝ:%s\n",store_buf);
        close(pipe_describer);  
    }  
	else {
		exit(1);
	}
	while (1)
    {
        sleep(1);
    }
  
}  
