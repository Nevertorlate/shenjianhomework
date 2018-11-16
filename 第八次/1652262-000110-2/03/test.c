#include<stdio.h>  
#include<string.h>  
int main(int argc,char*argv[]){  
    FILE *fstream=NULL;    
    char buff[1024];  
    memset(buff,0,sizeof(buff));  
    if(NULL==(fstream=popen("kill -0 3495","r")))    
    {   
        printf("failure");    
        return -1;    
    }
    else
      printf("success");   
    pclose(fstream);  
    return 0;   
} 

