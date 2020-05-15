//
// Created by wojiaojd on 11/5/2020.
// 本结构应常驻内存
//

#ifndef CHATSERVER_IDINDEXING_H
#define CHATSERVER_IDINDEXING_H

#endif //CHATSERVER_IDINDEXING_H

#include <pthread.h>

#define MSGQUEUE_MAX_NUM 10
#define USR_MAX_NUM 10000
#define USR_FST_NUM 10000  //起始号码

struct msg{
    struct msg *next;
    char *content;
};

struct msgqueue{
    struct msg *head;
    struct msg *tail;
    int max_num;
    int cur_num;
    int close;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

struct id_data{
    int fd;
    int id;
    struct msgqueue *sndqueue;
};

struct usrData{
    int cur_num;
    pthread_rwlock_t rwlock;
    struct id_data *data[USR_MAX_NUM];
};

struct msg *msg_init();

struct msgqueue *msgqueue_init(int max_num);
int msgqueue_insert(struct msgqueue *msg_queue, char *real_msg);
struct msg *msgqueue_pop(struct msgqueue *msg_queue);

struct usrData *idindx_init();
/*用户注册,在用户数据数组中分配一个节点*/
int usrData_insert(struct usrData* dataArray, struct id_data *idData);
struct id_data *usrData_find(struct usrData *dataArray, int id);