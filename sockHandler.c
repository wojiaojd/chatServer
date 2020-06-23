//
// Created by wojiaojd on 12/5/2020.
//
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errorhandler.h"
#include "sockHandler.h"
#include "database.h"
#include "sockqueue.h"
#include "errorhandler.h"

////handler注册, 必须对齐指令和回调函数
//void *(*commandSwitch[3])(void*)={sock_signup, sock_signin, sock_talkto};
CMDSwitch *cmdSwitch = NULL;
void cmd_switch_init()
{
    cmdSwitch = calloc(1, sizeof(CMDSwitch));
    cmdSwitch->rbTree = calloc(1, sizeof(RBTree));

    cmd_register(CMD_SIGNUP, sock_signup);
    cmd_register(CMD_SIGNIN, sock_signin);
    cmd_register(CMD_TALKTO, sock_talkto);
    cmd_register(CMD_NEWFND, sock_newfriend);
    cmd_register(CMD_USRINFO, sock_getUsrInfo);

}
void cmd_register(enum COMMAND cmd, void*(*handler)(void*))
{
    Node *newHandler = rbt_new_node();
    newHandler->key = calloc(1, sizeof(int));
    *((int*)newHandler->key) = cmd;
    newHandler->value = handler;
    rbt_insert(cmdSwitch->rbTree, newHandler);
}
void cmd_switch(enum COMMAND cmd, void *args)
{
    Node *handlerNode = rbt_find(cmdSwitch->rbTree, &cmd);
    printf("cmd:%d\n", *(int*)handlerNode->key);
    void*(*handler)(void*);
    handler = handlerNode->value;
    (*handler)(args);
}
void msg_save(char *content, size_t len)
{
    char pathname[255];
    sprintf(pathname, "%s", "/home/wojiaojd/chatServer/log_jojo");

    int fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, S_IWUSR | S_IRUSR);
    if(0 > fd)
    {
        error_handler("open file");
    }

    while(len > 0 && (content[len - 1] == '\n' || content[len - 1] == '\r'))
    {
        len--;
    }
    if(0 == len)
    {
        close(fd);
        return;
    }
    content[len++] = '\n';

    if(0 > write(fd, content, len))
    {
        close(fd);
        error_handler("write");
    }

    close(fd);
}
////行末无回车
int get_line(int fd, char *buf, int len)
{
    int i = 0;
    char c = '\0';
    int n;
    while(i < len - 1)
    {
        n = recv(fd, &c, 1, 0);
        if(n > 0)
        {
            if(c == '\n')
            {
                break;
            }
            buf[i++] = c;
        } else {
            break;
        }
    }
    buf[i] = '\0';
    return i;
}
/*这是接收用户发来的请求,如果是转发请求,则把消息挂到id索引数据结构的转发队列中*/
void *sock_recv(void *args)
{
    SockHandlerArgs *arg = args;
    int client = arg->fd;
    char buf[10];
    while(1)
    {
        SockPackage *package = sock_read_package(arg);
        if(package != NULL) {
            printf("COMMAND:%d\n", package->cmd_0);
            cmd_switch(package->cmd_0, arg);

        } else if(package == NULL){
            /*释放资源*/
            /*------*/
            printf("usrData_close fd:%d id:%d\n", arg->fd, arg->id);
            ////没有进行登录的连接不需要关闭用户数据
            if(arg->id != 0)
            {
                usrData_close(arg->id);
            }
            close(client);
            free(arg);
            return NULL;
        }
    }
}
void *sock_send(void *args)
{
    SockHandlerArgs *arg = args;
    char buf[100];
    ////用户登录
    usrData_signin(arg->id, arg->fd);
    ////通知对端连接成功可以继续发送消息
    sprintf(buf, "%d\n", CMD_SIGNIN);
    arg->package->cmd_0 = CMD_SIGNIN;
    arg->package->cmd_1 = CMD_CONFIRM;
    arg->package->sender = 0;
    arg->package->recver = arg->id;
    sock_package_removeOtherMsg(arg->package);
    if(0 > sock_write_package(arg))
    {
        return NULL;
    }
    //提取发送节点,发送的是原始数据(协议)
    Msg *m = NULL;
    while((m = usrData_msgqueue_pop(arg->id)) != NULL)
    {
        ////转发
        printf("转发:\n%s\n", m->content);
        send(arg->fd, m->content, strlen(m->content), 0);
        msg_free(m);
    }
    return NULL;
}

