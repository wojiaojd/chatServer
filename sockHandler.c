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
    struct sockHandlerArgs *arg = (struct sockHandlerArgs *)args;
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
            printf("client logout\n");
            /*释放资源*/
            /*------*/
            close(client);
            printf("client return \n");
            return NULL;
        } else {
            error_handler("recv");
        }
    }
}

/*用户注册过程*/
void *sock_signup(void *args)
{
    struct sockHandlerArgs *arg = args;
    int id;
    //避免mysql_real_escape_string过程中全部都是需转义的字符,长度应为2*n+1
    char username[101];
    char pswd[201];
    ////生成一个新的id,通过现有用户量进行偏移实现.用户名密码的合法性交由客户端判断
    pthread_rwlock_wrlock(&(arg->idindx->rwlock));
    id = arg->idindx->cur_num + USR_FST_NUM;
    //注册成功,将id发送给客户端
    char buf[20];
    sprintf(buf, "%d\n%d\n", SIGNUP, id);
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
    }
    return NULL;
}
void *sock_signin(void *args)
{

}
void *sock_talkto(void *args)
{

}