#include <stdio.h> 
#include <unistd.h>  
#include <stdlib.h>   
#include <string.h>  
#include <signal.h>  
#include <sys/shm.h>  
void *mapping_shared = NULL; 
struct shared_use_st  //共享内存空间
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
        fprintf(stderr, "shmdt failed\n"); //共享内存解除映射失败，无法从进程地址空间中移除 
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
	//创建共享内存
    create_shared = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(create_shared == -1)  
    {  
        perror("shmget failed\n");//创建共享内存失败
        exit(1);  
    }  
	//映射共享内存
    mapping_shared = shmat(create_shared, (void*)0, 0);  
    if(mapping_shared == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  //映射共享内存失败
        exit(1);  
    }    
    star_memory = (struct shared_use_st*)mapping_shared;  //让结构体指针指向这块共享内存 
    while(1)  
    {  
        while(!star_memory->write_flag1)
        {
            strncpy(star_memory->buffer_share1, buffer_out, 2048);  
            printf("发送内容为:%s\n",buffer_out);      
            star_memory->write_flag1 = 1;  
        }
        while(star_memory->write_flag2)
        {
            printf("读取内容为:%s\n", star_memory->buffer_share2);  
            star_memory->write_flag2 = 0;//循环的从共享内存中读数据，直到读到“end”为止
        }
        sleep(1);//读进程睡一秒，同时会导致写进程睡一秒，这样做到读了之后再写 
    }  
}  
