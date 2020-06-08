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
        int len = get_line(client, buf, sizeof(buf));
        if(len > 0) {
            enum COMMAND command = atoi(buf);
            printf("COMMAND:%d\n", command);
//            (*(commandSwitch[command]))(args);
            cmd_switch(command, args);

        } else if(len == 0){
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
        } else {
            error_handler("recv");
        }
    }
}
void *sock_send(void *args)
{
    SockHandlerArgs *arg = args;
    char buf[100];
    usrData_signin(arg->id, arg->fd);
    ////通知对端连接成功可以继续发送消息
    sprintf(buf, "%d\n", CMD_SIGNIN);
    if(0 > send(arg->fd, buf, strlen(buf), 0))
    {
        return NULL;
    }
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
    ////生成一个新的id,通过现有用户量进行偏移实现.用户名密码的合法性交由客户端判断
    pthread_rwlock_wrlock(&(arg->idindx->rwlock));
    id = arg->idindx->cur_num + USR_FST_NUM;
    //注册成功,将id发送给客户端
    char buf[20];
    sprintf(buf, "%d\n%d\n", CMD_SIGNUP, id);
    if(0 > send(arg->fd, buf, strlen(buf), 0))
    {
        pthread_rwlock_unlock(&(arg->idindx->rwlock));
    } else {

        arg->idindx->cur_num++;
        pthread_rwlock_unlock(&(arg->idindx->rwlock));
        get_line(arg->fd, username, sizeof(username));
        get_line(arg->fd, pswd, sizeof(pswd));
        db_user_sign_up(id, username, pswd);
        usrData_insert(id);
    }
    return NULL;
}
void *sock_signin(void *args)
{
    SockHandlerArgs *arg = args;
    int fd = arg->fd;
    char buf[100];
    get_line(fd, buf, sizeof(buf));
    USRID id = atoi(buf);
    if(usrData_exists(id) < 0)
    {
        ////用户不存在
        sprintf(buf, "%d\n", CMD_REFUSE);
        send(fd, buf, strlen(buf), 0);
        return NULL;
    }
    ////用户存在,返回用户信息
    get_line(fd, buf, sizeof(buf));
    char **data = db_fetch_usrData(id);
    if(strcmp(buf, data[1]) == 0)
    {
        //连接成功,转发线程应向客户端返回线程启动成功的消息CMD_SIGNIN
        arg->id = id;
        sockqueue_add(sock_send, arg);
    } else {
        sprintf(buf, "%d\n", CMD_REFUSE);
        send(fd, buf, strlen(buf), 0);
    }
    return NULL;
}
void *sock_talkto(void *args)
{
    printf("聊天:\n");
    SockHandlerArgs *arg = args;
    int fd = arg->fd;
    char sndBuf[1024];
    char buf[1024];
    char toId[10];
    get_line(fd, toId, sizeof(toId));
    USRID to_id = atoi(toId);

    sprintf(sndBuf, "%d\n%d\n", CMD_TALKTO, arg->id);
    get_line(fd, buf, sizeof(buf));
    strcat(sndBuf, "$\n");
    do{
        get_line(fd, buf, sizeof(buf));
        strcat(sndBuf, buf);
        strcat(sndBuf, "\n");
    } while (strcmp(buf, "$"));

    if(!usrData_exists(to_id))
    {
        ////to do sth. when user not exists
        printf("用户不存在\n");

    } else
    {
        printf("转发内容:\n%s\n", sndBuf);
        usrData_msgqueue_insert(to_id, sndBuf);
    }

    printf("talkto return \n");
    return NULL;
}
void *sock_newfriend(void *args)
{
    printf("添加好友\n");
    SockHandlerArgs *arg;
    int fd;
    char sendbuf[1024];
    char buf[1024];
    int cmd;
    USRID sender_id, recver_id;

    arg = args;
    fd = arg->fd;
    get_line(fd, buf, sizeof(buf));
    cmd = atoi(buf);
    get_line(fd, buf, sizeof(buf));
    sender_id = atoi(buf);
    get_line(fd, buf, sizeof(buf));
    recver_id = atoi(buf);

    sprintf(sendbuf, "%d\n%d\n%d\n%d\n", CMD_NEWFND, cmd, sender_id, recver_id);
    strcat(sendbuf, "$\n");
    do{
        get_line(fd, buf, sizeof(buf));
        strcat(sendbuf, buf);
        strcat(sendbuf, "\n");
    } while (strcmp(buf, "$"));

    if(!usrData_exists(recver_id))
    {
        printf("用户不存在\n");
        return NULL;
    }

    if(cmd == CMD_CONFIRM)
    {
        if(0 != redis_newFriend(sender_id, recver_id))
            printf("redis new friend fail");
        return NULL;
    }

    usrData_msgqueue_insert(recver_id, sendbuf);
    printf("newfnd return\n");
    return NULL;
}