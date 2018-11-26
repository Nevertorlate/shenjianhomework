#include "utility.h"
#include "common.h"


int main(int argc, char** argv)
{
    int ip, num, mask;
    short port;
    struct sockaddr_in addr;
    switch (parse_argu(argc, argv, addr, num, mask, CHECK_CLIENT)) {
    case TYPE_FORK_BLOCK:
        printf("Connect type:TYPE_FORK_BLOCK\n");
        // client_fork_block(addr, num);
        break;
    case TYPE_FORK_NONBLOCK:
        printf("Connect type:TYPE_FORK_NONBLOCK\n");
        // client_fork_nonblock(addr, num);
        break;
    case TYPE_NOFORK_NONBLOCK:
        printf("Connect type:TYPE_NOFORK_NONBLOCK\n");
        client_nofork_nonblock(addr, num);
        break;
    default:
        printf("Connect type:\n");
        // error_handler();
    }
}
