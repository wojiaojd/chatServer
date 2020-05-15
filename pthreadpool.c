//
// Created by wojiaojd on 7/5/2020.
//
#include <stdlib.h>
#include <pthread.h>
#include "pthreadpool.h"
#include "errorhandler.h"
struct threadpool *threadpool_init(int thread_num, struct sockqueue *sock_queue)
{
    struct threadpool *pool = NULL;
    pool = (struct threadpool*)calloc(1, sizeof(struct threadpool));
    if(!pool)
    {
        error_handler("pool_calloc");
    }
    pool->thread_num = thread_num;
    pool->sock_queue = sock_queue;
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
        pthread_mutex_lock(&(pool->sock_queue->mutex));
        while(pool->sock_queue->cur_num == 0 && !pool->sock_queue->close)
        {
            pthread_cond_wait(&(pool->sock_queue->not_empty), &(pool->sock_queue->mutex));
        }
        if(pool->sock_queue->close)
        {
            //连接队列已关闭,不再生产新任务
            pthread_mutex_unlock(&(pool->sock_queue->mutex));
            pthread_exit(NULL);
        }
        //线程从队列中取一个任务
        pool->sock_queue->cur_num--;
        s = pool->sock_queue->head;
        if(pool->sock_queue->cur_num == 0)
        {
            pool->sock_queue->head = NULL;
            pool->sock_queue->tail = NULL;
            pthread_cond_signal(&(pool->sock_queue->empty));
        } else {
            pool->sock_queue->head = s->next;
        }
        if(pool->sock_queue->cur_num == pool->sock_queue->max_num - 1)
        {
            pthread_cond_broadcast(&(pool->sock_queue->not_full));
        }
        pthread_mutex_unlock(&(pool->sock_queue->mutex));
        //执行任务
        (*(s->callback_handler))(s->arg);
        free(s);
        s = NULL;
    }
}