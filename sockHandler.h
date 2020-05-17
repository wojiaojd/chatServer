//
// Created by wojiaojd on 12/5/2020.
//

#ifndef CHATSERVER_SOCKHANDLER_H
#define CHATSERVER_SOCKHANDLER_H

#endif //CHATSERVER_SOCKHANDLER_H
#include <sys/types.h>
#include "usrData.h"

enum COMMAND {
    CMD_SIGNUP = 0,
    CMD_SIGNIN,
    CMD_TALKTO,
    CMD_REFUSE,
    CMD_ACK
};

struct sockHandlerArgs{
    int fd;
    struct usrData *idindx;
};

void msg_save(char *content, size_t len);
int get_line(int fd, char *buf, int len);
void *sock_recv(void *args);
void *sock_send(void *args);
void *sock_signup(void *args);
void *sock_signin(void *args);
void *sock_talkto(void *args);

