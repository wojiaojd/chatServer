//
// Created by wojiaojd on 11/5/2020.
// 本结构应常驻内存
//

#ifndef CHATSERVER_IDINDEXING_H
#define CHATSERVER_IDINDEXING_H

#endif //CHATSERVER_IDINDEXING_H

#include <pthread.h>
#include <hiredis/hiredis.h>
#include "rbTree.h"


#define MSGQUEUE_MAX_NUM 100
#define USR_MAX_NUM 10000
#define USR_FST_NUM 10000  //起始号码
typedef int USRID ;

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
    USRID id;
    struct msgqueue *sndqueue;
};

struct usrData{
    int cur_num;
    pthread_rwlock_t rwlock;
    struct id_data *data[USR_MAX_NUM];
};

struct msg *msg_init();
int msg_free(struct msg *m);

struct msgqueue *msgqueue_init(int max_num);

int usrData_msgqueue_insert(USRID usrid, char *real_msg);
struct msg *usrData_msgqueue_pop(USRID usrid);

struct usrData *usrData_init();
/*用户注册,在用户数据数组中分配一个节点*/
int usrData_insert(USRID usr_id);
struct id_data *usrData_find(USRID id);
int usrData_exists(USRID id);
int usrData_close(USRID id);
int usrData_signin(USRID id, int fd);

redisContext *redis_getInstance();
int redis_newFriend(USRID my_id, USRID friend_id);