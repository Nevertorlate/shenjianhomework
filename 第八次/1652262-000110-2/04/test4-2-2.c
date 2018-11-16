#include <stdio.h>  
#include <errno.h> 
#include <signal.h>  
#include <unistd.h>  
#include <stdlib.h> 
#include <string.h>  
#include <sys/msg.h>
#include <sys/types.h>  
int message_num1 = -1;
int message_num2 = -1;
struct message_con
{
	long int mode_info;
	char buffer_message[512];
};
void stopget(int sig)
{
	if (msgctl(message_num1, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(1);
	}
	if (msgctl(message_num2, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(1);
	}
	exit(0);
}
int main()
{
	pid_t pid_create;
	long int msgtype = 0;
	struct message_con send_info1;
	struct message_con send_info2;
	pid_create = fork();
	if (pid_create == -1 || pid_create > 0) {
		return 0;
	}
	message_num1 = msgget((key_t)4321, 0777 | IPC_CREAT);
	if (message_num1 == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(1);
	}
	message_num2 = msgget((key_t)1234, 0666 | IPC_CREAT);
	if (message_num2 == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(1);
	}
	(void)signal(SIGINT, stopget);
	(void)signal(SIGQUIT, stopget);
	(void)signal(SIGTERM, stopget);
	while (1)
	{
		send_info1.mode_info = 1;
		strcpy(send_info1.buffer_message, "wo ai shen jian test4-2!");
		printf("·¢ËÍÄÚÈİÎª:º:%s\n", send_info1.buffer_message);
		if (msgsnd(message_num1, (void*)&send_info1, 512, 0) == -1)
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(1);
		}
		memset(send_info2.buffer_message, 0, sizeof(send_info2.buffer_message));
		if (msgrcv(message_num2, (void*)&send_info2, 512, msgtype, 0) == -1)
		{
			fprintf(stderr, "msgrcv failed with errno: %d\n", errno);
			exit(1);
		}
		printf("½ÓÊÕÄÚÈİÎª:º:%s\n", send_info2.buffer_message);
		sleep(1);
	}
}
