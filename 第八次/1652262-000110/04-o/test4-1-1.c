#include <stdio.h>  
#include <errno.h>
#include <string.h>
#include <unistd.h>  
#include <stdlib.h>  
#include <sys/msg.h>  
struct message_con  
{  
    long int mode_info;  
    char buffer_message[512];  
};  
int main()  
{  
    pid_t pid_create;	
	char buffer[512]; 
	int message_num = -1; 
    struct message_con send_info;  
    pid_create=fork();
	if (pid_create == -1 || pid_create > 0) {
		return 0;
	}
    message_num = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(message_num == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(1);  
    }  
    while(1)  
    {  
        send_info.mode_info = 1; 
        printf("发送内容为:wo ai shen jian test4\n");
        strcpy(send_info.buffer_message, "wo ai shen jian test4");  
        if(msgsnd(message_num, (void*)&send_info, 512, 0) == -1)  
        {  
            fprintf(stderr, "msgsnd failed\n");  
            exit(1);  
        }  
        sleep(1);  
    }  
}  