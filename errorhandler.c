//
// Created by wojiaojd on 7/5/2020.
//
#include "errorhandler.h"

void error_handler(char *e)
{
    perror(e);
    exit(1);
}

