#include "common.h"
#include "utility.h"

void sig_handler(int sig)
{
    printf("signal: %d \n", sig);
    if (sig == SIGINT) {
        exit(0);
    }
}
int main(int argc, char** argv)
{
    for (int i = 1; i <= 64; ++i) {
        signal(i, sig_handler);
    }
    int ip, num, mask;
    short port;
    struct sockaddr_in addr;
    switch (parse_argu(argc, argv, addr, num, mask, CHECK_SEVER)) {
    case TYPE_FORK_BLOCK:
        printf("Connect type:TYPE_FORK_BLOCK\n");
        // server_fork_block(addr);
        break;
    case TYPE_FORK_NONBLOCK:
        printf("Connect type:TYPE_FORK_NONBLOCK\n");
        // server_fork_nonblock(addr);
        break;
    case TYPE_NOFORK_NONBLOCK:
        printf("Connect type:TYPE_NOFORK_NONBLOCK\n");
        server_nofork_nonblock(addr);
        break;
    default:
        printf("Connect type:\n");
        // error_handler();
    }
}