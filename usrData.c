//
// Created by wojiaojd on on 8/5/2020.
//
#include <stdlib.h>
#include <assert.h>
#include "usrData.h"
#include "errorhandler.h"


struct usrData *indexing = NULL;

struct msgqueue *msgqueue_init(int max_num)
{
    struct msgqueue *msg_queue = NULL;
    msg_queue = (struct msgqueue*)calloc(1, sizeof(struct msgqueue));
    if(!msg_queue)
    {
        error_handler("msg_queue_calloc");
    }
    if(pthread_mutex_init(&(msg_queue->mutex), NULL)){
        error_handler("msg_queue_mutex_init");
    }
    if(pthread_cond_init(&(msg_queue->empty), NULL))
    {
        error_handler("msg_queue_cond_init_empty");
    }
    if(pthread_cond_init(&(msg_queue->not_empty), NULL))
    {
        error_handler("msg_queue_cond_init_not_empty");
    }
    if(pthread_cond_init(&(msg_queue->not_full), NULL))
    {
        error_handler("msg_queue_cond_init_not_full");
    }
    msg_queue->cur_num = 0;
    msg_queue->max_num = max_num;
    msg_queue->head = NULL;
    msg_queue->tail = NULL;
    msg_queue->close = 0;
    return msg_queue;
}

struct msg *msg_init()
{
    struct msg *m = NULL;
    m = (struct msg*)calloc(1, sizeof(struct msg));
    if(!m)
    {
        error_handler("msg_init_calloc");
    }
    m->content = NULL;
    m->next = NULL;
    return m;
}

int msgqueue_insert(struct msgqueue *msg_queue, char *real_msg)
{
    assert(msg_queue != NULL);
    assert(real_msg != NULL);

    pthread_mutex_lock(&(msg_queue->mutex));
    while(msg_queue->cur_num == msg_queue->max_num && !msg_queue->close)
    {
        pthread_cond_wait(&(msg_queue->not_full), &(msg_queue->mutex));
    }
    if(msg_queue->close)
    {
        pthread_mutex_unlock(&(msg_queue->mutex));
        return -1;
    }
    struct msg *m;
    m = msg_init();
    m->content = real_msg;
    msg_queue->cur_num++;
    if(msg_queue->cur_num == 1)
    {
        msg_queue->head = m;
        msg_queue->tail = m;
        pthread_cond_broadcast(&(msg_queue->not_empty));
    } else {
        msg_queue->tail->next = m;
        msg_queue->tail = m;
    }
    pthread_mutex_unlock(&(msg_queue->mutex));
    return 0;
}
//取出头节点
struct msg *msgqueue_pop(struct msgqueue *msg_queue)
{
    struct msg *m = NULL;
    pthread_mutex_lock(&(msg_queue->mutex));
    while(msg_queue->cur_num == 0 && !msg_queue->close)
    {
        pthread_cond_wait(&(msg_queue->not_empty), &(msg_queue->mutex));
    }
    if(msg_queue->close)
    {
        pthread_mutex_unlock(&(msg_queue->mutex));
        return NULL;
    }

    //取出节点
    msg_queue->cur_num--;
    m = msg_queue->head;
    if(msg_queue->cur_num == 0)
    {
        msg_queue->head = NULL;
        msg_queue->tail = NULL;
        pthread_cond_signal(&(msg_queue->empty));
    } else {
        msg_queue->head = m->next;
    }
    pthread_mutex_unlock(&(msg_queue->mutex));
    return m;
}

struct usrData *idindx_init()
{
    if(indexing != NULL)
    {
        return indexing;
    }
    indexing = (struct usrData*)calloc(1, sizeof(struct usrData));
    for(int i = 0; i < USR_MAX_NUM; i++)
    {
        indexing->data[i] = NULL;
    }
    if(pthread_rwlock_init(&(indexing->rwlock), NULL))
    {
        error_handler("usrDataArray_mutex_r_init");
    }

    indexing->cur_num = 0;
    return indexing;
}
///用户数据结构或可改进,比如改成红黑树
int usrData_insert(struct usrData* dataArray, struct id_data *idData)
{
    int index = idData->id - USR_FST_NUM;
    if(dataArray->data[index] != NULL)
    {
        return -1;
    }
    dataArray->data[index] = idData;
    return 0;
}
///通过id查找用户数据
struct id_data *usrData_find(struct usrData *dataArray, int id)
{
    return dataArray->data[id];
}