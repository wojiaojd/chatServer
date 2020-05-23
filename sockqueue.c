//
// Created by wojiaojd on 7/5/2020.
//
#include <stdlib.h>
#include <assert.h>
#include "sockqueue.h"
#include "errorhandler.h"

struct sockqueue *sock_queue = NULL;

int sockqueue_init(int max_num)
{
    sock_queue = (struct sockqueue*)calloc(1, sizeof(struct sockqueue));
    if(!sock_queue)
    {
        error_handler("sockqueue_calloc");
    }
    if(pthread_mutex_init(&(sock_queue->mutex), NULL))
    {
        error_handler("squeue_pthread_mutex_init");
    }
    if(pthread_cond_init(&(sock_queue->empty), NULL))
    {
        error_handler("squeue_pthread_cond_init_empty");
    }
    if(pthread_cond_init(&(sock_queue->not_empty), NULL))
    {
        error_handler("squeue_pthread_cond_init_not_empty");
    }
    if(pthread_cond_init(&(sock_queue->not_full), NULL))
    {
        error_handler("squeue_pthread_cond_init_not_full");
    }
    sock_queue->close = 0;
    sock_queue->cur_num = 0;
    sock_queue->max_num = max_num;
    sock_queue->head = NULL;
    sock_queue->tail = NULL;
    return 0;
}

int sockqueue_add(void *(*callback_handler)(void*arg), void *arg)
{
    assert(sock_queue != NULL);
    assert(callback_handler != NULL);
    assert(arg != NULL);

    pthread_mutex_lock(&(sock_queue->mutex));
    while(sock_queue->cur_num == sock_queue->max_num && !sock_queue->close)
    {
        pthread_cond_wait(&(sock_queue->not_full), &(sock_queue->mutex));
    }
    if(sock_queue->close)
    {
        //队列已关闭,无法添加新连接
        return -1;
    }

    struct sock *s;
    s = (struct sock*)calloc(1, sizeof(struct sock));
    s->callback_handler = callback_handler;
    s->arg = arg;
    s->next = NULL;

    sock_queue->cur_num++;
    if(sock_queue->cur_num == 1)
    {
        sock_queue->head = s;
        sock_queue->tail = s;
        pthread_cond_broadcast(&(sock_queue->not_empty));
    } else {
        sock_queue->tail->next = s;
        sock_queue->tail = s;
    }
    pthread_mutex_unlock(&(sock_queue->mutex));
    //printf("添加连接\n");
    return 0;
}
struct sock *sockqueue_get()
{
    struct sock *s = NULL;
    pthread_mutex_lock(&(sock_queue->mutex));
    while(sock_queue->cur_num == 0 && !sock_queue->close)
    {
        pthread_cond_wait(&(sock_queue->not_empty), &(sock_queue->mutex));
    }
    if(sock_queue->close)
    {
        //连接队列已关闭,不再生产新任务
        pthread_mutex_unlock(&(sock_queue->mutex));
        pthread_exit(NULL);
    }
    //线程从队列中取一个任务
    sock_queue->cur_num--;
    s = sock_queue->head;
    if(sock_queue->cur_num == 0)
    {
        sock_queue->head = NULL;
        sock_queue->tail = NULL;
        pthread_cond_signal(&(sock_queue->empty));
    } else {
        sock_queue->head = s->next;
    }
    if(sock_queue->cur_num == sock_queue->max_num - 1)
    {
        pthread_cond_broadcast(&(sock_queue->not_full));
    }
    pthread_mutex_unlock(&(sock_queue->mutex));
    return s;
}