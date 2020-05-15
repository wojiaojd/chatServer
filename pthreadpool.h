//
// Created by wojiaojd on 7/5/2020.
//

#ifndef CHATSERVER_PTHREADPOLL_H
#define CHATSERVER_PTHREADPOLL_H

#endif //CHATSERVER_PTHREADPOLL_H

#include <pthread.h>
#include "sockqueue.h"
#define POOL_MAX_NUM 10

struct threadpool{
    int thread_num;
    pthread_t *pthreads;
//    pthread_mutex_t mutex;
    struct sockqueue *sock_queue;
    int pool_close;
};
////threadpool主动请求sock_queue中的工作,在这里即是获取用户连接
struct threadpool *threadpool_init(int thread_num, struct sockqueue *sock_queue);
////抽象工作线程
void *threadpool_run(void *arg);
////线程池的扩展动作