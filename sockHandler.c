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
void *(*commandSwitch[3])(void*)={sock_signup, sock_signin, sock_talkto};

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
    struct sockHandlerArgs *arg = args;
    struct msg *m = NULL;
    int client = arg->fd;
    char buf[10];
    while(1)
    {
        int len = get_line(client, buf, sizeof(buf));
        if(len > 0) {
            enum COMMAND command = atoi(buf);
            (*(commandSwitch[command]))(args);

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
    struct sockHandlerArgs *arg = args;
    char buf[100];
    printf("用户数据登录操作\n");
    usrData_signin(arg->id, arg->fd);
    ////通知对端连接成功可以继续发送消息
    sprintf(buf, "%d\n", CMD_SIGNIN);
    printf("登录确认前\n");
    if(0 > send(arg->fd, buf, strlen(buf), 0))
    {
        return NULL;
    }
    printf("发送登录确认\n");
    struct msg *m = NULL;
    while((m = usrData_msgqueue_pop(arg->id)) != NULL)
    {
        ////转发
        send(arg->fd, m->content, strlen(m->content), 0);
        msg_free(m);
    }
    return NULL;
}

/*用户注册过程*/
void *sock_signup(void *args)
{
    struct sockHandlerArgs *arg = args;
    USRID id;
    //避免mysql_real_escape_string过程中全部都是需转义的字符,长度应为2*n+1
    char username[101];
    char pswd[201];
    ////生成一个新的id,通过现有用户量进行偏移实现.用户名密码的合法性交由客户端判断
    pthread_rwlock_wrlock(&(arg->idindx->rwlock));
    id = arg->idindx->cur_num + USR_FST_NUM;
    printf("new user id: %d\n", id);
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
        DATABASE *db = db_getInstance();
        db_user_sign_up(id, username, pswd);
        usrData_insert(id);
    }
    return NULL;
}
void *sock_signin(void *args)
{
    struct sockHandlerArgs *arg = args;
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
    printf("获取密码:%s\n", buf);
    char **data = db_fetch_usrData(id);
    printf("从数据库获取用户数据\n");
    if(strcmp(buf, data[1]) == 0)
    {
        //连接成功,转发线程应向客户端返回线程启动成功的消息CMD_SIGNIN
        arg->id = id;
        sockqueue_add(sock_send, arg);
    } else {
        sprintf(buf, "%d\n", CMD_REFUSE);
        send(fd, buf, strlen(buf), 0);
    }

}
void *sock_talkto(void *args)
{

}