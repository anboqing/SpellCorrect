#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <queue>
#include <stdlib.h>
#include <stdio.h>

#include "Task.h"
#include "Diction.h"
#include "WorkThread.h"
#include "Configure.h"
#include "ThreadPool.h"
#include "Log.h"
#include "Query.h"
#include "EncodingConverter.h"
#include "json/json.h"
using namespace std;

string json_string(string keyword)
{
    EncodingConverter converter;
    //get edit distance
    Json::Value root ;
    Json::Value arr ;
    keyword = converter.utf8_to_gbk(keyword);
    Query *pquery = Query::getInstance();
    vector<pair<string, int> > res_vec = pquery->getFamiliarWordsByStruct(keyword);
    for (vector<pair<string, int> >::iterator iter = res_vec.begin(); iter != res_vec.end(); ++iter)
    {
        //serilize result into json;
        Json::Value elem ;
        elem["title"] = converter.gbk_to_utf8((*iter).first);
        elem["content"] = (*iter).second ;
        arr.append(elem);
    }
    root["files"] = arr;
    Json::FastWriter writer ;
    Json::StyledWriter stlwriter ;
    return stlwriter.write(root);
}


void WorkThread::run()
{
    // #ifndef NDEBUG
    //     string str( "thread start id is : ");
    //     WRITE_NUM(str, (int)get_tid());
    // #endif
    while (true)
    {
        Task t;
        p_pool_->getTaskFromQueue(&t);
        string result = json_string(t.expression_);
        int ret = sendto(t.server_fd_, result.c_str(), result.size(), 0, (struct sockaddr *)&t.address_, t.len_);
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