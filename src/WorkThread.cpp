
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <map>
#include <string>

#include "Task.h"
#include "stdio.h"
#include "Diction.h"
#include "EditDistance.h"
#include "WorkThread.h"
#include "Configure.h"
#include "ThreadPool.h"
#include "Log.h"

#include "json/json.h"
using namespace std;

map<string, int> getFamiliarWords(const string &keyword, int maxdistance = 5)
{
    Configure *conf = Configure::getInstance();
    string diction_path = conf->getConfigByName("diction_path");
    string home_path = conf->getConfigByName("home_path");
    string path = home_path + diction_path;

    Diction dict(path.c_str());

    map<string, int> mp = dict.getDictMap();
#ifndef NDEBUG
    string str(" mp.size () is : ");
    Log::APEND_NUM(str,mp.size());
#endif
    map<string, int> result;
    for ( map<string, int>::iterator ix = mp.begin(); ix != mp.end(); ++ix)
    {
        //find the edit Distance between keyword and each word in the map
        int dis = getEditDistance((*ix).first, keyword);
        
        //id ed < MAX_DIS then add the word into result map
        if (dis < maxdistance && (*ix).second >= 50)
        {
            result[(*ix).first] = (*ix).second;
        }
    }
    return result;
}

string json_string(string keyword)
{
    //get edit distance
    Json::Value root ;
    Json::Value arr ;

    map<string, int> mp = getFamiliarWords(keyword);
#ifdef NDEBUG
    Log::WRITE_LOG(string("json_string prepaired ok!"));
#endif
    for (map<string, int>::iterator iter = mp.begin(); iter != mp.end(); ++iter)
    {
        //serilize result into json;
        Json::Value elem ;
        elem["title"] = (*iter).first;
        elem["content"] = (*iter).second ;
        arr.append(elem);
    }
#ifdef NDEBUG
    Log::WRITE_LOG(string("json_string add to JSON ok!" ));
#endif
    root["files"] = arr;
    Json::FastWriter writer ;
    Json::StyledWriter stlwriter ;
    return stlwriter.write(root);
}


void WorkThread::run()
{
#ifndef NDEBUG
    string str( "thread start id is : ");
    Log::APEND_NUM(str,get_tid());
#endif
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