/*用户注册过程*/
void *sock_signup(void *args)
{
    SockHandlerArgs *arg = args;
    USRID id;
    //避免mysql_real_escape_string过程中全部都是需转义的字符,长度应为2*n+1
    char username[101];
    char pswd[201];
    int otherMsgIndex = 0, usrNameIndex = 0, pswdIndex = 0;
    //分割字符串提取用户名密码
    while(arg->package->otherMsg[otherMsgIndex] != '\n')
    {
        username[usrNameIndex++] = arg->package->otherMsg[otherMsgIndex++];
    }
    otherMsgIndex++;
    username[usrNameIndex] = '\0';
    while(arg->package->otherMsg[otherMsgIndex] != '\n')
    {
        pswd[pswdIndex++] = arg->package->otherMsg[otherMsgIndex++];
    }
    pswd[pswdIndex] = '\0';
    ////生成一个新的id,通过现有用户量进行偏移实现.用户名密码的合法性交由客户端判断
    pthread_rwlock_wrlock(&(arg->idindx->rwlock));
    id = arg->idindx->cur_num + USR_FST_NUM;
    //注册成功,将id发送给客户端
    arg->package->cmd_0 = CMD_SIGNUP;
    arg->package->cmd_1 = CMD_CONFIRM;
    arg->package->sender = 0;
    arg->package->recver = id;
    sock_package_removeOtherMsg(arg->package);
    if(0 > sock_write_package(arg))
    {
        pthread_rwlock_unlock(&(arg->idindx->rwlock));
    } else {
        pthread_rwlock_unlock(&(arg->idindx->rwlock));
        //用户数据插入数据库
        db_user_sign_up(id, username, pswd);
        //用户数据在内存中缓存
        usrData_insert(id);
    }
    return NULL;
}
void *sock_signin(void *args)
{
    SockHandlerArgs *arg = args;
    int fd = arg->fd;
//    char buf[100];
    USRID id = arg->package->sender;
    if(usrData_exists(id) < 0)
    {
        ////用户不存在
        arg->package->cmd_1 = CMD_REFUSE;
        arg->package->sender = 0;
        arg->package->recver = id;
        sock_package_removeOtherMsg(arg->package);
        sock_write_package(arg);
        return NULL;
    }
    ////用户存在,返回用户信息
    char **data = db_fetch_signinUsrData(id);
    arg->package->otherMsg[strlen(arg->package->otherMsg)-1] = '\0';//去掉行末回车
    if(strcmp(arg->package->otherMsg, data[1]) == 0)
    {
        //连接成功,转发线程应向客户端返回线程启动成功的消息CMD_SIGNIN
        arg->id = id;
        sockqueue_add(sock_send, arg);
    } else {
        //登录失败
        arg->package->cmd_1 = CMD_REFUSE;
        arg->package->sender = 0;
        arg->package->recver = id;
        sock_package_removeOtherMsg(arg->package);
        sock_write_package(arg);
    }
    return NULL;
}
void *sock_talkto(void *args)
{
    printf("聊天:\n");
    SockHandlerArgs *arg = args;
    if(!usrData_exists(arg->package->recver))
    {
        ////to do sth. when user not exists
        printf("用户不存在\n");
    }
    else
    {
        printf("转发内容:\n%s\n", sock_package_toRowMessage(arg->package));
        usrData_msgqueue_insert(arg->package->recver, sock_package_toRowMessage(arg->package));
    }

    printf("talkto return \n");
    return NULL;
}
void *sock_newfriend(void *args)
{
    printf("添加好友\n");
    SockHandlerArgs *arg = args;

    if(!usrData_exists(arg->package->recver))
    {
        printf("用户不存在\n");
        return NULL;
    }

    if(arg->package->cmd_1 == CMD_CONFIRM)
    {
        if(0 != redis_newFriend(arg->package->sender, arg->package->recver))
            printf("redis new friend fail");
        return NULL;
    }

    usrData_msgqueue_insert(arg->package->recver, sock_package_toRowMessage(arg->package));
    printf("newfnd return\n");
    return NULL;
}
void *sock_getUsrInfo(void *args)
{
    printf("获取用户信息\n");
    SockHandlerArgs *arg = args;

    if(arg->package->cmd_1 == CMD_BRIEF)
    {
        char **info;
        info = db_fetch_briefUsrData(arg->package->recver);
        sock_package_removeOtherMsg(arg->package);
        arg->package->otherMsg = info[1];
        arg->package->otherMsg[strlen(info[1])] = '\n';
        usrData_msgqueue_insert(arg->package->sender, sock_package_toRowMessage(arg->package));
        printf("请求用户数据：\n%s", sock_package_toRowMessage(arg->package));
    }
    return NULL;
}

SockPackage *sock_read_package(void *args)
{
    SockHandlerArgs *arg = args;
    //检查有效性,若读取到""则说明对端关闭
    char buf[20];
    int len = get_line(arg->fd, buf, sizeof(buf));

    if(len < 0)
        error_handler("recv");
    else if(len == 0)
        return NULL;

    if(arg->package == NULL)
        arg->package = calloc(1, sizeof(SockPackage));
    arg->package->cmd_0 = atoi(buf);
    get_line(arg->fd, buf, sizeof(buf));
    arg->package->cmd_1 = atoi(buf);
    get_line(arg->fd, buf, sizeof(buf));
    arg->package->sender = atoi(buf);
    get_line(arg->fd, buf, sizeof(buf));
    arg->package->recver = atoi(buf);
    //清空otherMsg
    if(arg->package->otherMsg != NULL)
    {
        free(arg->package->otherMsg);
        arg->package->otherMsg = NULL;
    }
    //从socket缓冲区中读取otherMsg
    char msg[1024];
    char otherMsg[1024] = "";
    get_line(arg->fd, buf, sizeof(buf));//  $\n
    get_line(arg->fd, msg, sizeof(msg));
    while(strcmp(msg, "$\n") != 0)
    {
        strcat(otherMsg, msg);
        strcat(otherMsg, "\n");
        get_line(arg->fd, msg, sizeof(msg));
    }
    //读取到otherMsg
    if(strcmp(otherMsg, "") != 0)
    {
        char *finalMsg = calloc(strlen(otherMsg), sizeof(char));
        strcpy(finalMsg, otherMsg);
        arg->package->otherMsg = finalMsg;
    }
    return arg->package;
}

int sock_write_package(void *args)
{
    SockHandlerArgs *arg = args;
    const char *msg = sock_package_toRowMessage(arg->package);
    return send(arg->fd, msg, strlen(msg), 0);
}