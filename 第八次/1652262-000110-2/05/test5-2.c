#include <stdio.h> 
#include <unistd.h>  
#include <stdlib.h>   
#include <string.h>  
#include <signal.h>  
#include <sys/shm.h>  
void *mapping_shared = NULL; 
struct shared_use_st  //�����ڴ�ռ�
{  
    int write_flag1;
    int write_flag2;
    char buffer_share1[2048];
    char buffer_share2[2048];
};
void shmdt_shared(int sig)  
{  
    if(shmdt(mapping_shared) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n"); //�����ڴ���ӳ��ʧ�ܣ��޷��ӽ��̵�ַ�ռ����Ƴ� 
        exit(1);  
    }  
    exit(0);
}  
int main()  
{  
    pid_t pid_father;
    pid_father=fork();
	int create_shared;
	char buffer_out[BUFSIZ + 1]; 
	struct shared_use_st *star_memory = NULL;
	if (pid_father == -1 || pid_father > 0) {
		return 0;
	}
    strcpy(buffer_out,"wo ai shen jian test5-1!");
    (void) signal(SIGINT, shmdt_shared);
    (void) signal(SIGQUIT, shmdt_shared);
    (void) signal(SIGTERM, shmdt_shared);
	//���������ڴ�
    create_shared = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(create_shared == -1)  
    {  
        perror("shmget failed\n");//���������ڴ�ʧ��
        exit(1);  
    }  
	//ӳ�乲���ڴ�
    mapping_shared = shmat(create_shared, (void*)0, 0);  
    if(mapping_shared == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  //ӳ�乲���ڴ�ʧ��
        exit(1);  
    }    
    star_memory = (struct shared_use_st*)mapping_shared;  //�ýṹ��ָ��ָ����鹲���ڴ� 
    while(1)  
    {  
        while(!star_memory->write_flag1)
        {
            strncpy(star_memory->buffer_share1, buffer_out, 2048);  
            printf("��������Ϊ:%s\n",buffer_out);      
            star_memory->write_flag1 = 1;  
        }
        while(star_memory->write_flag2)
        {
            printf("��ȡ����Ϊ:%s\n", star_memory->buffer_share2);  
            star_memory->write_flag2 = 0;//ѭ���Ĵӹ����ڴ��ж����ݣ�ֱ��������end��Ϊֹ
        }
        sleep(1);//������˯һ�룬ͬʱ�ᵼ��д����˯һ�룬������������֮����д 
    }  
}  
