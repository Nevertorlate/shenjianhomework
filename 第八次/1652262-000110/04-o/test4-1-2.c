#include <stdio.h>
#include <errno.h>    
#include <signal.h>  
#include <string.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/msg.h>  
int message_num = -1;  
struct message_con  
{  
    long int mode_info;  
    char buffer_message[512];  
};  
void stopget(int sig)  
{  
    if(msgctl(message_num, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(1);  
    }  
    exit(0);
}  
int main()  
{  
    pid_t pid_create;
    pid_create=fork();
	if (pid_create == -1 || pid_create > 0) {
		return 0;
	}
	long int mode_infom = 0;
    struct message_con send_info;  
    (void) signal(SIGINT, stopget);
    (void) signal(SIGQUIT, stopget);
    (void) signal(SIGTERM, stopget); 
    message_num = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(message_num == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(1);  
    }  
    while(1)  
    {  
        if(msgrcv(message_num, (void*)&send_info, 512, mode_infom, 0) == -1)  
        {  
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            exit(1);  
        }  
        printf("接收内容为:%s\n",send_info.buffer_message);  
    }  
}  