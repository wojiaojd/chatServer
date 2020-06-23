//
// Created by wojiaojd on 12/5/2020.
//

#ifndef CHATSERVER_SOCKHANDLER_H
#define CHATSERVER_SOCKHANDLER_H


#include <sys/types.h>
#include "usrData.h"
#include "sockPackage.h"

enum COMMAND {
    CMD_SIGNUP = 0,
    CMD_SIGNIN,
    CMD_TALKTO,
    CMD_REFUSE,
    CMD_NEWFND,
    CMD_REQUEST,
    CMD_CONFIRM,
    CMD_USRINFO,
    CMD_BRIEF,
    CMD_DETAIL
};

typedef struct sock_handler_args{
    int fd;             //此连接的套接字描述符
    USRData *idindx;    //此连接对应用户的数据
    USRID id;           //此连接对应用户的id
    SockPackage *package;
}SockHandlerArgs;

typedef struct command_switch{
    RBTree *rbTree;
}CMDSwitch;

void msg_save(char *content, size_t len);
int get_line(int fd, char *buf, int len);
void cmd_switch_init();
void cmd_register(enum COMMAND cmd, void*(*handler)(void*));
void cmd_switch(enum COMMAND cmd, void *args);
void *sock_recv(void *args);
void *sock_send(void *args);
void *sock_signup(void *args);
void *sock_signin(void *args);
void *sock_talkto(void *args);
void *sock_newfriend(void *args);
void *sock_getUsrInfo(void *args);
SockPackage *sock_read_package(void *args);
int sock_write_package(void *args);

#endif //CHATSERVER_SOCKHANDLER_H

