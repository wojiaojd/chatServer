cmake_minimum_required(VERSION 3.16)
project(chatServer C)

set(CMAKE_C_STANDARD 99)

add_executable(chatServer
        main.c
        pthreadpool.h
        sockqueue.h
        pthreadpool.c
        errorhandler.h
        sockqueue.c
        errorhandler.c
        database.h
        database.c
        usrData.h
        usrData.c
        sockHandler.h
        sockHandler.c rbTree.h rbTree.c sockPackage.h sockPackage.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
target_link_libraries(chatServer libmysqlclient.so)
target_link_libraries(chatServer libhiredis.so)