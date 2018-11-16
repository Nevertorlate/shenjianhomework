#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>  
#include <unistd.h>  
#include<sys/wait.h>
#include <string.h>
#include<sys/prctl.h>
void son()
{
   if(fork()==0)
     son();
}
void find_pid()
{
    FILE* fp=NULL;
    int count=0;
    int BUFSZ=100;
    char buf[BUFSZ];
    char command[150];


    //system() 也可以检测进程是否存在，但非常耗费资源
    sprintf(command, "ps -C %s|wc -l", "test-extra-2" );


    if((fp = popen(command,"r")) == NULL)
    {
        printf("(fp = popen(command,\"r\")) == NULL\n");
    }

    if( (fgets(buf,BUFSZ,fp))!= NULL )
    {
        count = atoi(buf);

        if((count - 1) == 0)
        {

            printf("已收到kill -9信号\n");

            //可以用system()开启test
        }

        else
        {
            printf("未收到kill -9信号\n");
        }

    }
    pclose(fp);
    fp=NULL;
}
int main()
{
	pid_t pid_kill;
	int pid_create;
        int pid_child2;
        //pid_t pid_child=fork();
        //if(pid_child==0)
        //{
        //   pid_child2=getpid();

        //}        

 
	char buf_fp[10] = { 0 };
	FILE *fp = popen("ps -e | grep \'test-extra-2\' | awk \'{print $1}\'", "r");
	pid_kill = fork();
	if (pid_kill == -1 || pid_kill > 0) {
		return 0;
	}
       
	fgets(buf_fp, 10, fp);
	pid_create = atoi(buf_fp);
	kill(pid_create, SIGUSR1);
	sleep(5);
        int ret=kill(pid_create,SIGKILL);
        if(ret==-1)
        {
           find_pid();
           printf("signal SIGKILL kill failed\n");
        }
        else {
        printf("signal SIGKILL kill received\n");
        find_pid();
        }
        sleep(5);
        kill(pid_create, SIGUSR1);
        sleep(5);
	//kill(pid_create, SIGUSR2);
        //sleep(5);
        //kill(pid_create, SIGINT);
	while (1) {
		sleep(1);
	}
}
