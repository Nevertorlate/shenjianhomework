#include <stdio.h>   
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
void pipesig_out(int sig)
{
    printf("sig=%d\n", sig);
}
int main()  
{  
    pid_t pid_pipe;
    pid_pipe=fork();
	if (pid_pipe == -1 || pid_pipe > 0) {
		return 0;
	}
    signal(SIGPIPE, pipesig_out);
    unlink("./fifo");
    mkfifo("./fifo", 0644);
    pid_t pid_father = fork();
    if (pid_father < 0) 
    {
        perror("fork error");
        return -1;
    }
    if (0 == pid_father) 
    {  
        int fifodescriberw;
		int len_write=strlen("wo ai shen jian test2-2!");
        fifodescriberw = open("./fifo", O_WRONLY);
        write(fifodescriberw, "wo ai shen jian test2-2!", len_write);
        close(fifodescriberw);
        while(1)
        {
            sleep(1);
        }
    } 
    else 
    {          
		int return_read;
        int fifodescriberw;
        char store_read[128] = {0};
        fifodescriberw = open("./fifo", O_RDONLY);
        return_read = read(fifodescriberw, store_read, sizeof(store_read));
        printf("%s\n", store_read);
        close(fifodescriberw);
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}
