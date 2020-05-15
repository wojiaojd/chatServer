//
// Created by wojiaojd on 7/5/2020.
//
#include <stdlib.h>
#include <assert.h>
#include "sockqueue.h"
#include "errorhandler.h"

struct sockqueue *sockqueue_init(int max_num)
{
    struct sockqueue *squeue;
    squeue = (struct sockqueue*)calloc(1, sizeof(struct sockqueue));
    if(!squeue)
    {
        error_handler("sockqueue_calloc");
    }
    if(pthread_mutex_init(&(squeue->mutex), NULL))
    {
        error_handler("squeue_pthread_mutex_init");
    }
    if(pthread_cond_init(&(squeue->empty), NULL))
    {
        error_handler("squeue_pthread_cond_init_empty");
    }
    if(pthread_cond_init(&(squeue->not_empty), NULL))
    {
        error_handler("squeue_pthread_cond_init_not_empty");
    }
    if(pthread_cond_init(&(squeue->not_full), NULL))
    {
        error_handler("squeue_pthread_cond_init_not_full");
    }
    squeue->close = 0;
    squeue->cur_num = 0;
    squeue->max_num = max_num;
    squeue->head = NULL;
    squeue->tail = NULL;
    return squeue;
}

int sockqueue_add(struct sockqueue* squeue, void *(*callback_handler)(void*arg), void *arg)
{
    assert(squeue != NULL);
    assert(callback_handler != NULL);
    assert(arg != NULL);

    pthread_mutex_lock(&(squeue->mutex));
    while(squeue->cur_num == squeue->max_num && !squeue->close)
    {
        pthread_cond_wait(&(squeue->not_full), &(squeue->mutex));
    }
    if(squeue->close)
    {
        //队列已关闭,无法添加新连接
        return -1;
    }

    struct sock *s;
    s = (struct sock*)calloc(1, sizeof(struct sock));
    s->callback_handler = callback_handler;
    s->arg = arg;
    s->next = NULL;

    squeue->cur_num++;
    if(squeue->cur_num == 1)
    {
        squeue->head = s;
        squeue->tail = s;
        pthread_cond_broadcast(&(squeue->not_empty));
    } else {
        squeue->tail->next = s;
        squeue->tail = s;
    }
    pthread_mutex_unlock(&(squeue->mutex));
    return 0;
}