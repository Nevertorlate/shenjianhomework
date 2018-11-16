#include <stdio.h> 
#include <signal.h>  
#include <stdlib.h>  
#include <unistd.h>
#include<string.h>  
void find_pid();
void sig_stop1(int sig)  
{  
    printf("\nOUCH1! - I got signal %d\n", sig);
    find_pid();       
}  
void sig_stop2(int sig)  
{  
    printf("\nOUCH2! - I got signal %d\n", sig);
    find_pid();  
    exit(0);
}  
void sig_stop3(int sig)
{
    printf("\nOUCH3! - I got signal %d\n", sig); 
    (void) signal(SIGINT, SIG_DFL); 
}
void sig_kill(int sig)
{
    printf("\nsig_kill! - I got signal %d\n", sig);

}
void find_pid()
{
    FILE* fp=NULL;
    int count=0;
    int BUFSZ=100;
    char buf[BUFSZ];
    char command[150];
 
 
    //system() 也可以检测进程是否存在，但非常耗费资源
    sprintf(command, "ps -C %s|wc -l", "test-extra-1" );
 
 
    if((fp = popen(command,"r")) == NULL)
    {
        printf("(fp = popen(command,\"r\")) == NULL\n");
    }
 
    if( (fgets(buf,BUFSZ,fp))!= NULL )
    {
        count = atoi(buf);
 
        if((count - 1) == 0)
        {
 
            //printf("已收到kill -9信号\n");
 
            //可以用system()开启test
        }
 
        else
        {
           //printf("未收到kill -9信号\n");
        }
 
    }
    pclose(fp);
    fp=NULL;
}

void find_pid1()
{
    FILE *fstream=NULL;
    char buff[1024];
    //memset(buff,0,sizeof(buff));
    if(NULL==(fstream=popen("kill -0 3593","r"))) 
    {
        printf("未收到kill -9信号\n");
    }
    else
      printf("接收到kill -9信号\n");
    pclose(fstream);
}
int main()  
{  
    pid_t pid_create;
    int num=getpid();
    char numStr[6];
    char temp[50];
    snprintf(numStr,5,"%d",num);
    char tempcommand[50]="strace -e trace=write -s 200 -f -p ";
    strcat(tempcommand,numStr);       
    sprintf(temp,tempcommand);

    pid_create=fork();
	if (pid_create == -1 || pid_create > 0) {
		return 0;
	}
    (void) signal(SIGKILL, sig_kill);    
    (void) signal(SIGUSR1, sig_stop1); 
    (void) signal(SIGUSR2, sig_stop2);
    (void) signal(SIGINT, sig_stop3);
    while(1)
    {
       sleep(1);
    }
}  
