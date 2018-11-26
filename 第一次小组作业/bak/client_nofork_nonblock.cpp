#include "nofork_nonblock.h"
#include "utility.h"

//! 静态全局
static const int FDS_SIZE = 2048;
static Recorder clients[FDS_SIZE];
static int maxfd = 1024;
static int socket_running = 0;
static int socket_success = 0;
static int socket_failed = 0;
static int socket_target = 0;

void client_handler(int);
void add_connection(const sockaddr_in& addr);

void client_nofork_nonblock(const sockaddr_in& addr, int num)
{
    socket_target = num;
    // signal(SIGALRM, client_handler);
    // client_handler(0);
    // alarm(1);
    int cnt = 0;
    while (socket_success < num) {
        while (socket_running + socket_success < socket_target && cnt < 200) {
            add_connection(addr);
            // ++cnt;
        }
        cnt = 0;
        client_handler(0);
    }
}

//以一定时间间隔唤醒执行数据接收
void client_handler(int sig)
{
    fd_set rfds, wfds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    for (int connfd = 0; connfd <= maxfd; ++connfd) {
        if (clients[connfd].state != CLIENT_NULL) {
            FD_SET(connfd, &rfds);
            FD_SET(connfd, &wfds);
        }
    }

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 50000;
    select(0, 0, 0, 0, &timeout);

    timeout.tv_sec = 0;
    timeout.tv_usec = 50000;
    switch (select(maxfd + 1, &rfds, &wfds, 0, &timeout)) {
    case 0:
        return;
    case -1:
        printf("select error:%s(errno:%d)\n", strerror(errno), errno);
        return;
    default:
        break;
    }

    for (int connfd = 0; connfd <= maxfd; ++connfd) {
        if (FD_ISSET(connfd, &rfds) == 0 && FD_ISSET(connfd, &wfds) == 0)
            continue;
        printf("connfd:%d  state:%s\n", connfd, str_client_state(clients[connfd].state));
        errno = 0;

        switch (clients[connfd].state) {
        case CLIENT_CONNECT:
            client_connect(connfd);
            break;
        case CLIENT_WAIT_STUNO:
            client_wait_stuno(connfd);
            break;
        case CLIENT_WAIT_PID:
            client_wait_pid(connfd);
            break;
        case CLIENT_WAIT_TIME:
            client_wait_time(connfd);
            break;
        case CLIENT_WAIT_STR:
            client_wait_str(connfd);
            break;
        case CLIENT_WAIT_END:
            client_wait_end(connfd);
            break;

        case CLIENT_ECHO_STUNO:
            client_echo_stuno(connfd);
            break;
        case CLIENT_ECHO_PID:
            client_echo_pid(connfd);
            break;
        case CLIENT_ECHO_TIME:
            client_echo_time(connfd);
            break;
        case CLIENT_ECHO_STR:
            client_echo_str(connfd);
            break;
        default:
            printf("default\n");
            // exit(0);
            break;
        }
    }
}

