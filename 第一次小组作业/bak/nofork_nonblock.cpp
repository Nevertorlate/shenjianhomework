#include "nofork_nonblock.h"

const char* str_client_state(int state)
{
    static const char* strs[] = {
        "CLIENT_NULL",
        "CLIENT_CONNECT",
        "CLIENT_WAIT_STUNO",
        "CLIENT_ECHO_STUNO",
        "CLIENT_WAIT_PID",
        "CLIENT_ECHO_PID",
        "CLIENT_WAIT_TIME",
        "CLIENT_ECHO_TIME",
        "CLIENT_WAIT_STR",
        "CLIENT_ECHO_STR",
        "CLIENT_WAIT_END",
        "CLIENT_SUCCESS",
        // "CLIENT_ERROR"
    };
    // switch(state){
    // CLIENT_NULL: return
    // CLIENT_CONNECT: return
    // CLIENT_WAIT_STUNO: return
    // CLIENT_ECHO_STUNO: return
    // CLIENT_WAIT_PID: return
    // CLIENT_ECHO_PID: return
    // CLIENT_WAIT_TIME: return
    // CLIENT_ECHO_TIME: return
    // CLIENT_WAIT_STR: return
    // CLIENT_ECHO_STR: return
    // CLIENT_WAIT_END: return
    // CLIENT_SUCCES: return
    // }
    // return state >= 0 && state < (sizeof(client_state_str) / sizeof(char*)) ? client_state_str[state];
    return strs[state];
}

const char* str_server_state(int state)
{
    static const char* strs[] = {
        "SERVER_NULL",
        "SERVER_SEND_STUNO",
        "SERVER_RECV_STUNO",
        "SERVER_SEND_PID",
        "SERVER_RECV_PID",
        "SERVER_SEND_TIME",
        "SERVER_RECV_TIME",
        "SERVER_SEND_STR",
        "SERVER_RECV_STR",
        "SERVER_SEND_END",
        "SERVER_RECV_END",
        "SERVER_SUCCESS"
    };
    // switch(state){
    // CLIENT_NULL: return
    // CLIENT_CONNECT: return
    // CLIENT_WAIT_STUNO: return
    // CLIENT_ECHO_STUNO: return
    // CLIENT_WAIT_PID: return
    // CLIENT_ECHO_PID: return
    // CLIENT_WAIT_TIME: return
    // CLIENT_ECHO_TIME: return
    // CLIENT_WAIT_STR: return
    // CLIENT_ECHO_STR: return
    // CLIENT_WAIT_END: return
    // CLIENT_SUCCES: return
    // }
    // return state >= 0 && state < (sizeof(client_state_str) / sizeof(char*)) ? client_state_str[state];
    return strs[state];
}