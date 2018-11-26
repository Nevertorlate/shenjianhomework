#include "nofork_nonblock.h"
#include "utility.h"


//! 静态全局
static const int FDS_SIZE = 2048;
static const int MAX_CONNECT = 1024;
static int maxfd = 0;
static Recorder clients[FDS_SIZE];


void server_handler(int);
void add_connection(int connfd);

void server_nofork_nonblock(const sockaddr_in& addr)
{
    int listenfd;
    int optval;
    int connfd;

    //listen socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    //设置端口重用
    optval = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) < 0) {
        printf("setsockopt error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    //绑定端口
    if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    //监听端口
    if (listen(listenfd, MAX_CONNECT) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("wait for connect\n");

    while (1) {
        while ((connfd = accept(listenfd, NULL, NULL)) >= 0) {
            add_connection(connfd);
        }
        server_handler(0);
    }

    close(listenfd);
}

//以一定时间间隔唤醒执行数据接收
void server_handler(int sig)
{
    // printf("server_handler\n");
    fd_set rfds, wfds;
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);

    for (int i = 0; i <= maxfd; ++i) {
        if (clients[i].state != SERVER_NULL) {
            FD_SET(i, &rfds);
            FD_SET(i, &wfds);
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
        if (FD_ISSET(connfd, &wfds) == 0 && FD_ISSET(connfd, &rfds) == 0)
            continue;
        printf("connfd:%d   state:%s\n", connfd, str_server_state(clients[connfd].state));
        errno = 0;

        switch (clients[connfd].state) {
        case SERVER_SEND_STUNO:
            server_send_stuno(connfd);
            break;
        case SERVER_SEND_PID:
            server_send_pid(connfd);
            break;
        case SERVER_SEND_TIME:
            server_send_time(connfd);
            break;
        case SERVER_SEND_STR:
            server_send_str(connfd);
            break;
        case SERVER_SEND_END:
            server_send_end(connfd);
            break;
        case SERVER_RECV_STUNO:
            server_recv_stuno(connfd);
            break;
        case SERVER_RECV_PID:
            server_recv_pid(connfd);
            break;
        case SERVER_RECV_TIME:
            server_recv_time(connfd);
            break;
        case SERVER_RECV_STR:
            server_recv_str(connfd);
            break;
        case SERVER_RECV_END:
            server_recv_end(connfd);
            break;
        default:
            break;
        }
    }
}

void add_connection(int connfd)
{
    fcntl(connfd, F_SETFL, fcntl(connfd, F_GETFL, 0) | O_NONBLOCK);
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    getsockname(connfd, (struct sockaddr*)&client_addr, &addr_len);
    printf("Connected, client <%s:%d>, Connfd:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), connfd);
    memset(&clients[connfd],0,sizeof(Recorder));
    clients[connfd].sockfd = connfd;
    clients[connfd].state = SERVER_SEND_STUNO;
    maxfd = connfd > maxfd ? connfd : maxfd;
}

void rm_connection(int connfd)
{
    printf("Connfd:%d,  End state:%s\n", connfd, str_client_state(clients[connfd].state));
    close(connfd);
    //! 释放内存
    delete[] clients[connfd].str;
    memset(&clients[connfd], 0, sizeof(Recorder));
    if (connfd == maxfd) {
        while (maxfd > 0 && clients[maxfd].state == SERVER_NULL)
            --maxfd;
    }
}

void close_connection(int connfd)
{
    save_file(clients[connfd].stuno, clients[connfd].pid, clients[connfd].timestamp, clients[connfd].str, clients[connfd].strlen);
    rm_connection(connfd);
}

int server_send_helper(int connfd, const void* buff, int next_state, int next_msglen)
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
            clients[connfd].state = next_state;
            cli.offset = 0;
            cli.msglen = next_msglen;
            return 1;
        } else {
            cli.offset += retval;
            printf("!!!!!!!!!!!!!!!!!!!send partial\n");
            // rm_connection(connfd);
            // exit(EXIT_FAILURE);
        }
        break;
    }
    return 0;
}

int server_recv_helper(int connfd, void* buff, int next_state, int next_msglen)
{
    Recorder& cli = clients[connfd];
    int retval;
    switch ((retval = read(connfd, (char*)buff + cli.offset, cli.msglen - cli.offset))) {
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
            printf("recv partial\n");
            cli.offset += retval;
            // rm_connection(connfd);
            // exit(EXIT_FAILURE);
        }
        break;
    }
    return 0;
}

void server_send_stuno(int connfd)
{
    clients[connfd].msglen = 5;
    server_send_helper(connfd, "StuNo", SERVER_RECV_STUNO, 4);
}
void server_recv_stuno(int connfd)
{
    if (server_recv_helper(connfd, clients[connfd].buff, SERVER_SEND_PID, 3) > 0) {
        clients[connfd].stuno = ntohl(*(int*)clients[connfd].buff);
    }
}
void server_send_pid(int connfd)
{
    server_send_helper(connfd, "pid", SERVER_RECV_PID, 4);
}
void server_recv_pid(int connfd)
{
    if (server_recv_helper(connfd, clients[connfd].buff, SERVER_SEND_TIME, 5) > 0) {
        clients[connfd].pid = ntohl(*(int*)clients[connfd].buff);
    }
}
void server_send_time(int connfd)
{
    server_send_helper(connfd, "TIME", SERVER_RECV_TIME, 19);
}
void server_recv_time(int connfd)
{
    if (server_recv_helper(connfd, clients[connfd].timestamp, SERVER_SEND_STR, 9) > 0) {
        clients[connfd].strlen = get_rand_num();
        clients[connfd].str = new char[clients[connfd].strlen];
        sprintf(clients[connfd].buff, "str%5d", clients[connfd].strlen);
    }
}
void server_send_str(int connfd)
{
    if(server_send_helper(connfd, clients[connfd].buff, SERVER_RECV_STR, clients[connfd].strlen)>0){

    }
}
void server_recv_str(int connfd)
{
    if (server_recv_helper(connfd, clients[connfd].str, SERVER_SEND_END, 3) > 0) {
    }
    // server_recv_helper(connfd, clients[connfd].str + clients[connfd].offset, clients[connfd].msglen - clients[connfd].offset, SERVER_SEND_END);
}
void server_send_end(int connfd)
{
    server_send_helper(connfd, "end", SERVER_RECV_END, 0);
}

void server_recv_end(int connfd)
{
    switch (read(connfd, clients[connfd].buff, 1)) {
    case 0:
    case -1:
        clients[connfd].state = SERVER_SUCCESS;
        close_connection(connfd);
        break;
    default:
        rm_connection(connfd);
        break;
    }
}