int get_echopid(int connfd)
{
    return (getpid() << 16) + connfd;
}
int get_echosid()
{
    return 1652267;
}
void add_connection(const sockaddr_in& addr)
{
    //建立socket
    socket_running++;
    printf("Summary: Target:%d  Runing:%d  Success:%d  Failed：%d\n", socket_target, socket_running, socket_success, socket_failed);
    int connfd, retval;
    if ((connfd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    if ((retval = connect(connfd, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
        if (errno != EINPROGRESS) {
            printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
            exit(0);
        }
    }
    memset(&clients[connfd], 0, sizeof(Recorder));
    clients[connfd].sockfd = connfd;
    clients[connfd].state = retval >= 0 ? CLIENT_WAIT_STUNO : CLIENT_CONNECT;
    maxfd = connfd > maxfd ? connfd : maxfd;
}

void rm_connection(int connfd)
{
    socket_running--;
    socket_failed++;
    printf("Connfd:%d,  End state:%s\n", connfd, str_client_state(clients[connfd].state));

    close(connfd);
    //! 释放内存
    delete[] clients[connfd].str;
    memset(&clients[connfd], 0, sizeof(Recorder));
    if (connfd == maxfd) {
        while (maxfd > 0 && clients[maxfd].state == CLIENT_NULL)
            --maxfd;
    }
    printf("Summary: Target:%d  Runing:%d  Success:%d  Failed：%d\n", socket_target, socket_running, socket_success, socket_failed);
}

void close_connection(int connfd)
{
    save_file(clients[connfd].stuno, clients[connfd].pid, clients[connfd].timestamp, clients[connfd].str, clients[connfd].strlen);
    socket_success++;
    socket_failed--;
    rm_connection(connfd);
}

int client_recv_helper(int connfd, char* buff, int msg_type, int next_state, int next_msglen)
{
    Recorder& cli = clients[connfd];
    int retval;
    switch ((retval = read(connfd, buff + cli.offset, cli.msglen - cli.offset))) {
    case -1:
        if (errno == EAGAIN) {
            break;
        } else {
            printf("recv error:%s(errno:%d)\n", strerror(errno), errno);
            rm_connection(connfd);
        }
        break;
    case 0:
        rm_connection(connfd);
        break;
    default:
        if (parse_str((const char*)buff, cli.offset + retval) == msg_type) {
            cli.state = next_state;
            cli.offset = 0;
            cli.msglen = next_msglen;
            return 1;
        } else if (cli.offset + retval != cli.msglen) {
            printf("!!!!!!!!!!!!!!!!!!!recv partial\n");
            // rm_connection(connfd);
            // exit(EXIT_FAILURE);
            cli.offset += retval;
            rm_connection(connfd);
        } else {
            rm_connection(connfd);
        }
        break;
    }

    return 0;
}

int client_send_helper(int connfd, const void* buff, int next_state, int next_msglen)
{
    Recorder& cli = clients[connfd];
    int retval;
    switch ((retval = write(connfd, (char*)buff + cli.offset, cli.msglen - cli.offset))) {
    case -1:
        if (errno == EAGAIN) {
            break;
        } else {
            printf("recv error:%s(errno:%d)\n", strerror(errno), errno);
            rm_connection(connfd);
        }
        break;
    case 0:
        rm_connection(connfd);
        break;
    default:
        if (retval == cli.msglen - cli.offset) {
            cli.state = next_state;
            cli.offset = 0;
            cli.msglen = next_msglen;
            return 1;
        } else {
            printf("send partial\n");
            cli.offset += retval;
            // rm_connection(connfd);
            // exit(EXIT_FAILURE);
        }
        break;
    }
    return 0;
}

void client_connect(int connfd)
{
    int optval, retval;
    socklen_t optlen = sizeof(optval);
    retval = getsockopt(connfd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
    if (retval == 0 && optval == 0) {
        clients[connfd].state = CLIENT_WAIT_STUNO;
        clients[connfd].offset = 0;
        clients[connfd].msglen = 5;
        client_wait_stuno(connfd);
    } else {
        rm_connection(connfd);
    }
}
void client_wait_stuno(int connfd)
{
    if (client_recv_helper(connfd, clients[connfd].buff, MSG_STUNO, CLIENT_ECHO_STUNO, 4) > 0) {
        clients[connfd].stuno = get_echosid();
        *(int*)clients[connfd].buff = htonl(clients[connfd].stuno);
        client_echo_stuno(connfd);
    }
}
void client_echo_stuno(int connfd)
{
    if (client_send_helper(connfd, clients[connfd].buff, CLIENT_WAIT_PID, 3) > 0) {
    }
}
void client_wait_pid(int connfd)
{
    if (client_recv_helper(connfd, clients[connfd].buff, MSG_PID, CLIENT_ECHO_PID, 4) > 0) {
        clients[connfd].pid = get_echopid(connfd);
        *(int*)clients[connfd].buff = htonl(clients[connfd].pid);
        client_echo_pid(connfd);
    }
}
void client_echo_pid(int connfd)
{

    if (client_send_helper(connfd, clients[connfd].buff, CLIENT_WAIT_TIME, 5) > 0) {
    }
}
void client_wait_time(int connfd)
{
    if (client_recv_helper(connfd, clients[connfd].buff, MSG_TIME, CLIENT_ECHO_TIME, 19) > 0) {
        get_timestamp(clients[connfd].timestamp);
        client_echo_time(connfd);
    }
}
void client_echo_time(int connfd)
{
    if (client_send_helper(connfd, clients[connfd].timestamp, CLIENT_WAIT_STR, 9)) {
    }
}
void client_wait_str(int connfd)
{
    if (client_recv_helper(connfd, clients[connfd].buff, MSG_STR, CLIENT_ECHO_STR, 0) > 0) {
        Recorder& cli = clients[connfd];
        cli.strlen = atoi((char*)cli.buff + 3);
        cli.msglen = cli.strlen;
        //! 申请内存
        cli.str = new char[cli.strlen];
        get_rand_str(cli.str, cli.msglen);
        client_echo_str(connfd);
    }
}

void client_echo_str(int connfd)
{
    if (client_send_helper(connfd, clients[connfd].str, CLIENT_WAIT_END, 3)) {
    }
}

void client_wait_end(int connfd)
{
    if (client_recv_helper(connfd, clients[connfd].buff, MSG_END, CLIENT_SUCCESS, 0) > 0) {
        close_connection(connfd);
    }
}
