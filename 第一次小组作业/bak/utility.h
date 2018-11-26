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
Description : ��������������в���
Parameter:
    argc  :
    argv  :
    addr  :���ò�����IPV4��ַ��Ϊ�丳ֵ�����ip/portֵ��������server ipȱʡʹ��0.0.0.0, client��ȱʡ�� port ��ȱʡ
    num   :���ò��������ش����������numֵ��ȱʡΪ100
    mask  :���ò��������ڱ�Ǵ��������--ip, --port, --block, --nonblock, --fork, 
           --nofork�ֱ��Ӧһ������λ���������ж��Ƿ�����ĳ������
    check :ֵΪCHECK_CLIENT��CHECK_SEVER������ȷ�������ļ��ȼ���server/client�Բ�����Ҫ��ͬ����server ip��ȱʡֵ��client��
Return value:
    TYPE_FORK_BLOCK     : ���� fork/block ��ʽ����
    TYPE_FORK_NONBLOCK  : ���� fork/nonblock ��ʽ����
    TYPE_NOFORK_NONBLOCK: ���� nofork/nonblock ��ʽ����
    TYPE_ERROR          : ����������������ȣ�����ִ������
******************************************/
int parse_argu(int argc, char** argv, sockaddr_in &addr, int& num, int& flag, int check);

/******************************************
Description : ���潻�����ݵ��ļ�
Parameter:
    stuno  :ѧ��, int, ������
    pid    :����ID, int, ������
    time   :ʱ���, char*, ��ʽ yyyy-mm-dd hh:mm:ss, ����19�ֽ�,����Ҫβ0
    buff   :����ַ���������, char*, �������ַ�,ͨ��length�޶��������ݵĳ���,buff��С��С��length
    length :����ַ�������, size_t
Return value:
    0   : failed
    1   : success
******************************************/
int save_file(int stuno, int pid, const char* time, const char* buff, size_t length);

/******************************************
Description : clientʶ��server�������ַ�������5�֣�StuNo,pid,TIME,str*****,end���ֱ��Ӧ�ĺ� MSG_*
Parameter:
    buff  :�ַ���������
    length    :���յ����ַ������ȣ���read/recv����ֵ������β0
    randnum   :���ò����������ַ���Ϊstr*****ʱ����randnum��ֵΪ*****�������˲����İ汾��ִ��Щ����
Return value:
    MSG_STUNO   1:StuNo
    MSG_PID     2:pid
    MSG_TIME    3:TIME
    MSG_STR     4:str*****
    MSG_END     5:end
    MSG_ERROR   6:����
NOTE:
    �ַ���Ϊstr*****��*****���Ƿ��ַ���Χ����32768-99999ʱ��ΪMSG_ERROR����
******************************************/
int parse_str(const char* buff, size_t length, int& randnum);
int parse_str(const char* buff, size_t length);


/******************************************
Description : ����һ����Χ��32768-99999�������
******************************************/
int get_rand_num();

/******************************************
Description : ����һ��ָ�����ȵ�����ַ���
******************************************/
void get_rand_str(char *buff, size_t length);

void get_timestamp(char *buff);

#endif