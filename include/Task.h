#ifndef TASK_H_
#define TASK_H_
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Copyable.h"
class Task : public Copyable{
public:
    Task():server_fd_(0){
        memset(expression_,0,sizeof(char)*512);
    }
    char expression_[512];
    int server_fd_;
    struct sockaddr_in address_;
    socklen_t len_;
};
#endif