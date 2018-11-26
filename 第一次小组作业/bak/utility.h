#ifndef UTILITY_H
#define UTILITY_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PARM_BLOCK 1
#define PARM_NONBLOCK 2
#define PARM_FORK 4
#define PARM_NOFORK 8
#define PARM_IP 16
#define PARM_PORT 32
#define PARM_NUM 64

#define TYPE_FORK_BLOCK 1
#define TYPE_FORK_NONBLOCK 2
#define TYPE_NOFORK_NONBLOCK 3
#define TYPE_ERROR -1

#define CHECK_CLIENT 1
#define CHECK_SEVER 2

#define MSG_STUNO   1
#define MSG_PID     2
#define MSG_TIME    3
#define MSG_STR     4
#define MSG_END     5
#define MSG_ERROR   6

/******************************************
Description : 分析传入的命令行参数
Parameter:
    argc  :
    argv  :
    addr  :引用参数，IPV4地址，为其赋值传入的ip/port值，网络序，server ip缺省使用0.0.0.0, client无缺省， port 无缺省
    num   :引用参数，返回传入的连接数num值，缺省为100
    mask  :引用参数，用于标记传入参数，--ip, --port, --block, --nonblock, --fork, 
           --nofork分别对应一个比特位，可用于判断是否传入了某个参数
    check :值为CHECK_CLIENT或CHECK_SEVER，用于确定参数的检查等级，server/client对参数的要求不同，如server ip有缺省值，client无
Return value:
    TYPE_FORK_BLOCK     : 采用 fork/block 方式连接
    TYPE_FORK_NONBLOCK  : 采用 fork/nonblock 方式连接
    TYPE_NOFORK_NONBLOCK: 采用 nofork/nonblock 方式连接
    TYPE_ERROR          : 输入参数不足或有误等，不能执行连接
******************************************/
int parse_argu(int argc, char** argv, sockaddr_in &addr, int& num, int& flag, int check);

/******************************************
Description : 保存交互内容到文件
Parameter:
    stuno  :学号, int, 主机序
    pid    :进程ID, int, 主机序
    time   :时间戳, char*, 格式 yyyy-mm-dd hh:mm:ss, 定长19字节,不需要尾0
    buff   :随机字符串缓冲区, char*, 含特殊字符,通过length限定输入内容的长度,buff大小不小于length
    length :随机字符串长度, size_t
Return value:
    0   : failed
    1   : success
******************************************/
int save_file(int stuno, int pid, const char* time, const char* buff, size_t length);

/******************************************
Description : client识别server发来的字符串，共5种，StuNo,pid,TIME,str*****,end，分别对应的宏 MSG_*
Parameter:
    buff  :字符串缓冲区
    length    :接收到的字符串长度，即read/recv返回值，不含尾0
    randnum   :引用参数，仅当字符串为str*****时，将randnum赋值为*****，不含此参数的版本不执行些操作
Return value:
    MSG_STUNO   1:StuNo
    MSG_PID     2:pid
    MSG_TIME    3:TIME
    MSG_STR     4:str*****
    MSG_END     5:end
    MSG_ERROR   6:其它
NOTE:
    字符串为str*****，*****含非法字符或范围不在32768-99999时作为MSG_ERROR处理
******************************************/
int parse_str(const char* buff, size_t length, int& randnum);
int parse_str(const char* buff, size_t length);


/******************************************
Description : 生成一个范围在32768-99999的随机数
******************************************/
int get_rand_num();

/******************************************
Description : 生成一个指定长度的随机字符串
******************************************/
void get_rand_str(char *buff, size_t length);

void get_timestamp(char *buff);

#endif