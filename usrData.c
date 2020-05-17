//
// Created by wojiaojd on on 8/5/2020.
//
#include <stdlib.h>
#include <assert.h>
#include "usrData.h"
#include "errorhandler.h"


struct usrData *usr_data = NULL;
////特别用来保存用户的好友关系
redisContext *redisConn = NULL;

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

int usrData_msgqueue_insert(USRID usrid, char *real_msg)
{
    assert(real_msg != NULL);
    struct id_data *idData = usrData_find(usrid);
    struct msgqueue *snd_queue = idData->sndqueue;
    pthread_mutex_lock(&(snd_queue->mutex));
    while(snd_queue->cur_num == snd_queue->max_num && !snd_queue->close)
    {
        pthread_cond_wait(&(snd_queue->not_full), &(snd_queue->mutex));
    }
    if(snd_queue->close)
    {
        pthread_mutex_unlock(&(snd_queue->mutex));
        return -1;
    }
    struct msg *m;
    m = msg_init();
    m->content = real_msg;
    snd_queue->cur_num++;
    if(snd_queue->cur_num == 1)
    {
        snd_queue->head = m;
        snd_queue->tail = m;
        pthread_cond_broadcast(&(snd_queue->not_empty));
    } else {
        snd_queue->tail->next = m;
        snd_queue->tail = m;
    }
    pthread_mutex_unlock(&(snd_queue->mutex));
    return 0;
}
//取出头节点
struct msg *usrData_msgqueue_pop(USRID usrid)
{
    struct msg *m = NULL;
    struct id_data *idData = usrData_find(usrid);
    struct msgqueue *snd_queue = idData->sndqueue;
    pthread_mutex_lock(&(snd_queue->mutex));
    while(snd_queue->cur_num == 0 && !snd_queue->close)
    {
        pthread_cond_wait(&(snd_queue->not_empty), &(snd_queue->mutex));
    }
    if(snd_queue->close)
    {
        pthread_mutex_unlock(&(snd_queue->mutex));
        return NULL;
    }

    //取出节点
    snd_queue->cur_num--;
    m = snd_queue->head;
    if(snd_queue->cur_num == 0)
    {
        snd_queue->head = NULL;
        snd_queue->tail = NULL;
        pthread_cond_signal(&(snd_queue->empty));
    } else {
        snd_queue->head = m->next;
    }
    pthread_mutex_unlock(&(snd_queue->mutex));
    return m;
}

struct usrData *usrData_init()
{
    if(usr_data != NULL)
    {
        return usr_data;
    }
    usr_data = (struct usrData*)calloc(1, sizeof(struct usrData));
    for(int i = 0; i < USR_MAX_NUM; i++)
    {
        usr_data->data[i] = NULL;
    }
    if(pthread_rwlock_init(&(usr_data->rwlock), NULL))
    {
        error_handler("usrDataArray_mutex_r_init");
    }

    usr_data->cur_num = 0;
    return usr_data;
}
///用户数据结构或可改进,比如改成红黑树
int usrData_insert(USRID usrid)
{
    struct id_data *idData = NULL;
    idData = calloc(1, sizeof(struct id_data));
    idData->id = usrid;
    idData->fd = 0;
    idData->sndqueue = msgqueue_init(MSGQUEUE_MAX_NUM);
    USRID index = usrid - USR_FST_NUM;
    if(usr_data->data[index] != NULL)
    {
        return -1;
    }
    usr_data->data[index] = idData;
    return 0;
}
///通过id查找用户数据
struct id_data *usrData_find(USRID id)
{
    return usr_data->data[id];
}

int usrData_exists(USRID id)
{
    if(usr_data->data[id] != NULL)
    {
        return 0;
    } else {
        return -1;
    }
}
redisContext *redis_getInstance()
{
    if(redisConn != NULL)
    {
        return redisConn;
    }
    redisConn = redisConnect("127.0.0.1", 6379);
    if(redisConn->err)
    {
        return NULL;
    }
    return redisConn;
}