//
// Created by wojiaojd on 9/5/2020.
//

#ifndef CHATSERVER_DATABASE_H
#define CHATSERVER_DATABASE_H

#endif //CHATSERVER_DATABASE_H
#include <mysql/mysql.h>

#define DB_HOST "localhost"
#define DB_USR "root"
#define DB_PSW "wojiaojd"
#define DB_NAME "mysql"
#define DB_PORT 3306

typedef MYSQL DATABASE;

DATABASE *db_startup();
DATABASE *db_getInstance();
int db_user_sign_up(int id, char *username, char *password);
char **db_fetch_usrData(int id);