//
// Created by wojiaojd on 1/5/2020.
//
#include <sys/epoll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "errorhandler.h"
#include "pthreadpool.h"
#include "sockHandler.h"
#include "database.h"

#define EPOLL_SIZE 30
#define LISTEN_BUF_SIZE 5
#define PORT 9999


void *sock_handler(void *ev);
int startup(short port);

////将消息保存到文件中


////处理来自客户端的连接
/*
void* sock_handler2(void *args)
{
    printf("sock_handler\n");
    struct sockHandlerArgs *arg = (struct sockHandlerArgs*)args;
    int client = arg->epev->data.fd;
    if(arg->epev->events & EPOLLIN)
    {
        char buf[1024];
        size_t len;
        printf("recv\n");
        len = recv(client, buf, sizeof(buf)-1, 0);
        printf("server recv :len:%d, content:%s\n", (int)len, buf);
        if(0 > len)
        {
            error_handler("recv");
        }
        //close
        if(0 == len || (arg->epev->events & EPOLLRDHUP))
        {
            printf("EPOLL_CTL_DEL\n");
            if(-1 == epoll_ctl(arg->epfd, EPOLL_CTL_DEL, client, NULL))
            {
                error_handler("epoll_ctl_del");
            }
            if(-1 == close(client))
            {
                error_handler("EPOLL close clientsock");
            }
            return NULL;
        }
        msg_save(buf, len);

        sprintf(buf, "%s", "i am server.i recv your msg.\n");
        if(0 > send(client, buf, strlen(buf), 0))
        {
            error_handler("server_send");
        }
    } else {
        printf("no EPOLLIN");
    }

}
*/
////启动监听
int startup(short port)
{
    int serversock;
    serversock = socket(AF_INET, SOCK_STREAM, 0);
    if(serversock < 0)
    {
        error_handler("serversock");
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;
    if(0 != setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        error_handler("setsockopt");
    }
    if(0 != bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)))
    {
        error_handler("bind");
    }

    if(0 != listen(serversock, LISTEN_BUF_SIZE))
    {
        error_handler("listen");
    }
    return serversock;
}

////主流程
int main()
{
    int serversock = -1;
    serversock = startup(PORT);

    /*加入数据库功能*/
    DATABASE *database;
    database = db_startup();

    /*用户id索引数据数据结构*/
    struct usrData *idindx;
    idindx = idindx_init();

    /*添加线程池机制*/
    struct sockqueue *squeue;
    squeue = sockqueue_init(SOCKQUEUE_MAX_NUM);
    struct threadpool *pool;
    pool = threadpool_init(POOL_MAX_NUM, squeue);

    /*添加epoll机制监听事件*/
    int epnum = -1;
    struct epoll_event ev;
    struct epoll_event evlist[EPOLL_SIZE];

    int epfd = -1;
    epfd = epoll_create(EPOLL_SIZE);
    if(0 > epfd)
    {
        error_handler("epoll_creat");
    }

    ev.events = EPOLLIN;
    ev.data.fd = serversock;
    //第一个受监听的连接:服务器的套接字
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, serversock, &ev) == -1)
    {
        error_handler("epoll_ctl_add_server");
    }
    /*epoll*/

    int clientsock = -1;
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);//在这里初始化后再传入accept()

    while(1)
    {
        printf("%s\r\n", "server is ready to accept...");
        epnum = epoll_wait(epfd, evlist, EPOLL_SIZE, -1);
        if(epnum == -1)
        {
            error_handler("epoll_wait");
        }
        //处理就绪连接

        for(int i = 0; i < epnum; i++)
        {
            //服务器收到连接请求

            if(evlist[i].data.fd == serversock)
            {
                clientsock = accept(serversock, (struct sockaddr*)&clientaddr, &addrlen);
                if(0 > clientsock)
                {
                    error_handler("accept");
                }

                /*//边缘触发通知
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientsock;
                if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, clientsock, &ev))
                {
                    error_handler("epoll_ctl_add_client");
                }
                 */
                /*线程池主动工作*/
                struct sockHandlerArgs arg = {clientsock, idindx};
                sockqueue_add(squeue, sock_recv, &arg);
                /*clientsock*/
            }
            else
            {
                /*处理客户端*/
                printf("客户端:i=%d, sock = %d\n", i, evlist[i].data.fd);

            }

        }/*for*/

    }/*while*/

}

