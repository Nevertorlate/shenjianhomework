#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>
int main()
{
	pid_t pid_kill;
	int pid_create;
	char buf_fp[10] = { 0 };
	FILE *fp = popen("ps -e | grep \'test3-1-2\' | awk \'{print $1}\'", "r");
	pid_kill = fork();
	if (pid_kill == -1 || pid_kill > 0) {
		return 0;
	}
	fgets(buf_fp, 10, fp);
	pid_create = atoi(buf_fp);
	kill(pid_create, SIGUSR1);
	sleep(5);

        kill(2180,SIGKILL);
        sleep(5);

	kill(pid_create, SIGUSR2);
        sleep(5);
        kill(pid_create, SIGINT);
	while (1) {
		sleep(1);
	}
}
