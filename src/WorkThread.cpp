#include "WorkThread.h"
#include <iostream>
#include <stdexcept>
#include "ThreadPool.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "Task.h"
#include "stdio.h"

#include <fstream>

#include "json/json.h"

using namespace std;

string json_string(string keyword)
{
    Json::Value root ;
    Json::Value arr ;
	Json::Value elem ;
	elem["title"] = keyword;
	elem["content"] = keyword ;
	arr.append(elem);
    root["files"] = arr;
    Json::FastWriter writer ;
    Json::StyledWriter stlwriter ;
    return stlwriter.write(root);
}


void WorkThread::run()
{
    while (true)
    {
        Task t;
        p_pool_->getTaskFromQueue(&t);

        string result = json_string(t.expression_);
#ifndef NDEBUG
        cout <<result<<endl;
#endif
        int ret = sendto(t.server_fd_,result.c_str(),result.size(),0,(struct sockaddr *)&t.address_,t.len_);
        if (ret == -1)
        {
            throw std::runtime_error("sendto error");
        }
        sleep(1);
    }
}

bool WorkThread::regeditThreadPool(ThreadPool *p_pool)
{
    if (!p_pool)
    {
        throw runtime_error("can not regeditThreadPool ");
        return false;
    }
    p_pool_ = p_pool;
    return true;
}

int WorkThread::compute(char *expression)
{
    int result = 0, lo = 0, ro = 0;
    char oper = '0';
    printf("%s\n", expression);
    sscanf(expression, "%d%c%d", &lo, &oper, &ro);
    // cout<< lo << ro << oper <<endl;
    switch (oper)
    {
    case '+':
        result = lo + ro;
        break;
    case '-':
        result = lo - ro;
        break;
    case '*':
        result = lo * ro;
        break;
    case '/':
        result = lo / ro;
        break;
    case '%':
        result = lo % ro;
    default:
        result = 0;
        break;
    }
    return result;
}
