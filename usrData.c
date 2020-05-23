//
// Created by wojiaojd on on 8/5/2020.
//
#include <stdlib.h>
#include <assert.h>
#include "usrData.h"
#include "errorhandler.h"


USRData *usr_data = NULL;
////特别用来保存用户的好友关系
redisContext *redisConn = NULL;

MSGQueue *msgqueue_init(int max_num)
{
    MSGQueue *msg_queue = NULL;
    msg_queue = (MSGQueue*)calloc(1, sizeof(MSGQueue));
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

Msg *msg_init()
{
    Msg *m = NULL;
    m = (Msg*)calloc(1, sizeof(Msg));
    if(!m)
    {
        error_handler("msg_init_calloc");
    }
    m->content = NULL;
    m->next = NULL;
    return m;
}
int msg_free(Msg *m)
{
    if(m == NULL)
    {
        return -1;
    }
    free(m);
    return 0;
}

int usrData_msgqueue_insert(USRID usrid, char *real_msg)
{
    assert(real_msg != NULL);
    printf("msg insert %d\n", usrid);
    IDData *idData = usrData_find(usrid);
    if(idData == NULL)
    {
        printf("用户不存在\n");
        return -1;
    }
    MSGQueue *snd_queue = idData->sndqueue;
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
    Msg *m;
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
Msg *usrData_msgqueue_pop(USRID usrid)
{
    Msg *m = NULL;
    IDData *idData = usrData_find(usrid);
    MSGQueue *snd_queue = idData->sndqueue;
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

int my_cmp(void *left_key, void *right_key)
{
    USRID *l = left_key;
    int *r = right_key;
    if(*l < *r)
        return -1;
    else if(*l > *r)
        return 1;
    else
        return 0;
}

USRData *usrData_init()
{
    if(usr_data != NULL)
    {
        return usr_data;
    }
    usr_data = (USRData *)calloc(1, sizeof(USRData));
    usr_data->rbTree = calloc(1, sizeof(RBTree));
    usr_data->rbTree->cmp = my_cmp;
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
    IDData *idData = NULL;
    idData = calloc(1, sizeof(IDData));
    idData->id = usrid;
    idData->fd = 0;
    idData->sndqueue = msgqueue_init(MSGQUEUE_MAX_NUM);

    Node *node = rbt_new_node();
    node->key = calloc(1, sizeof(USRID));
    *(USRID*)node->key = usrid;
    node->value = idData;
    rbt_insert(usr_data->rbTree, node);
    return 0;
}
///通过id查找用户数据
IDData *usrData_find(USRID id)
{
    Node *p = NULL;
    p = usr_data->rbTree->root;
    int (*compare)(void*, void*);
    compare = usr_data->rbTree->cmp;
    void *pId = &id;
    while(p)
    {
        int res = (*compare)(pId, p->key);
        if(res < 0)
            p = p->left;
        else if(res > 0)
            p = p->right;
        else
            break;
    }
    return p != NULL ? p->value : NULL;
}

int usrData_exists(USRID id)
{
    IDData *temp = usrData_find(id);
    if(temp)
        return 1;
    else
        return 0;
}
int usrData_close(USRID id)
{
    printf("usrData_close\n");
    IDData *data = usrData_find(id);
    if(data == NULL)
    {
        printf("usrData_close data is null\n");
        return -1;
    }
    pthread_mutex_lock(&(data->sndqueue->mutex));
    data->fd = 0;
    if(data->sndqueue->close == 1)
    {
        pthread_mutex_unlock(&(data->sndqueue->mutex));
        return -1;
    }
    data->sndqueue->close = 1;
    pthread_cond_signal(&(data->sndqueue->not_empty));
    pthread_mutex_unlock(&(data->sndqueue->mutex));
    return 0;
}
int usrData_signin(USRID id, int fd)
{
    IDData *id_data;
    id_data = usrData_find(id);
    if(!id_data)
        return -1;
    id_data->fd = fd;
    return 0;
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