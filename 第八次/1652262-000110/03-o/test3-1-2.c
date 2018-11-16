#include <stdio.h> 
#include <signal.h>  
#include <stdlib.h>  
#include <unistd.h>  
void sig_stop1(int sig)  
{  
    printf("\nOUCH1! - I got signal %d\n", sig);       
}  
void sig_stop2(int sig)  
{  
    printf("\nOUCH2! - I got signal %d\n", sig);  
    exit(0);
}  
void sig_stop3(int sig)
{
    printf("\nOUCH3! - I got signal %d\n", sig); 
    (void) signal(SIGINT, SIG_DFL); 
}
int main()  
{  
    pid_t pid_create;
    pid_create=fork();
	if (pid_create == -1 || pid_create > 0) {
		return 0;
	}
    (void) signal(SIGUSR1, sig_stop1); 
    (void) signal(SIGUSR2, sig_stop2);
    (void) signal(SIGINT, sig_stop3);
	while (1) {
		sleep(1);
	}
}  