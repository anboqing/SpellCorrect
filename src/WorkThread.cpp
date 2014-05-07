
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

map<string, int> getFamiliarWords(const string &keyword)
{
    Configure *conf = Configure::getInstance();
    string diction_path = conf->getConfigByName("diction_path");
    string home_path = conf->getConfigByName("home_path");
    string path = home_path + diction_path;

    Diction dict(path.c_str());

    map<string, int> diction_map = dict.getDictMap();
#ifndef NDEBUG
    string str(" diction_map.size () is : ");
    Log::APEND_NUM(str, diction_map.size());
#endif
    //define a priority queue to save:
    //  pair < distance , pair < word , frequency > >
    std::priority_queue<pair<int, map<string, int> >,
        std::vector<pair<int, map<string, int> > >,
        std::greater<pair<int, map<string, int> > > > result_queue ;

    for ( map<string, int>::iterator ix = diction_map.begin(); ix != diction_map.end(); ++ix)
    {
        //find the edit Distance between keyword and each word in the map
        int dis = getEditDistance((*ix).first, keyword);
        //id ed < MAX_DIS then add the word into result map
        Configure *pconf = Configure::getInstance();
        string mdis = pconf->getConfigByName("maxdistance");
        int maxdistance = atoi(mdis.c_str());
        if (dis < maxdistance)
        {
            map<string, int> temp;
            temp.insert(*ix);
            pair<int, map<string, int> > dis_pair(dis, temp);
            result_queue.push(dis_pair);
        }
    }
#ifndef NDEBUG
    string str3("*****************result_queue.size() is : ");
    Log::APEND_NUM(str3, result_queue.size());
#endif
    //select the top n word into a map and return it;
    map<string, int> result_map;
    Configure *pconf = Configure::getInstance();
    string topk_str = pconf->getConfigByName("topk");
    int topk_int = atoi(topk_str.c_str());
    while ( --topk_int && (!result_queue.empty()))
    {
        map<string, int> mpp = result_queue.top().second;
        string first = (*(mpp.begin())).first;
        int second = (*(mpp.begin())).second;
        result_map.insert(make_pair(first, second));
        result_queue.pop();
    }
#ifndef NDEBUG
    string str2("*****************result_map.size() is : ");
    Log::APEND_NUM(str2, result_map.size());
#endif
    return result_map;
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
    Log::APEND_NUM(str, get_tid());
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