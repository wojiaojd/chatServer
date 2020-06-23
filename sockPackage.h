//
// Created by wojiaojd on 22/6/2020.
// call sock_package_removeOtherMsg() before set new value to otherMsg
//

#ifndef CHATSERVER_SOCKPACKAGE_H
#define CHATSERVER_SOCKPACKAGE_H
#include <stdlib.h>
typedef struct sock_package {
    int cmd_0;
    int cmd_1;
    int sender;
    int recver;
    char * otherMsg;
}SockPackage;

SockPackage * sock_package_newPackage();

void sock_package_appendOtherMsg(SockPackage * package, const char * otherMsg);

const char * sock_package_toRowMessage(SockPackage * package);

void sock_package_removeOtherMsg(SockPackage *package);

#endif //CHATSERVER_SOCKPACKAGE_H
