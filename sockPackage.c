//
// Created by wojiaojd on 22/6/2020.
//
#include <string.h>
#include <stdio.h>
#include "sockPackage.h"

SockPackage * sock_package_newPackage()
{
    SockPackage *sockPackage = calloc(1, sizeof(SockPackage));
    sockPackage->cmd_0 = 0;
    sockPackage->cmd_1 = 0;
    sockPackage->sender = 0;
    sockPackage->recver = 0;
    sockPackage->otherMsg = NULL;
    return sockPackage;
}
//不自动追加回车
void sock_package_appendOtherMsg(SockPackage * package, const char * otherMsg)
{
    strcat(package->otherMsg, otherMsg);
}

const char * sock_package_toRowMessage(SockPackage * package)
{
    char buf[1024];
    sprintf(buf, "%d\n%d\n%d\n%d\n$\n",
            package->cmd_0,
            package->cmd_1,
            package->sender,
            package->recver);
    if(package->otherMsg == NULL)
    {
        strcat(buf, "$\n");
    } else {
        strcat(buf, package->otherMsg);
        strcat(buf, "\n$\n");
    }
    char *res = calloc(strlen(buf), sizeof(char));
    strcpy(res, buf);
    return res;
}

void sock_package_removeOtherMsg(SockPackage *package)
{
    if(package->otherMsg == NULL)
        return;
    free(package->otherMsg);
    package->otherMsg = NULL;
    return;
}
