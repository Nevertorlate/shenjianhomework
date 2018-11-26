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
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
void client_fork_block(const sockaddr_in& addr, int num);
void client_fork_nonblock(const sockaddr_in& addr, int num);
void client_nofork_nonblock(const sockaddr_in& addr, int num);
// void error_handler(); //没实现 占个位置

void server_fork_block(const sockaddr_in& addr);
void server_fork_nonblock(const sockaddr_in& addr);
void server_nofork_nonblock(const sockaddr_in& addr);