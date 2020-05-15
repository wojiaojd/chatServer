//
// Created by wojiaojd on 7/5/2020.
//

#ifndef CHATSERVER_SOCK_QUEUE_H
#define CHATSERVER_SOCK_QUEUE_H

#endif //CHATSERVER_SOCK_QUEUE_H
#include <pthread.h>
#define SOCKQUEUE_MAX_NUM 10

struct sock{
    void *(*callback_handler)(void *arg);
    void *arg;//参数
    struct sock *next;
};
struct sockqueue{
    int max_num;
    int cur_num;
    struct sock *head;
    struct sock *tail;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    int close;
};

struct sockqueue *sockqueue_init(int max_num);

int sockqueue_add(struct sockqueue* squeue, void *(*callback_handler)(void*arg), void *arg);