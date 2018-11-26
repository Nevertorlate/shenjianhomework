#include "utility.h"
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int parse_argu(int argc, char** argv, sockaddr_in& addr, int& num, int& flag, int check)
{
    flag = 0;
    num = 100;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    for (int i = 1; i < argc; ++i) {
        if (i + 1 < argc && strcmp(argv[i], "--ip") == 0) {
            flag |= PARM_IP;
            addr.sin_addr.s_addr = inet_addr(argv[i + 1]);
            i++;
        } else if (i + 1 < argc && strcmp(argv[i], "--port") == 0) {
            flag |= PARM_PORT;
            addr.sin_port = htons(atoi(argv[i + 1]));
            i++;
        } else if (strcmp(argv[i], "--block") == 0) {
            flag |= PARM_BLOCK;
        } else if (strcmp(argv[i], "--fork") == 0) {
            flag |= PARM_FORK;
        } else if (strcmp(argv[i], "--nonblock") == 0) {
            flag |= PARM_NONBLOCK;
        } else if (strcmp(argv[i], "--nofork") == 0) {
            flag |= PARM_NOFORK;
        } else if (i + 1 < argc && strcmp(argv[i], "--num") == 0) {
            flag |= PARM_NUM;
            int tmp = atoi(argv[i + 1]);
            num = tmp >= 1 && tmp <= 1000 ? tmp : 100;
            ++i;
        } else {
            printf("error :%s\n", argv[i]);
        }
    }
    if ((flag & PARM_IP) == 0) { //缺ip
        if (check == CHECK_CLIENT) {
            printf("ip required \n");
            return TYPE_ERROR;
        } else { //server 使用缺省ip 0.0.0.0
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
    }
    if ((flag & PARM_PORT) == 0) { //缺port
        printf("port required \n");
        return TYPE_ERROR;
    }
    if ((flag & PARM_BLOCK)) {
        if (flag & PARM_NOFORK) //--nofork --block, --block 无效
            return TYPE_NOFORK_NONBLOCK;
        else //
            return TYPE_FORK_BLOCK;
    } else {
        if ((flag & PARM_FORK))
            return TYPE_FORK_NONBLOCK;
        else
            return TYPE_NOFORK_NONBLOCK;
    }
    return TYPE_ERROR;
}

int save_file(int stuno, int pid, const char* time, const char* str, size_t length)
{
    char buff[128];
    sprintf(buff, "%d.%d.pid.txt", stuno, pid);
    FILE* fp;
    if ((fp = fopen(buff, "w")) == NULL) {
        // printf()
        return 0;
    }
    fprintf(fp, "%d\n%d\n", stuno, pid);
    fwrite(time, sizeof(char), 19, fp);
    fprintf(fp, "\n");
    fwrite(str, sizeof(char), length, fp);
    fclose(fp);
    return 1;
}

int parse_str(const char* buff, size_t length, int& randnum)
{
    switch (length) {
    case 3:
        if (strncmp(buff, "pid", 3) == 0)
            return MSG_PID;
        if (strncmp(buff, "end", 3) == 0)
            return MSG_END;
        break;
    case 5:
        if (strncmp(buff, "StuNo", 5) == 0)
            return MSG_STUNO;
        if (strncmp(buff, "TIME", 5) == 0)
            return MSG_TIME;
        break;
    // case 5:
    //     break;
    case 9:
        if (strncmp(buff, "str", 3) == 0 && buff[8] == 0) {
            int tmp = 0;
            for (int i = 3; i < 8; ++i) {
                if (buff[i] < '0' || buff[i] > '9')
                    return MSG_ERROR;
                tmp = buff[i] - '0' + tmp * 10;
            }
            randnum = tmp;
            return MSG_STR;
        }
        break;
    default:
        break;
    }
    return MSG_ERROR;
}
int parse_str(const char* buff, size_t length)
{
    int num;
    return parse_str(buff, length, num);
}

int get_rand_num()
{
    std::uniform_int_distribution<int> u(32768, 99999);
    std::default_random_engine e(time(0));
    return u(e);
}
void get_rand_str(char* buff, size_t length)
{
    std::uniform_int_distribution<int> u(0, 255);
    std::default_random_engine e(time(0));
    for (int i = 0; i != length; ++i) {
        buff[i] = char(u(e));
    }
}

void get_timestamp(char* buff)
{
    time_t a = time(0);
    struct tm* t = localtime(&a);

    sprintf(buff,"%04d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}