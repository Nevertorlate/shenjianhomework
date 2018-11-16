#include <stdio.h>  
#include <fcntl.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <limits.h>  
#include <string.h>
#include <sys/stat.h>  
#include <sys/types.h>    
int main()  
{  
    pid_t pid_father;
    pid_father=fork();    
	int pipe_describer = -1;  
    int create_flag = 0;  
    int file_readonly = O_RDONLY;  
    char store_buf[100];
	const char *file_path = "./my_fifo"; 
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
    memset(store_buf, 0, sizeof(store_buf));  
    printf("Process %d opening FIFO O_RDONLY\n", getpid());  
    pipe_describer = open(file_path, file_readonly);  
    if(pipe_describer != -1)  
    {  
        create_flag = read(pipe_describer, store_buf, sizeof(store_buf)); 
        printf("read:%s\n",store_buf);
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