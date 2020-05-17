//
// Created by wojiaojd on 7/5/2020.
//
#include <stdlib.h>
#include <pthread.h>
#include "pthreadpool.h"
#include "errorhandler.h"
struct threadpool *threadpool_init(int thread_num)
{
    struct threadpool *pool = NULL;
    pool = (struct threadpool*)calloc(1, sizeof(struct threadpool));
    if(!pool)
    {
        error_handler("pool_calloc");
    }
    pool->thread_num = thread_num;
    pool->pool_close = 0;
//    if(pthread_mutex_init(&(pool->mutex), NULL))
//    {
//        error_handler("pthread_mutex_init");
//    }
    pool->pthreads = calloc(thread_num, sizeof(pthread_t));
    if(!pool->pthreads)
    {
        error_handler("pool_pthreads_calloc");
    }
    for(int i = 0; i < thread_num; i++)
    {
        pthread_create(&(pool->pthreads[i]), NULL, threadpool_run, (void*)pool);
    }
    return pool;
}

void *threadpool_run(void *arg)
{
    struct threadpool *pool = (struct threadpool*)arg;
    struct sock *s = NULL;
    while(1)
    {
        s = sockqueue_get();
        //执行任务
        (*(s->callback_handler))(s->arg);
        free(s);
        s = NULL;
    }
}