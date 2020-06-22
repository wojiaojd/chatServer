//
// Created by wojiaojd on 9/5/2020.
//
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "database.h"
#include "errorhandler.h"

DATABASE *chat_database_ptr = NULL;
pthread_mutex_t db_mutex;
DATABASE *db_startup()
{
    if(chat_database_ptr != NULL)
    {
        return chat_database_ptr;
    }
    chat_database_ptr = mysql_init(NULL);
    if(chat_database_ptr == NULL)
    {
        error_handler("chat_database_ptr_init");
    }
    if(pthread_mutex_init(&(db_mutex), NULL))
    {
        error_handler("db_mutex_init");
    }
    if(!mysql_real_connect(chat_database_ptr, DB_HOST, DB_USR, DB_PSW, DB_NAME, DB_PORT, NULL, 0))
    {
        error_handler("dabase_connect");
    }
    char *query;
    query = "CREATE DATABASE IF NOT EXISTS CHAT_DATABASE";
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        error_handler("create_database_query");
    }
    query = "USE CHAT_DATABASE";
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        error_handler("use_chat_database");
    }
    query = "CREATE TABLE IF NOT EXISTS `USER`("
            "`id` int(11) NOT NULL,"
            "`username` varchar(50) DEFAULT NULL,"
            "`password` varchar(100) NOT NULL,"
            "PRIMARY KEY (`id`)"
            ")";
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        error_handler("create table user");
    }
    return chat_database_ptr;
}
DATABASE *db_getInstance()
{
    return chat_database_ptr;
}
int db_user_sign_up(int id, char *username, char *password)
{
    char query[1024];
    mysql_real_escape_string(chat_database_ptr, username, username, strlen(username));
    mysql_real_escape_string(chat_database_ptr, password, password, strlen(password));
    sprintf(query, "INSERT INTO USER(id, username, password) VALUES (%d, '%s', '%s')", id, username, password);
    pthread_mutex_lock(&(db_mutex));
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        pthread_mutex_unlock(&(db_mutex));
        error_handler("db_user_sign_up");
    }
    pthread_mutex_unlock(&(db_mutex));
    return 0;
}

char **db_fetch_signinUsrData(int id)
{
    MYSQL_ROW row = NULL;
//    char ID[20];
    char query[100];
    sprintf(query, "SELECT username, password from USER where id = %d", id);
    pthread_mutex_lock(&(db_mutex));
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        pthread_mutex_unlock(&(db_mutex));
        error_handler("select query false");
    }
    MYSQL_RES *result = mysql_store_result(chat_database_ptr);
    row = mysql_fetch_row(result);
    mysql_free_result(result);
    pthread_mutex_unlock(&(db_mutex));
    return row;
}
char **db_fetch_briefUsrData(int id)
{
    MYSQL_ROW row = NULL;
//    char ID[20];
    char query[200];
    sprintf(query, "SELECT id, username from USER where id = %d", id);
    pthread_mutex_lock(&(db_mutex));
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        pthread_mutex_unlock(&(db_mutex));
        error_handler("select query false");
    }
    MYSQL_RES *result = mysql_store_result(chat_database_ptr);
    row = mysql_fetch_row(result);
    mysql_free_result(result);
    pthread_mutex_unlock(&(db_mutex));
    return row;
}