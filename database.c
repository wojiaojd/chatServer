//
// Created by wojiaojd on 9/5/2020.
//
#include <stdio.h>
#include <string.h>
#include "database.h"
#include "errorhandler.h"

DATABASE *chat_database_ptr = NULL;
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
    if(!mysql_real_connect(chat_database_ptr, DB_HOST, DB_USR, DB_PSW, DB_NAME, DB_PORT, NULL, 0))
    {
        error_handler("dabase_connect");
    }
    printf("database connect successful\n");
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
    printf("db_user_sign_up\n");
    char query[1024];
    mysql_real_escape_string(chat_database_ptr, username, username, strlen(username));
    mysql_real_escape_string(chat_database_ptr, password, password, strlen(password));
    sprintf(query, "INSERT INTO USER(id, username, password) VALUES (%d, '%s', '%s')", id, username, password);

    printf("%s\n", query);
    if(mysql_real_query(chat_database_ptr, query, strlen(query)))
    {
        error_handler("db_user_sign_up");
    }
    return 0;
}