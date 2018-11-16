#include <stdio.h>  
#include <fcntl.h>
#include<string.h>
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
	pid_t pid_father;
    pid_pipe=fork();
	if (pid_pipe == -1 || pid_pipe > 0) {
		return 0;
	}
    signal(SIGPIPE, pipesig_out);
    unlink("./fifo");
    mkfifo("./fifo", 0644);
    pid_father = fork();
    if (pid_father < 0) {
        perror("fork error");
        return -1;
    }
    if (pid_father==0) {
		int fifodescriberw;
		int fifodescriberr;
		int return_read;
        char store_read[128] = {0};
        fifodescriberw = open("./fifo", O_WRONLY);
        write(fifodescriberw, "wo ai shen jian test2-3!(to child)", strlen("wo ai shen jian test2-3!(to child)"));
        sleep(1);
        fifodescriberr=open("./fifo", O_RDONLY);
        return_read = read(fifodescriberr, store_read, sizeof(store_read));
        printf("child:%s\n", store_read);
        close(fifodescriberw);
        close(fifodescriberr);
		while (1) {
			sleep(1);
		}
    } 
    else 
    { 
		int return_read;
		int fifodescriberw;
		int fifodescriberr;
        char store_read[128] = {0};
        fifodescriberw = open("./fifo", O_RDONLY);
        return_read = read(fifodescriberw, store_read, sizeof(store_read));
        printf("parent:%s\n", store_read);
        sleep(1);
        fifodescriberr = open("./fifo", O_WRONLY);
        write(fifodescriberr, "wo ai shen jian test2-3!(to parent)", strlen("wo ai shen jian test2-3!(to parent)"));
        close(fifodescriberw);
        close(fifodescriberr);
		while (1) {
			sleep(1);
		}
    }
    return 0;
}
