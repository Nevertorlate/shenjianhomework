#include "utility.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <map>
#include <netinet/in.h>
#include <set>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

enum CLIENT_STATE {
    CLIENT_NULL = 0,
    CLIENT_CONNECT,
    CLIENT_WAIT_STUNO,
    CLIENT_ECHO_STUNO,
    CLIENT_WAIT_PID,
    CLIENT_ECHO_PID,
    CLIENT_WAIT_TIME,
    CLIENT_ECHO_TIME,
    CLIENT_WAIT_STR,
    CLIENT_ECHO_STR,
    CLIENT_WAIT_END,
    CLIENT_SUCCESS
};

enum SERVER_STATE {
    SERVER_NULL = 0,
    SERVER_SEND_STUNO,
    SERVER_RECV_STUNO,
    SERVER_SEND_PID,
    SERVER_RECV_PID,
    SERVER_SEND_TIME,
    SERVER_RECV_TIME,
    SERVER_SEND_STR,
    SERVER_RECV_STR,
    SERVER_SEND_END,
    SERVER_RECV_END,
    SERVER_SUCCESS
};

const char* str_client_state(int state);
const char* str_server_state(int state);


//TODO: 结构待改进
struct Recorder {
    int sockfd;
    int stuno;
    int pid;
    char timestamp[19];
    char *str;
    int strlen;
    int offset;
    int msglen;
    char buff[10];
    int state;
};


void client_connect(int connfd);
void client_wait_stuno(int connfd);
void client_wait_pid(int connfd);
void client_wait_time(int connfd);
void client_wait_str(int connfd);
void client_wait_end(int connfd);
void client_echo_stuno(int connfd);
void client_echo_pid(int connfd);
void client_echo_time(int connfd);
void client_echo_str(int connfd);

void server_send_stuno(int connfd);
void server_send_pid(int connfd);
void server_send_time(int connfd);
void server_send_str(int connfd);
void server_send_end(int connfd);
void server_recv_stuno(int connfd);
void server_recv_pid(int connfd);
void server_recv_time(int connfd);
void server_recv_str(int connfd);
void server_recv_end(int connfd);

// void rm_connfd(int connfd);
// void close_connfd(int connfd);

